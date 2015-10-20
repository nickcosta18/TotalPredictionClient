#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Total Prediction");
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::onExitClick()
{
    exit(0);
}

void MainWindow::onInstructionsClick()
{

    m_msg = QMessageBox::information(this, "Instructions",
                                     "1) Create a test.\n"
                                     "2) Calibrate the test.\n"
                                     "3) Proctor the test to patients.\n",
                                     QMessageBox::Ok);
}

void MainWindow::onCreateNewClick()
{
    CreateNew *win = new CreateNew(this);
    win->show();
    this->hide();
}

void MainWindow::onCalibrateTestClick()
{
    //Verify > 0  _original.txt files exist
    bool present = false;
    QDir dir(QDir::currentPath());
    foreach(QString file, dir.entryList())
    {
        if(file.right(13) == "_original.txt")
        {
            present = true;
            break;
        }
    }


    if(present)
    {
        FileSelect *win = new FileSelect(this, "");
        win->show();
        this->hide();
    }
    else
    {
        m_msg = QMessageBox::critical(this, "Error",
                                         "No exercises have been created yet.",
                                         QMessageBox::Ok);
    }
}

void MainWindow::onProctorTestClick()
{
    //Verify > 0  _original.txt files exist
    bool present = false;
    QDir dir(QDir::currentPath());
    foreach(QString file, dir.entryList())
    {
        if(file.right(13) == "_original.txt")
        {
            present = true;
            break;
        }
    }

    if(present)
    {
        UserSelect *win = new UserSelect(this);
        win->show();
        this->hide();
    }
    else
    {
        m_msg = QMessageBox::critical(this, "Error",
                                         "No exercises have been calibrated yet.",
                                         QMessageBox::Ok);
    }
}


void MainWindow::onViewEditDataClick()
{
    ViewEditChoice *win = new ViewEditChoice(this);
    win->show();
    this->hide();
}




void MainWindow::on_recv_newTest(QString name)
{
    m_msg = QMessageBox::information(this, "Update",
                                     "New Exercise '" + name + "' Created!",
                                     QMessageBox::Ok);
}

void MainWindow::on_recv_importTest(QString name)
{
    m_msg = QMessageBox::information(this, "Update",
                                     "Exercise '" + name + "' Imported!",
                                     QMessageBox::Ok);
}


void MainWindow::on_recv_calibrated(QString name, int num)
{
    qDebug() << num;
    m_msg = QMessageBox::information(this, "Update",
                                     "Exercise '" + name + "' has been calibrated \
                                      based on " + QString::number(num) + " input(s)!",
                                     QMessageBox::Ok);
}
