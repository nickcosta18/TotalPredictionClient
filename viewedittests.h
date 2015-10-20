#ifndef VIEWEDITTESTS_H
#define VIEWEDITTESTS_H

#include <QDebug>

#include <QMessageBox>
#include <QDialog>
#include <QDir>
#include <QStandardPaths>

namespace Ui {
class ViewEditTests;
}

class ViewEditTests : public QDialog
{
    Q_OBJECT

public:
    explicit ViewEditTests(QWidget *parent = 0);
    ~ViewEditTests();

private:
    Ui::ViewEditTests *ui;

private slots:
    void onBack();
    void reject();

    void onExport();
    void onDelete();

    void on_testChange(QString fileBase);
};

#endif // VIEWEDITTESTS_H
