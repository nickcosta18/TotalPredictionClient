#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDateTime>
#include <math.h>

#include <QDebug>

namespace Ui {
class TestWindow;
}

class TestWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget *parent, QString fileName, QString user);
    ~TestWindow();

    void startOnClick();


private:
    void onFinish();

    Ui::TestWindow *ui;
    QMessageBox::StandardButton m_msg;

    QString m_fileNameBase;

    QString m_fileText;
    int m_currentIndex;
    QString m_visibleText;

    bool m_firstMiss;
    int m_misses;
    int m_correct;

    int m_time;
    int m_timer;
    void timerEvent(QTimerEvent *event);

    QList<int> m_missedIndicies;

    bool m_isCalibrateMode;
    bool m_isOriginalFile;
    QString m_user;

private slots:
    void keyReleaseEvent(QKeyEvent* e);
    void onStop();
    void reject();

signals:
    void send_calibrated(QString name, int num);

};

#endif // TESTWINDOW_H
