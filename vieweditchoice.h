#ifndef VIEWEDITCHOICE_H
#define VIEWEDITCHOICE_H

#include <QDialog>

#include <QDir>
#include <QMessageBox>

#include "viewedittests.h"
#include "vieweditusers.h"

namespace Ui {
class ViewEditChoice;
}

class ViewEditChoice : public QDialog
{
    Q_OBJECT

public:
    explicit ViewEditChoice(QWidget *parent = 0);
    ~ViewEditChoice();

private:
    Ui::ViewEditChoice *ui;


private slots:
    void onUsers();
    void onTests();
    void onBack();
    void reject();
};

#endif // VIEWEDITCHOICE_H
