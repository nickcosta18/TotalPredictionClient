#ifndef FILESELECT_H
#define FILESELECT_H

#include <QDialog>
#include <QDir>

#include "testwindow.h"

#include <QDebug>

namespace Ui {
class FileSelect;
}

class FileSelect : public QDialog
{
    Q_OBJECT

public:
    explicit FileSelect(QWidget *parent, QString user);
    ~FileSelect();

private:
    Ui::FileSelect *ui;
    QString m_user;

private slots:
    void onCancel();
    void onSubmit();

    void toggled(bool checked);

    void reject();
};

#endif // FILESELECT_H
