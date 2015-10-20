#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>
#include <QFileDialog>

#include "fileselect.h"
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
    void onProctorTestClick();
    void onViewEditDataClick();

};

#endif // MAINWINDOW_H
