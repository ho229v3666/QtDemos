﻿#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "multithreadeddownloader.h"

#include "toast.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void on_downloadBtn_clicked();

    void on_startBtn_clicked();

    void on_pauseBtn_clicked();

    void on_stopBtn_clicked();

private:
    Ui::MainWidget *ui;

    MultithreadedDownloader* m_downloader = nullptr;

    Toast *m_toast = nullptr;

    inline void initUI();
    inline void initSignalSlots();
};
#endif // MAINWIDGET_H
