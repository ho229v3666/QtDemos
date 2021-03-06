﻿/**
 * @brief Video Player
 * @anchor Ho 229
 * @date 2021/4/14
 */

#include "audiooutput.h"
#include "videoplayer.h"
#include "videoplayer_p.h"
#include "videorenderer.h"
#include "subtitlerenderer.h"

#include <QThread>
#include <QTimerEvent>

VideoPlayer::VideoPlayer(QQuickItem *parent) :
    QQuickFramebufferObject(parent),
    d_ptr(new VideoPlayerPrivate(this))
{
    Q_D(VideoPlayer);

    d->decodeThread = new QThread(this);

    d->decoder = new FFmpegDecoder(nullptr);
    d->decoder->moveToThread(d->decodeThread);

    d->audioOutput = new AudioOutput(d->decoder, this);

    d->subtitleRenderer = new SubtitleRenderer(this);
    d->subtitleRenderer->setRenderTarget(QQuickPaintedItem::FramebufferObject);
}

VideoPlayer::~VideoPlayer()
{
    Q_D(VideoPlayer);

    if(d->isPlaying)
        this->play(false);

    d->decoder->deleteLater();

    // Delete the VideoPlayerPrivate
    delete d;
}

QQuickFramebufferObject::Renderer *VideoPlayer::createRenderer() const
{
    return new VideoRenderer(d_ptr);    // Create custom renderer
}

void VideoPlayer::setSource(const QUrl& source)
{
    Q_D(VideoPlayer);

    if(!source.isValid())
        return;

    d->decoder->setUrl(source);

    emit sourceChanged(source);
}

QUrl VideoPlayer::source() const
{
    return d_ptr->decoder->url();
}

void VideoPlayer::play(bool playing)
{
    Q_D(VideoPlayer);

    if(d->isPlaying == playing)
        return;

    if(playing)
    {
        if(d->decoder->state() == FFmpegDecoder::Closed)
        {
            if(d->decoder->load())
            {
                d->isVideoInfoChanged = true;
                d->audioOutput->setAudioFormat(d->decoder->audioFormat());

                emit loaded();
            }
            else
            {
                qCritical() << __FUNCTION__ << ": Source load failed.";
                emit error(ResourceError);
                return;
            }
        }

        qDebug() << "fps:" << d->decoder->fps();

        d->interval = static_cast<int>(1000 / d->decoder->fps());
        d->timerId = this->startTimer(d->interval);
        d->lastDiff = 0;
        d->totalStep = 0;

        d->audioOutput->play();
    }
    else
    {
        if(!d->isPaused)
            this->killTimer(d->timerId);

        d->audioOutput->stop();
        d->decoder->release();

        d->position = 0;

        emit positionChanged(d->position);

        this->update();
    }

    d->isPlaying = playing;
    emit playingChanged(playing);
}

bool VideoPlayer::isPlaying() const
{
    return d_ptr->isPlaying;
}

void VideoPlayer::pause(bool paused)
{
    Q_D(VideoPlayer);

    if(d->isPaused == paused || !d->isPlaying)
        return;

    if(paused)
    {
        this->killTimer(d->timerId);
        d->audioOutput->pause();
    }
    else
    {
        d->timerId = this->startTimer(d->interval);
        d->audioOutput->resume();
    }

    d->isPaused = paused;
    emit pausedChanged(paused);
}

bool VideoPlayer::isPaused() const
{
    return d_ptr->isPaused;
}

void VideoPlayer::setVolume(qreal volume)
{
    Q_D(VideoPlayer);
    d->audioOutput->setVolume(volume);
    emit volumeChanged(volume);
}

qreal VideoPlayer::volume() const
{
    return d_ptr->audioOutput->volume();
}

int VideoPlayer::audioTrackCount() const
{
    return d_ptr->decoder->audioTrackCount();
}

int VideoPlayer::subtitleTrackCount() const
{
    return d_ptr->decoder->subtitleTrackCount();
}

int VideoPlayer::duration() const
{
    return d_ptr->decoder->duration();
}

int VideoPlayer::position() const
{
    return d_ptr->position;
}

bool VideoPlayer::hasVideo() const
{
    return d_ptr->decoder->hasVideo();
}

bool VideoPlayer::hasAudio() const
{
    return d_ptr->decoder->hasAudio();
}

void VideoPlayer::seek(int position)
{
    Q_D(VideoPlayer);
    d->decoder->seek(position);

    d->lastDiff = 0;
    d->totalStep = 0;

    emit positionChanged(position);
}

void VideoPlayer::trackedAudio(int index)
{
    Q_D(VideoPlayer);
    d->decoder->trackedAudio(index);
}

void VideoPlayer::trackSubtitle(int index)
{
    Q_D(VideoPlayer);
    d->decoder->trackSubtitle(index);
}

void VideoPlayer::timerEvent(QTimerEvent *)
{
    Q_D(VideoPlayer);

    d->isUpdated = true;
    this->update();

    d->subtitleRenderer->setSize(this->size());
    d->subtitleRenderer->render(d->decoder->takeSubtitleFrame());

    if(d->decoder->position() != d->position)
    {
        d->position = d->decoder->position();
        emit positionChanged(d->position);
    }

    if(!d->decoder->hasFrame() && d->decoder->isDecodeFinished())
    {
        this->play(false);
        return;
    }

    const qreal diff = d->decoder->diff();

    if(diff >= ALLOW_DIFF)          // Too slow
    {
        if(d->interval <= 10 || diff > ALLOW_DIFF * 2)
        {
            AVFrame *frame = d->decoder->takeVideoFrame();
            av_frame_free(&frame);
        }
        else
        {
            if(diff - d->lastDiff > ALLOW_DIFF / 2 && d->totalStep < 9)
            {
                ++d->totalStep;
                --d->interval;
                this->updateTimer();
            }
        }
    }
    else if(diff <= -ALLOW_DIFF)    // Too quick
    {
        if(diff - d->lastDiff > -ALLOW_DIFF / 2)
        {
            --d->totalStep;
            ++d->interval;
            this->updateTimer();
        }
    }

    d->lastDiff = diff;
}

void VideoPlayer::updateTimer()
{
    Q_D(VideoPlayer);

    this->killTimer(d->timerId);
    d->timerId = this->startTimer(d->interval);
}
