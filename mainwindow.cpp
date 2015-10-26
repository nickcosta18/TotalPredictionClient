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
                                     "This application will assist you with cognative rehabilitation through letter prediction exercises. Using this app, you will continually guess the next letter in a text file until the end is reached. \n\n"
                                     "1) Import an exercise. Exercises should be provided to you by your physician. Note that more exercises are hosted at www.totalprediction.weebly.com. \n\n"
                                     "2) Take the exercise. Choose the exercise you want to complete. When the exercise begins, you will continually guess the next letter in the paragraph until the exercise is complete. \n\n"
                                     "3) You can view information about your performances on the exercises.",
                                     QMessageBox::Ok);
}

void MainWindow::onCreateNewClick()
{
        QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString importFile = QFileDialog::getOpenFileName(this, "Import File", desktop, "Text File (*.txt)");

        //On Cancel
        if(importFile == "")
        {
            return;
        }


        QStringList pathParts = importFile.split("/");
        QString fileBase = pathParts[pathParts.length() - 1].left(pathParts[pathParts.length() - 1].length() - 4);


        //Check if name already exists
        bool found = false;
        QMessageBox::StandardButton reply = QMessageBox::No;
        QDir dir(QDir::currentPath());
        foreach(QString file, dir.entryList())
        {
            if(file.right(13) == "_original.txt" && file.left(file.length() - 13) == fileBase)
            {
                found = true;
                reply = QMessageBox::question(this, "Overwrite?",
                                                 "An exercise by this name already exists. Would you like to overwrite it?",
                                                 QMessageBox::Yes | QMessageBox::No);

                break;
            }
        }

        if( found && reply == QMessageBox::No)
        {
            return;
        }


        QString fileText;

        QFile file( importFile );
        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&file);
            fileText = in.readAll();
            file.close();
        }else{
            QMessageBox::critical(this, "Error",
                                  "There was an error importing this file.",
                                   QMessageBox::Ok);
            return;
        }

        QStringList parts = fileText.split("<>");
        bool calibratePresent;
        if(parts.length() == 5)
        {
            calibratePresent = true;
        }
        else if(parts.length() == 4)
        {
            calibratePresent = false;
        }
        else
        {
            QMessageBox::critical(this, "Error",
                                  "There was an error importing this file.",
                                   QMessageBox::Ok);
            return;
        }

        QFile file2( fileBase + "_original.txt" );
        if ( file2.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream stream( &file2 );
            stream << parts[1];
            file2.close();
        }else{
            QMessageBox::critical(this, "Error",
                                  "There was an error importing this file.",
                                   QMessageBox::Ok);
            return;
        }


        QFile file3( fileBase + "_missed.txt" );
        if ( file3.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream stream( &file3 );
            stream << parts[2];
            file3.close();
        }else{
            QMessageBox::critical(this, "Error",
                                  "There was an error importing this file.",
                                   QMessageBox::Ok);
            return;
        }

        if(calibratePresent)
        {
            QFile file3( fileBase + "_calibrated.txt" );
            if ( file3.open(QIODevice::WriteOnly | QIODevice::Text) )
            {
                QTextStream stream( &file3 );
                stream << parts[3];
                file3.close();
            }else{
                QMessageBox::critical(this, "Error",
                                      "There was an error importing this file.",
                                       QMessageBox::Ok);
                return;
            }
        }


        m_msg = QMessageBox::information(this, "Update",
                                         "Exercise '" + fileBase + "' Imported!",
                                         QMessageBox::Ok);
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
        FileSelect *win = new FileSelect(this, "user");
        win->show();
        this->hide();
    }
    else
    {
        m_msg = QMessageBox::critical(this, "Error",
                                         "No exercises have been imported yet.",
                                         QMessageBox::Ok);
    }
}


void MainWindow::onViewEditDataClick()
{
    ViewEditChoice *win = new ViewEditChoice(this);
    win->show();
    this->hide();
}

