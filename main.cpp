#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QTimer>
#include <QUdpSocket>
#include <QDateTime>
#include <QPalette>
#include <QPixmap>

class CountdownTimer : public QWidget {
    Q_OBJECT

public:
    CountdownTimer(QWidget *parent = nullptr);
protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void startTimer();
    void stopTimer();
    void holdTimer();
    void resetTimer();
    void updateDisplay();

private:
    QSpinBox *hourSpinBox;
    QSpinBox *minuteSpinBox;
    QSpinBox *secondSpinBox;
    QLabel *displayLabel;
    QTimer *timer;
    QLabel *hourlabel;
    QLabel *minlabel;
    QLabel *seclabel;
    int remainingTime;
    bool timerStopped;
    QDateTime startTime;
    QUdpSocket *udpSocket;
    void sendTimeToClient();
};

CountdownTimer::CountdownTimer(QWidget *parent)
    : QWidget(parent), remainingTime(0), timerStopped(false) {


    QPixmap bkgnd(":/rock.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    this->setAutoFillBackground(true);
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);


    QHBoxLayout *timeLayout = new QHBoxLayout();
    hourlabel = new QLabel("Enter hour:");
    hourSpinBox = new QSpinBox();
    hourSpinBox->setRange(0, 23);
    hourSpinBox->setMinimumHeight(30);
    hourSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    minlabel = new QLabel("Enter minutes:");
    minuteSpinBox = new QSpinBox();
    minuteSpinBox->setRange(0, 59);
    minuteSpinBox->setMinimumHeight(30);
    minuteSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    seclabel = new QLabel("Enter seconds:");
    secondSpinBox = new QSpinBox();
    secondSpinBox->setRange(0, 59);
    secondSpinBox->setMinimumHeight(30);
    secondSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    timeLayout->setAlignment(Qt::AlignCenter);
    timeLayout->setSpacing(10);
    timeLayout->addWidget(hourlabel);
    timeLayout->addWidget(hourSpinBox);
    timeLayout->addWidget(minlabel);
    timeLayout->addWidget(minuteSpinBox);
    timeLayout->addWidget(seclabel);
    timeLayout->addWidget(secondSpinBox);
    mainLayout->addLayout(timeLayout);


    displayLabel = new QLabel("00:00:00");
    displayLabel->setAlignment(Qt::AlignCenter);
    displayLabel->setStyleSheet("font-size: 44px; font-weight: bold; color: #FFFFFF;");
    mainLayout->addWidget(displayLabel);


    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *startButton = new QPushButton("Start");
    QPushButton *stopButton = new QPushButton("Stop");
    QPushButton *holdButton = new QPushButton("Hold");
    QPushButton *resetButton = new QPushButton("Reset");


    startButton->setStyleSheet("background-color: #90EE90; font-weight: bold;");
    stopButton->setStyleSheet("background-color: #FF6347; font-weight: bold;");
    holdButton->setStyleSheet("background-color: #FFD700; font-weight: bold;");
    resetButton->setStyleSheet("background-color: #87CEEB; font-weight: bold;");

    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setSpacing(10);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(holdButton);
    buttonLayout->addWidget(resetButton);
    mainLayout->addLayout(buttonLayout);


    timer = new QTimer(this);
    udpSocket = new QUdpSocket(this);

    connect(startButton, &QPushButton::clicked, this, &CountdownTimer::startTimer);
    connect(stopButton, &QPushButton::clicked, this, &CountdownTimer::stopTimer);
    connect(holdButton, &QPushButton::clicked, this, &CountdownTimer::holdTimer);
    connect(resetButton, &QPushButton::clicked, this, &CountdownTimer::resetTimer);
    connect(timer, &QTimer::timeout, this, &CountdownTimer::updateDisplay);
}
void CountdownTimer::resizeEvent(QResizeEvent *event) {
    QPixmap bkgnd(":/rock.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    QWidget::resizeEvent(event);
}


void CountdownTimer::startTimer() {
    if (!timerStopped) {
        int hours = hourSpinBox->value();
        int minutes = minuteSpinBox->value();
        int seconds = secondSpinBox->value();
        remainingTime = hours * 3600 + minutes * 60 + seconds;


        startTime = QDateTime(QDate::currentDate(), QTime(hours, minutes, seconds));
    } else {
        timerStopped = false;
    }
    timer->start(1000);
}




void CountdownTimer::stopTimer() {
    timer->stop();
    timerStopped = true;
}

void CountdownTimer::holdTimer() {
    timer->stop();
    sendTimeToClient();
}

void CountdownTimer::resetTimer() {
    timer->stop();
    remainingTime = 0;
    displayLabel->setText("00:00:00");
    timerStopped = false;
}


void CountdownTimer::updateDisplay() {
    if (remainingTime > 0) {
        remainingTime--;
        QTime time(0, 0);
        time = time.addSecs(remainingTime);
        displayLabel->setText(time.toString("HH:mm:ss"));
    } else {
        timer->stop();
        displayLabel->setText("00:00:00");
        sendTimeToClient();
    }
}



void CountdownTimer::sendTimeToClient() {

    QString formattedStartTime = startTime.time().toString("HH:mm:ss");


    QTime holdTime(0, 0);
    holdTime = holdTime.addSecs(remainingTime);
    QString formattedHoldTime = holdTime.toString("HH:mm:ss");


    QByteArray datagram;
    datagram.append("Start Time: " + formattedStartTime.toUtf8() + "\n");
    datagram.append("Hold Time: " + formattedHoldTime.toUtf8() + "\n");
    datagram.append("Remaining Time: " + QByteArray::number(remainingTime) + " seconds\n");


    udpSocket->writeDatagram(datagram, QHostAddress("192.168.0.129"), 12345);
    qDebug() << "Datagram sent!";
}




int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CountdownTimer timer;
    timer.show();

    return app.exec();
}

#include "main.moc"
