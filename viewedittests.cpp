#include "viewedittests.h"
#include "ui_viewedittests.h"

ViewEditTests::ViewEditTests(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewEditTests)
{
    ui->setupUi(this);
    this->setWindowTitle("Total Prediction");

    ui->headingLabel->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 45px;");
    ui->backButton->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 25px;");
    ui->deleteButton->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 25px;");


    QDir dir(QDir::currentPath());
    foreach(QString file, dir.entryList())
    {
        if(file.right(13) == "_original.txt")
        {
            ui->testList->addItem(file.left(file.length() - 13));
        }
    }

    ui->testList->setFocus();
    ui->testList->setCurrentRow(0);
}

ViewEditTests::~ViewEditTests()
{
    delete ui;
}

void ViewEditTests::onBack()
{
    reject();
}

void ViewEditTests::reject()
{
    this->parentWidget()->show();
    delete this;
}

void ViewEditTests::onDelete()
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Stop?",
                                     "Are you sure you want to delete this exercise?",
                                     QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        QString fileBase = ui->testList->currentItem()->text();

        //remove _original.txt
        QFile file( fileBase + "_original.txt" );
        file.remove();

        //remove _missed.txt
        QFile file2( fileBase + "_missed.txt" );
        file2.remove();

        //remove _calibrated.txt if it exists
        QFile file3( fileBase + "_calibrated.txt" );
        file3.remove();

        ui->testList->clear();

        QDir dir(QDir::currentPath());
        bool found = false;
        foreach(QString file, dir.entryList())
        {
            if(file.right(13) == "_original.txt")
            {
                ui->testList->addItem(file.left(file.length() - 13));
                found = true;
            }
        }

        if(found)
        {
            ui->testList->setFocus();
            ui->testList->setCurrentRow(0);
        }
        else //last test was deleted
        {
            reject();
        }
    }

}

