#include "vieweditchoice.h"
#include "ui_vieweditchoice.h"

ViewEditChoice::ViewEditChoice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewEditChoice)
{
    ui->setupUi(this);
    this->setWindowTitle("Total Prediction");

    ui->headingLabel->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 45px;");
    ui->backButton->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 25px;");
    ui->testButton->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 30px;");
    ui->userButton->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 30px;");



}

ViewEditChoice::~ViewEditChoice()
{
    delete ui;
}

void ViewEditChoice::onUsers()
{

    //Verify > 0  _original.txt files exist
    bool present = false;
    QDir dir(QDir::currentPath());
    foreach(QString file, dir.entryList())
    {
        if(file.right(9) == "_user.txt")
        {
            present = true;
            break;
        }
    }

    if(present)
    {
        ViewEditUsers *win = new ViewEditUsers(this);
        win->show();
        this->hide();
    }
    else
    {
        QMessageBox::critical(this, "Error",
                                         "No users have been created yet.",
                                         QMessageBox::Ok);
    }

}

void ViewEditChoice::onTests()
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
        ViewEditTests *win = new ViewEditTests(this);
        win->show();
        this->hide();
    }
    else
    {
        QMessageBox::critical(this, "Error",
                                         "No tests have been created yet.",
                                         QMessageBox::Ok);
    }

}

void ViewEditChoice::onBack()
{
    reject();
}

void ViewEditChoice::reject()
{
    this->parentWidget()->show();
    delete this;
}
