#ifndef VIEWEDITUSERS_H
#define VIEWEDITUSERS_H

#include <QDialog>
#include <QDir>
#include <QMessageBox>

#include <QDebug>

namespace Ui {
class ViewEditUsers;
}

class ViewEditUsers : public QDialog
{
    Q_OBJECT

public:
    explicit ViewEditUsers(QWidget *parent = 0);
    ~ViewEditUsers();

private:
    Ui::ViewEditUsers *ui;

    QString m_fileText;
    QStringList m_entries;

private slots:
    void onBack();
    void reject();

    void onDelete();
    void on_userChanged(QString user);
    void on_fileChange(QString file);

    void on_headerClick(int column);

};

#endif // VIEWEDITUSERS_H
