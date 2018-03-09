#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void evExitStatus(QProcess::ExitStatus error);
    ~MainWindow();

public slots:
    void slotProcessError(QProcess::ProcessError error);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
