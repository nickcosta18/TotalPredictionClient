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
    QDir dir(QDir::currentPath());

    bool calibExists = false;

    foreach(QString file, dir.entryList())
    {
        if(file.right(15) == "_calibrated.txt")
        {
            ui->fileList->addItem(file.left(file.length() - 15));
            calibExists = true;
        }
    }

    foreach(QString file, dir.entryList())
    {
        if(file.right(13) == "_original.txt")
        {
            ui->fileList_original->addItem(file.left(file.length() - 13));
        }
    }

    if(calibExists)
    {
        ui->originalCheckbox->setChecked(false);
        ui->calibratedCheckbox->setChecked(true);

        ui->fileList_original->setCurrentRow(0);

        ui->fileList->setFocus();
        ui->fileList->setCurrentRow(0);
    }
    else
    {
        ui->originalCheckbox->setChecked(true);
        ui->calibratedCheckbox->setChecked(false);

        ui->calibratedCheckbox->setEnabled(false);
        ui->originalCheckbox->setEnabled(false);
        ui->fileList->setEnabled(false);

        ui->fileList_original->setFocus();
        ui->fileList_original->setCurrentRow(0);
    }

    connect(ui->calibratedCheckbox, SIGNAL(clicked(bool)), this, SLOT(toggled(bool)));
    connect(ui->originalCheckbox, SIGNAL(clicked(bool)), this, SLOT(toggled(bool)));
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
    if (ui->calibratedCheckbox->isChecked())
    {
        file = ui->fileList->currentItem()->text();
        calib = true;
    }
    else
    {
        file = ui->fileList_original->currentItem()->text();
        calib = false;
    }


    //determine which one it came from
    if(calib)
    {
        file.append("_calibrated.txt");
    }
    else
    {
        file.append("_original.txt");
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

void FileSelect::toggled(bool checked)
{
    if(QObject::sender() == ui->calibratedCheckbox)
    {
        if(checked)
        {
            ui->originalCheckbox->setChecked(false);
            ui->fileList->setFocus();
        }
        else
        {
            ui->originalCheckbox->setChecked(true);
            ui->fileList_original->setFocus();
        }
    }
    else
    {
        if(checked)
        {
            ui->calibratedCheckbox->setChecked(false);
            ui->fileList_original->setFocus();
        }
        else
        {
            ui->calibratedCheckbox->setChecked(true);
            ui->fileList->setFocus();
        }
    }
}
