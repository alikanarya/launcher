#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);
    /* START PROCESS WITHIN THIS APP WITH "WINDOWS SHELL COMMAND" IF THERE IS A PERMISSION ISSUE
    #ifdef Q_OS_WIN
        int result = (int)::ShellExecuteA(0, "open", exeFileName.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
        if (SE_ERR_ACCESSDENIED == result) {
            result = (int)::ShellExecuteA(0, "runas", exeFileName.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
        }
        if (result <= 32) { // error handling }
        qDebug() << result;
    #else
        if (!QProcess::startDetached(exeFileName, exePath)) { qDebug() << "error"; }
    #endif
    */
    /* START PROCESS WITHIN THIS APP WITH "QPROCESS"
    QProcess *myProcess = new QProcess(parent);
    connect(myProcess, SIGNAL(error(QProcess::ProcessError)),this, SLOT(slotProcessError(QProcess::ProcessError)));
    connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus){ evExitStatus(exitStatus); });

    //myProcess->setWorkingDirectory(exePath);
    //myProcess->start(exeFileName);

    //QStringList args = {};
    //myProcess->startDetached(exeFileName,args,exePath);
    */

    regCrashDialog(1);  // disable windows crash dialog

    exeFoundStatePrev = false;
    exeFoundState = false;

    connect(&checkTimer, SIGNAL(timeout()),this, SLOT(checkProcessStatus()));

    checkTimer.start(timerPeriod);



}

MainWindow::~MainWindow(){

    regCrashDialog(0);  // enable windows crash dialog
    delete ui;
}

void MainWindow::regCrashDialog(int value){

    // value 0; enable dialog, 1: disable dialog
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\Windows Error Reporting", QSettings::NativeFormat);
    settings.setValue("DontShowUI", value);
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

void MainWindow::checkProcessStatus(){

    QProcess process;
    process.setReadChannel(QProcess::StandardOutput);
    process.setReadChannelMode(QProcess::MergedChannels);
    process.start("wmic.exe /OUTPUT:STDOUT PROCESS get Caption");

    process.waitForStarted(100);
    process.waitForFinished(500);

    QByteArray result = process.readAll();
    //qDebug() << "Read" << list.length() << "bytes"; qDebug() << list;
    QList<QByteArray> lines = result.split('\n');
    QList<QString> strLines;

    bool lock = false;
    exeFoundState = false;
    for (int i=0; i<lines.size(); i++) {
        strLines.append(QString(lines[i].constData()));
        //qDebug() << strLines[i];
        if (strLines[i].indexOf(processName)>=0 && !lock){
            exeFoundState = true;
            lock = true;
        }
    }
    if (exeFoundState && !exeFoundStatePrev)
        qDebug() << processName << "running";

    if (!exeFoundState && exeFoundStatePrev)
        qDebug() << processName << "not running";

    if (exeFoundState != exeFoundStatePrev)
        exeFoundStatePrev = exeFoundState;
}
