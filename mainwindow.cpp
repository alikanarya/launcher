#include "mainwindow.h"
#include "ui_mainwindow.h"


#ifdef Q_OS_WIN
    #include <windows.h>
    #include <shellapi.h>
#endif
QString exeFileName = "\"D:\\Engineering\\JTWStaticTest\\JTWcrash.exe\"";
//QString exeFileName = "D:/Engineering/Repository WORK/JTWStatic/JTW.exe";
QString exePath = "D:/Engineering/JTWStaticTest";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);
/*
#ifdef Q_OS_WIN
    int result = (int)::ShellExecuteA(0, "open", exeFileName.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
    if (SE_ERR_ACCESSDENIED == result) {
        result = (int)::ShellExecuteA(0, "runas", exeFileName.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
    }
    if (result <= 32) {
        // error handling
    }
    qDebug() << result;
#else
    if (!QProcess::startDetached(exeFileName, exePath)) {
        qDebug() << "error";
    }
#endif
*/

    QProcess *myProcess = new QProcess(parent);
    connect(myProcess, SIGNAL(error(QProcess::ProcessError)),this, SLOT(slotProcessError(QProcess::ProcessError)));
    connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus){ evExitStatus(exitStatus); });

    // DISABLE WINDOWS ERROR DIALOG
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\Windows Error Reporting", QSettings::NativeFormat);
    settings.setValue("DontShowUI", 1);

    myProcess->setWorkingDirectory(exePath);
    myProcess->start(exeFileName);

}

MainWindow::~MainWindow(){

    // ENABLE WINDOWS ERROR DIALOG
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\Windows Error Reporting", QSettings::NativeFormat);
    settings.setValue("DontShowUI", 0);

    delete ui;
}

void MainWindow::slotProcessError(QProcess::ProcessError error){

    switch (error) {
        case QProcess::FailedToStart :
            qDebug() << "FailedToStart";
            break;
        case QProcess::Crashed :
            qDebug() << "Crashed";
            break;
        case QProcess::Timedout :
            qDebug() << "Timedout";
            break;
        case QProcess::WriteError :
            qDebug() << "WriteError";
            break;
        case QProcess::ReadError :
            qDebug() << "ReadError";
        case QProcess::UnknownError :
            qDebug() << "UnknownError";
            break;
    }
}

void MainWindow::evExitStatus(QProcess::ExitStatus exitStatus){
    //qDebug() << exitStatus;
    switch (exitStatus) {
        case QProcess::NormalExit :
            qDebug() << "Normal...Exit";
            break;
        case QProcess::CrashExit :
            qDebug() << "Crash...Exit";
            break;
    }

}
