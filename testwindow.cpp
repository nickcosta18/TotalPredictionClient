#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent, QString fileName, QString user) :
    QDialog(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Total Prediction");


    m_user = user;
    m_isCalibrateMode = (user == "");
    m_isOriginalFile = (fileName.right(13) == "_original.txt");

    if(m_isCalibrateMode)
    {
        m_fileNameBase = fileName.left(fileName.length() - 13);
        ui->userLabel->setText("");
    }
    else
    {
        if(m_isOriginalFile)
        {
            m_fileNameBase = fileName.left(fileName.length() - 13);
        }
        else
        {
            m_fileNameBase = fileName.left(fileName.length() - 15);
        }
        ui->userLabel->setText("User:  " + user);
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
    while(!m_isCalibrateMode && m_fileText[m_currentIndex] == '\\')
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
                //autofill if not calibrate mode and \ is found
                while(!m_isCalibrateMode && m_fileText[m_currentIndex] == '\\')
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

    if(m_isCalibrateMode)
    {

        m_msg = QMessageBox::information(this, "Finished!",
                                         "Exercise Complete!",
                                         QMessageBox::Ok);

        this->parentWidget()->show();

        int numCalibrations;
        //read in previously missed indicies from file
        QFile file(m_fileNameBase + "_missed.txt");
        if (file.open(QFile::ReadOnly | QFile::Text)) {

            QTextStream in(&file);
            QString missText = in.readAll();
            QStringList missTextList = missText.split(",");
            int num;
            for(int i = 0; i < missTextList.length() - 1; i++)
            {
                num = missTextList[i].toInt();
                if(!m_missedIndicies.contains(num))
                {
                    m_missedIndicies.append(num);
                }
            }
            numCalibrations = missTextList[missTextList.length() - 1].toInt();
            file.close();
        }



        //update _missed.txt file
        QFile file2( m_fileNameBase + "_missed.txt" );
        if ( file2.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream stream( &file2 );
            QString text;
            for(int i = 0; i < m_missedIndicies.length(); i++)
            {
                text.append(QString::number(m_missedIndicies[i]));
                text.append(',');
            }
            numCalibrations++;
            text.append(QString::number(numCalibrations));

            stream << text;
            file2.close();
        }


        //Generate _calibrated.txt using _missed.txt
        QFile file3( m_fileNameBase + "_calibrated.txt" );
        if ( file3.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream stream( &file3 );
            QString text;
            for(int i = 0; i < m_fileText.length(); i++)
            {
                if(m_missedIndicies.contains(i))
                {
                    text.append('\\');
                }
                text.append(m_fileText[i]);
            }
            stream << text;
            file3.close();
        }

        this->hide();
        connect(this, SIGNAL(send_calibrated(QString,int)), this->parent(), SLOT(on_recv_calibrated(QString, int)));
        emit send_calibrated(m_fileNameBase, numCalibrations);

    }
    else
    {


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

    }



    delete this;
}
