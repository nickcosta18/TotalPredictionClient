#include "fileselect.h"
#include "ui_fileselect.h"

FileSelect::FileSelect(QWidget *parent, QString user) :
    QDialog(parent),
    ui(new Ui::FileSelect)
{
    ui->setupUi(this);
    ui->headingLabel->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 45px;");
    ui->headingLabel2->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 30px;");
    ui->headingLabel3->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 30px;");
    this->setWindowTitle("Total Prediction");

    m_user = user;
    m_isCalibrateMode = (user == "");
    QDir dir(QDir::currentPath());

    if(m_isCalibrateMode)
    {
        ui->headingLabel2->hide();
        ui->headingLabel3->hide();
        ui->fileList_original->hide();
        ui->fileList->move(290, 125);

        foreach(QString file, dir.entryList())
        {
            if(file.right(13) == "_original.txt")
            {
                ui->fileList->addItem(file.left(file.length() - 13));
            }
        }
    }
    else
    {
        foreach(QString file, dir.entryList())
        {
            if(file.right(15) == "_calibrated.txt")
            {
                ui->fileList->addItem(file.left(file.length() - 15));
            }
        }

        foreach(QString file, dir.entryList())
        {
            if(file.right(13) == "_original.txt")
            {
                ui->fileList_original->addItem(file.left(file.length() - 13));
            }
        }
    }

    ui->fileList->setFocus();
    ui->fileList->setCurrentRow(0);
}

FileSelect::~FileSelect()
{
    delete ui;
}

void FileSelect::onCancel()
{
    reject();
}

void FileSelect::onSubmit()
{
    QString file;
    bool calib;
    if (ui->fileList->hasFocus())
    {
        file = ui->fileList->currentItem()->text();
        calib = true;
    }
    else
    {
        file = ui->fileList_original->currentItem()->text();
        calib = false;
    }

    if(m_isCalibrateMode)
    {
        file.append("_original.txt");
    }
    else
    {
        //determine which one it came from
        if(calib)
        {
            file.append("_calibrated.txt");
        }
        else
        {
            file.append("_original.txt");
        }
    }
    TestWindow *win = new TestWindow(this->parentWidget(), file, m_user);
    win->show();
    win->startOnClick();
    delete this;
}

void FileSelect::reject()
{
    this->parentWidget()->show();
    delete this;
}
