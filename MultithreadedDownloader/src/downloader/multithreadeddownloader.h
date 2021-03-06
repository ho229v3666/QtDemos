﻿/**
 * @brief MultithreadedDownloader
 * @anchor Ho229<2189684957@qq.com>
 * @date 2021/2/1
 */

#ifndef MULTITHREADEDDOWNLOADER_H
#define MULTITHREADEDDOWNLOADER_H

#include <QFile>

#include "downloadmission.h"
#include "multithreadeddownloaderwriter.h"

class QNetworkAccessManager;

class MultithreadedDownloader : public AbstractMission
{
    Q_OBJECT
public:
    enum Error
    {
        OpenFileFailed,
        DownloadFailed
    };

    explicit MultithreadedDownloader(QObject *parent = nullptr);
    ~MultithreadedDownloader() Q_DECL_OVERRIDE;

    /**
     * @brief 设置下载线程数
     * @param num 下载线程数
     */
    void setThreadCount(int num){ m_threadCount = num; }
    int threadCount() const { return m_threadCount; }

    /**
     * @brief 初始化下载
     * @note 获取下载文件大小，文件名
     * @return 是否成功
     */
    bool load();

    /**
     * @brief 设置下载文件名
     * @param name 下载文件名
     */
    void setFileName(const QString& name){ m_writer->setFileName(name); }
    QString fileName() const { return m_writer->fileName(); }

    /**
     * @return 下载文件大小
     */
    qint64 downloadSize() const { return m_writer->size(); }

    /**
     * @return 目标服务器是否支持 Range Header
     * @note 如果不支持 Range，多线程下载和暂停将不可用
     */
    bool isRangeSupport() const { return m_isRangeSupport; }

    /**
     * @brief The interval at downloadProgress() will emit.
     * @default 500
     */
    void setNotifyInterval(int interval) { m_notifyInterval = interval; }
    int notifyInterval() const { return m_notifyInterval; }

    QNetworkReply::NetworkError networkError() const { return m_networkError; }
    QString networkErrorString() const { return m_networkErrorString; }

    void start() Q_DECL_OVERRIDE;
    void pause() Q_DECL_OVERRIDE;
    void stop()  Q_DECL_OVERRIDE;

signals:
    void error(const MultithreadedDownloader::Error err);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void errorHanding(QNetworkReply::NetworkError err);     // 错误处理
    void on_finished();

private:
    QNetworkAccessManager* m_manager = nullptr;
    MultithreadedDownloaderWriter *m_writer = nullptr;

    int m_threadCount = 0;
    int m_finishedCount = 0;

    int m_timerId = 0;

    int m_notifyInterval = 500;

    bool m_isRangeSupport = false;

    QList<DownloadMission *> m_missions;

    QNetworkReply::NetworkError m_networkError = QNetworkReply::NoError;
    QString m_networkErrorString;

    inline DownloadMission* createMission(qint64 start, qint64 end);
    inline void destoryMissions();
    inline void updateProgress();
    inline void reset();

    void timerEvent(QTimerEvent* event) Q_DECL_OVERRIDE;        // 更新进度
};

#endif // MULTITHREADEDDOWNLOADER_H
