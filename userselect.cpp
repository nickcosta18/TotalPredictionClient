#include "userselect.h"
#include "ui_userselect.h"

UserSelect::UserSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserSelect)
{
    ui->setupUi(this);
    this->setWindowTitle("Total Prediction");
    ui->headingLabel->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 45px;");
    ui->subLabel1->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 30px;");
    ui->subLabel2->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 30px;");




    connect(ui->existingCheck, SIGNAL(clicked(bool)), this, SLOT(toggled(bool)));
    connect(ui->newCheck, SIGNAL(clicked(bool)), this, SLOT(toggled(bool)));

    //fill in existing users if any
    bool usersExist = false;
    QDir dir(QDir::currentPath());
    foreach(QString file, dir.entryList())
    {
        if(file.right(9) == "_user.txt")
        {
            ui->userList->addItem(file.left(file.length() - 9));
            usersExist = true;
        }
    }

    if(usersExist)
    {
        ui->newCheck->setChecked(false);
        ui->existingCheck->setChecked(true);

        ui->userList->setFocus();
        ui->userList->setCurrentRow(0);
    }
    else
    {
        ui->newCheck->setChecked(true);
        ui->existingCheck->setChecked(false);

        ui->existingCheck->setEnabled(false);
        ui->newCheck->setEnabled(false);

        ui->newUser->setFocus();
        ui->userList->setEnabled(false);
    }
}

UserSelect::~UserSelect()
{
    delete ui;
}

void UserSelect::onSubmit()
{
    QString userName;
    if(ui->newCheck->isChecked())
    {
        userName = ui->newUser->text();

        if(userName != "")
        {
            //Check if name already exists
            QDir dir(QDir::currentPath());
            foreach(QString file, dir.entryList())
            {
                if(file.right(9) == "_user.txt" && file.left(file.length() - 9) == userName)
                {
                    QMessageBox::critical(this, "Issue",
                                                     "A user by this name already exists. Please select a new name.",
                                                     QMessageBox::Ok);

                    return;
                }
            }

            //Since a new user is being added, create a _user.txt file
             QFile file( userName + "_user.txt" );
             if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
             {
                 file.close();
             }
        }
        else
        {
            QMessageBox::critical(this, "Issue",
                                             "You must enter a name for the new user.",
                                             QMessageBox::Ok);

            return;
        }
    }
    else
    {
        userName = ui->userList->currentItem()->text();
    }

    FileSelect *win = new FileSelect(this->parentWidget(), userName);
    win->show();
    delete this;

}

void UserSelect::onCancel()
{
    reject();
}

void UserSelect::reject()
{
    this->parentWidget()->show();
    delete this;
}

void UserSelect::toggled(bool checked)
{
    if(QObject::sender() == ui->newCheck)
    {
        if(checked)
        {
            ui->existingCheck->setChecked(false);
            ui->newUser->setFocus();
        }
        else
        {
            ui->existingCheck->setChecked(true);
            ui->userList->setFocus();
        }
    }
    else
    {
        if(checked)
        {
            ui->newCheck->setChecked(false);
            ui->userList->setFocus();
        }
        else
        {
            ui->newCheck->setChecked(true);
            ui->newUser->setFocus();
        }
    }
}
