#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>

#include "createnew.h"
#include "fileselect.h"
#include "userselect.h"
#include "vieweditchoice.h"


#include <QDebug>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QMessageBox::StandardButton m_msg;

private slots:
    void onExitClick();
    void onInstructionsClick();
    void onCreateNewClick();
    void onCalibrateTestClick();
    void onProctorTestClick();
    void onViewEditDataClick();

public slots:
    void on_recv_newTest(QString name);
    void on_recv_importTest(QString name);
    void on_recv_calibrated(QString name, int num);

};

#endif // MAINWINDOW_H
