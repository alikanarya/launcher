#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QTimer>

#ifdef Q_OS_WIN
    #include <windows.h>
    #include <shellapi.h>

#endif

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    QString exeFileName = "\"D:\\Engineering\\JTWStaticTest\\JTW.exe\"";
    //QString exeFileName = "D:/Engineering/Repository WORK/JTWStatic/JTW.exe";
    QString exePath = "D:/Engineering/JTWStaticTest";
    QString processName = "JTW.exe";


    QTimer checkTimer;
    int timerPeriod = 1000;

    explicit MainWindow(QWidget *parent = 0);
    void evExitStatus(QProcess::ExitStatus error);
    ~MainWindow();

public slots:
    void slotProcessError(QProcess::ProcessError error);
    void checkProcessStatus();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
