#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent, QString fileName, QString user) :
    QDialog(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Total Prediction");


    m_user = user;
    m_isOriginalFile = (fileName.right(13) == "_original.txt");

    if(m_isOriginalFile)
    {
        m_fileNameBase = fileName.left(fileName.length() - 13);
    }
    else
    {
        m_fileNameBase = fileName.left(fileName.length() - 15);
    }

    ui->testLabel->setText("Exercise:   " + m_fileNameBase);

    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        m_fileText = in.readAll();
        file.close();
    }
    m_currentIndex = 0;
    m_visibleText = "";

    m_firstMiss = true;
    m_misses = 0;
    m_correct = 0;
    m_time = 0;
}

TestWindow::~TestWindow()
{
    delete ui;
}

void TestWindow::startOnClick()
{

    m_msg = QMessageBox::information(this, "Begin Exercise", "Click Ok to begin the exercise.",
                                     QMessageBox::Ok);

    m_timer = startTimer(1000);

    //autofill if not calibrate mode and \ is found
    while(m_fileText[m_currentIndex] == '\\')
    {
        m_currentIndex++;
        m_visibleText += m_fileText[m_currentIndex];
        ui->textView->setPlainText(m_visibleText);
        m_currentIndex++;
    }
}

void TestWindow::keyReleaseEvent(QKeyEvent* e)
{
    if(e->key() != 16777220 && e->key() != 16777248){
        QString get = e->text();
        if(get[0].toUpper() == m_fileText[m_currentIndex].toUpper()){
            m_visibleText += m_fileText[m_currentIndex];
            ui->textView->setPlainText(m_visibleText);
            m_currentIndex++;

            if(m_firstMiss)
            {
                m_correct++;
            }

            ui->input->setPlainText(" ");
            m_firstMiss = true;

            if(m_currentIndex == m_fileText.length())
            {
                onFinish();
            }
            else //correct guess but not end of file
            {
                //autofill if \ is found
                while(m_fileText[m_currentIndex] == '\\')
                {
                    m_currentIndex++;
                    m_visibleText += m_fileText[m_currentIndex];
                    ui->textView->setPlainText(m_visibleText);
                    m_currentIndex++;
                }

                if(m_currentIndex == m_fileText.length())
                {
                    onFinish();
                }
            }
        }
        else
        {
            QString space = " ";

            if(m_firstMiss)
            {
                m_misses++;
                m_missedIndicies.append(m_currentIndex);
                m_firstMiss = false;
            }
            if(m_fileText[m_currentIndex] == space[0])
            {
                ui->input->setPlainText(" _");
            }
            else
            {
                ui->input->setPlainText(space[0] + m_fileText[m_currentIndex].toUpper());
            }
        }
    }


}




void TestWindow::timerEvent(QTimerEvent *event)
{
    m_time++;
}


void TestWindow::onStop()
{
    reject();
}

void TestWindow::reject()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Stop?",
                                     "Are you sure you want to stop the exercise early and return to the main menu?",
                                     QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        this->parentWidget()->show();
        delete this;
    }
}

void TestWindow::onFinish()
{
    killTimer(m_timer);

    int sec = m_time % 60;
    int min = m_time / 60;
    QString timeText;
    if(sec >= 10){
        timeText = QString::number(min) + ":" + QString::number(sec);
    }else{
        timeText = QString::number(min) + ":0" + QString::number(sec);
    }

    int total = m_correct + m_misses;
    float percentCorrect = roundf(((float)m_correct / total) * 10000) / 100;
    float percentMissed = roundf(((float)m_misses / total) * 10000) / 100;

    m_msg = QMessageBox::information(this, "Finished!",
                                     "Exercise Complete! \n\nCorrect: " + QString::number(m_correct) + '/'
                                        + QString::number(total) + " -- "
                                        + QString::number(percentCorrect)
                                     +" %\nMissed: " + QString::number(m_misses) + '/'
                                        + QString::number(total) + " -- "
                                        + QString::number(percentMissed)
                                     +" %\nTime: " + timeText,
                                     QMessageBox::Ok);

    QDateTime local(QDateTime::currentDateTime());
    //Store Stats in User file
    QFile file4( m_user + "_user.txt" );
    if ( file4.open(QIODevice::Append) )
    {
        QTextStream stream( &file4 );
        QString storeFile = m_fileNameBase;
        if(m_isOriginalFile)
        {
            storeFile.append(" - Total");
        }

        QString text = storeFile + "," + QString::number(m_correct) + "," + QString::number(m_misses) + "," + QString::number(total)
                       + "," + QString::number(percentCorrect) + "," + QString::number(percentMissed) + "," + timeText + "," + local.toString() + "|";
        stream << text;
        file4.close();
    }


    this->parentWidget()->show();




    delete this;
}
