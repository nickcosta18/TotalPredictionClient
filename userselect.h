#ifndef USERSELECT_H
#define USERSELECT_H

#include <QDialog>

#include "fileselect.h"

namespace Ui {
class UserSelect;
}

class UserSelect : public QDialog
{
    Q_OBJECT

public:
    explicit UserSelect(QWidget *parent);
    ~UserSelect();

private:
    Ui::UserSelect *ui;

    void reject();

private slots:
    void onCancel();
    void onSubmit();
    void toggled(bool checked);
};

#endif // USERSELECT_H
