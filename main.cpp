#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDebug>

class UdpClient : public QWidget {
    Q_OBJECT

public:
    UdpClient(QWidget *parent = nullptr);

private slots:
    void connectToServer();
    void processPendingDatagrams();

private:
    QLineEdit *ipLineEdit;
    QLabel *statusLabel;
    QLabel *startTimeLabel;
    QLabel *holdTimeLabel;
    QUdpSocket *udpSocket;
    QHostAddress serverAddress;
};

UdpClient::UdpClient(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *ipLayout = new QHBoxLayout();
    QLabel *ipLabel = new QLabel("Server IP:");
    ipLineEdit = new QLineEdit();
    ipLayout->addWidget(ipLabel);
    ipLayout->addWidget(ipLineEdit);
    mainLayout->addLayout(ipLayout);

    QPushButton *connectButton = new QPushButton("Connect");
    mainLayout->addWidget(connectButton);

    statusLabel = new QLabel("Enter server IP and click connect.");
    mainLayout->addWidget(statusLabel);

    startTimeLabel = new QLabel("Start Time: Not Received");
    holdTimeLabel = new QLabel("Hold Time: Not Received");
    mainLayout->addWidget(startTimeLabel);
    mainLayout->addWidget(holdTimeLabel);

    udpSocket = new QUdpSocket(this);

    connect(connectButton, &QPushButton::clicked, this, &UdpClient::connectToServer);
    connect(udpSocket, &QUdpSocket::readyRead, this, &UdpClient::processPendingDatagrams);
}

void UdpClient::connectToServer() {
    QString ipAddress = ipLineEdit->text();
    if (serverAddress.setAddress(ipAddress)) {
        udpSocket->bind(QHostAddress::Any, 12345);  // Bind to port 12345 to match the sender
        statusLabel->setText("Connected to server: " + ipAddress);
    } else {
        statusLabel->setText("Invalid IP address. Please try again.");
    }
}

void UdpClient::processPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << "Received datagram: " << datagram;

        // Split the datagram by newlines
        QList<QByteArray> receivedData = datagram.split('\n');

        // Update the UI based on the received data
        if (receivedData.size() >= 3) {
            QString startTime = QString(receivedData[0]).trimmed();
            QString holdTime = QString(receivedData[1]).trimmed();
            QString remainingTime = QString(receivedData[2]).trimmed();

            // Display the formatted data
            startTimeLabel->setText(startTime);
            holdTimeLabel->setText(holdTime);
            statusLabel->setText(remainingTime); // Assuming statusLabel to display remaining time
        } else {
            qDebug() << "Unexpected data format received.";
        }
    }
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    UdpClient client;
    client.show();

    return app.exec();
}

#include "main.moc"
