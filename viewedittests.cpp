#include "viewedittests.h"
#include "ui_viewedittests.h"

ViewEditTests::ViewEditTests(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewEditTests)
{
    ui->setupUi(this);
    this->setWindowTitle("Total Prediction");

    ui->headingLabel->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 45px;");
    ui->calibrationsLabel->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 45px;");
    ui->backButton->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 25px;");
    ui->deleteButton->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 25px;");
    ui->exportButton->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 25px;");


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

void ViewEditTests::on_testChange(QString fileBase)
{
    ui->textView->clear();

    QFile file(fileBase + "_original.txt");
    QString fileText;
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        fileText = in.readAll();
        file.close();
    }

    QFile file2(fileBase + "_missed.txt");
    QString missedText;
    int numCalibrations;
    QList<int> missedIndicies;
    if (file2.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file2);
        missedText = in.readAll();
        QStringList missTextList = missedText.split(",");
        for(int i = 0; i < missTextList.length() - 1; i++)
        {
            missedIndicies.append(missTextList[i].toInt());
        }
        numCalibrations = missTextList[missTextList.length() - 1].toInt();

        file2.close();
    }

    ui->calibrationsLabel->setText("Calibrations: " + QString::number(numCalibrations));


    //generate color coded html based on missed indicies
    QString blackOpen = "<p style=\"font-size: 18px;color:black\">";
    QString colorOpen = "<p style=\"font-size: 18px;color: rgb(213, 101, 52)\">";
    QString close = "</p>";
    QString letter;
    for(int i = 0; i < fileText.length(); i++)
    {
        letter = fileText[i];
        if(letter == " ") // make spaces visible
            letter = "_";

        if(missedIndicies.contains(i))
        {
            ui->textView->insertHtml(colorOpen + letter + close);
        }
        else
        {
            ui->textView->insertHtml(blackOpen + letter + close);
        }
    }

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


void ViewEditTests::onExport()
{

    QString oldFileText;
    QString newFileText = "<>";

    //combine _missing, _original, and _calibrated into one .txt file
    QString fileBase = ui->testList->currentItem()->text();

    QFile file( fileBase + "_original.txt" );
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        oldFileText = in.readAll();
        newFileText.append(oldFileText);
        newFileText.append("<>");
        file.close();
    }else{
        QMessageBox::critical(this, "Error",
                              "There was an error exporting this file.",
                               QMessageBox::Ok);
        return;
    }

    QFile file2( fileBase + "_missed.txt" );
    if (file2.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file2);
        oldFileText = in.readAll();
        newFileText.append(oldFileText);
        newFileText.append("<>");
        file2.close();
    }else{
        QMessageBox::critical(this, "Error",
                              "There was an error exporting this file.",
                               QMessageBox::Ok);
        return;
    }

    QFile file3( fileBase + "_calibrated.txt" );
    if (file3.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file3);
        oldFileText = in.readAll();
        newFileText.append(oldFileText);
        newFileText.append("<>");
        file3.close();
    }


    QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QFile file4( desktop + "/" + fileBase + ".txt" );
    if ( file4.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream stream( &file4 );
        stream << newFileText;
        file4.close();
    }else{
        QMessageBox::critical(this, "Error",
                              "There was an error exporting this file.",
                               QMessageBox::Ok);
        return;
    }

    QMessageBox::information(this, "Success",
                          "The file has succesfully been exported to your Desktop.",
                           QMessageBox::Ok);

}
