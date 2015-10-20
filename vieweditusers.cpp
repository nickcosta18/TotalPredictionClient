#include "vieweditusers.h"
#include "ui_vieweditusers.h"

ViewEditUsers::ViewEditUsers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewEditUsers)
{
    ui->setupUi(this);
    this->setWindowTitle("Total Prediction");

    ui->headingLabel->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 45px;");
    ui->subLabel->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 25px;");
    ui->backButton->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 25px;");
    ui->deleteButton->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 25px;");

    ui->infoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->infoTable->setColumnWidth(0, 115);
    ui->infoTable->setColumnWidth(1, 126);
    ui->infoTable->setColumnWidth(2, 126);
    ui->infoTable->setColumnWidth(3, 50);
    ui->infoTable->setColumnWidth(4, 180);

    ui->infoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    connect(ui->infoTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(on_headerClick(int)));

    ui->infoTable->verticalHeader()->hide();

    QDir dir(QDir::currentPath());
    foreach(QString file, dir.entryList())
    {
        if(file.right(9) == "_user.txt")
        {
            ui->userList->addItem(file.left(file.length() - 9));
        }
    }

    ui->userList->setFocus();
    ui->userList->setCurrentRow(0);
}

ViewEditUsers::~ViewEditUsers()
{
    delete ui;
}

void ViewEditUsers::onBack()
{
    reject();
}

void ViewEditUsers::reject()
{
    ui->infoTable->clearContents();
    this->parentWidget()->show();
    delete this;
}

void ViewEditUsers::on_userChanged(QString user)
{
    ui->infoTable->clearContents();
    ui->fileFilter->clear();
    ui->fileFilter->addItem("All");
    ui->fileFilter->setCurrentIndex(0);


    QFile file(user + "_user.txt");
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        m_fileText = in.readAll();
        file.close();
    }

    QStringList files;
    QStringList subEntries;
    m_entries = m_fileText.split("|");
    ui->infoTable->setRowCount(m_entries.length() - 1);
    for(int i = 0; i < m_entries.length() - 1; i++)
    {
        subEntries = m_entries[i].split(",");
        if(!files.contains(subEntries[0]))
        {
            files.append(subEntries[0]);
            ui->fileFilter->addItem(subEntries[0]);
        }
        ui->infoTable->setItem(i, 0, new QTableWidgetItem(subEntries[0])); //File
        ui->infoTable->setItem(i, 1, new QTableWidgetItem(subEntries[4] + "%" + " - " +  subEntries[1] + "/" + subEntries[3] )); //Correct
        ui->infoTable->setItem(i, 2, new QTableWidgetItem(subEntries[5] + "%" + " - " +  subEntries[2] + "/" + subEntries[3] )); //Missed
        ui->infoTable->setItem(i, 3, new QTableWidgetItem(subEntries[6])); //Time to Complete
        ui->infoTable->setItem(i, 4, new QTableWidgetItem(subEntries[7])); //Date/Time
    }
}

void ViewEditUsers::onDelete()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Stop?",
                                     "Are you sure you want to delete this user?",
                                     QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        QString user = ui->userList->currentItem()->text();

        //remove _user.txt
        QFile file( user + "_user.txt" );
        file.remove();

        ui->userList->clear();

        QDir dir(QDir::currentPath());
        bool found = false;
        foreach(QString file, dir.entryList())
        {
            if(file.right(9) == "_user.txt")
            {
                ui->userList->addItem(file.left(file.length() - 9));
                found = true;
            }
        }

        if(found)
        {
            ui->userList->setFocus();
            ui->userList->setCurrentRow(0);
        }
        else //last user was deleted
        {
            reject();
        }
    }
}

void ViewEditUsers::on_fileChange(QString file)
{
    ui->infoTable->clearContents();

    QStringList subEntries;
    int totalRows = 0;
    for(int i = 0; i < m_entries.length() - 1; i++)
    {
        subEntries = m_entries[i].split(",");
        if(file == "All" || subEntries[0] == file)
        {
            totalRows++;
        }
    }

    ui->infoTable->setRowCount(totalRows); // must be set before the table can be filled in.

    int rowNum = 0;
    for(int i = 0; i < m_entries.length() - 1; i++)
    {
        subEntries = m_entries[i].split(",");
        if(file == "All" || subEntries[0] == file)
        {
            ui->infoTable->setItem(rowNum, 0, new QTableWidgetItem(subEntries[0])); //File
            ui->infoTable->setItem(rowNum, 1, new QTableWidgetItem(subEntries[4] + "%" + " - " +  subEntries[1] + "/" + subEntries[3] )); //Correct
            ui->infoTable->setItem(rowNum, 2, new QTableWidgetItem(subEntries[5] + "%" + " - " +  subEntries[2] + "/" + subEntries[3] )); //Missed
            ui->infoTable->setItem(rowNum, 3, new QTableWidgetItem(subEntries[6])); //Time to Complete
            ui->infoTable->setItem(rowNum, 4, new QTableWidgetItem(subEntries[7])); //Date/Time
            rowNum++;
        }
    }

}

void ViewEditUsers::on_headerClick(int column)
{
    ui->infoTable->sortByColumn(column);
}
