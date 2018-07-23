#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets/QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setPalette( colorTheme( QColor( Qt::darkGray ).dark( 100 ) ) );
    QColor color = Qt::white;
    QPalette palette = ui->lbTemp->palette();
    palette.setColor(QPalette::WindowText, color);
    ui->lbTemp->setPalette(palette);
    ui->statusBar->setPalette(palette);
    m_client = new QMqttClient(this);
    m_client->setHostname("eu.thethings.network");
    m_client->setPort(1883);
    m_client->setUsername("gestion-clim-lab-c12");
    m_client->setPassword("ttn-account-v2.GzYy6dxbgfWGUqkef_7JZ6YQjjlNLdCejB4bGS58BZM");

    connect(m_client, &QMqttClient::disconnected, this, &MainWindow::brokerDisconnected);
    connect(m_client, &QMqttClient::stateChanged, this, &MainWindow::stateChange);
    connect(m_client, &QMqttClient::messageReceived, this, &MainWindow::receivedMessage);

    myThermo = new QwtThermo(this);
    myThermo->setScale(0,50);
    myThermo->setScaleMaxMinor(10);
    myThermo->setFixedHeight(280);
    myThermo->setPipeWidth(20);
    myThermo->setFillBrush(QBrush(QColor("red")));

    ui->tempLayout_3->addWidget(myThermo);

    humGauge = new myGauge(this,"%");
    humGauge->setScaleStepSize( 10.0 );
    humGauge->setScale( 0.0, 100.0 );

    ui->humLayout->addWidget(humGauge);

}

MainWindow::~MainWindow()
{
    delete ui;
}

QPalette MainWindow::colorTheme( const QColor &base ) const
{
    QPalette palette;
    palette.setColor( QPalette::Base, base );
    palette.setColor( QPalette::Window, base.dark( 150 ) );
    palette.setColor( QPalette::Mid, base.dark( 110 ) );
    palette.setColor( QPalette::Light, base.light( 170 ) );
    palette.setColor( QPalette::Dark, base.dark( 170 ) );
    palette.setColor( QPalette::Text, base.dark( 200 ).light( 800 ) );
    palette.setColor( QPalette::WindowText, base.dark( 200 ) );

    return palette;
}


void MainWindow::on_pbConnexion_clicked()
{
    if (m_client->state() == QMqttClient::Disconnected) {
            ui->pbConnexion->setText(tr("Deconnexion"));
            m_client->connectToHost();

        } else {
            ui->pbConnexion->setText(tr("Connexion"));
            m_client->disconnectFromHost();
    }
}

void MainWindow::brokerDisconnected()
{
     QMessageBox::critical(this, "Erreur", "Connexion avec le broker interrompue");
}

void MainWindow::stateChange()
{
    QString message;
    switch (m_client->state())
    {
        case 0 :message = "Déconnecté";break;
        case 1 :message = "En cours de connexion";break;
        case 2 :message = "Connecté";
            QString myTopic1 = "gestion-clim-lab-c12/devices/temp-hum-sensor-1/up/temperature_1";
            auto subscription = m_client->subscribe(myTopic1);
            if (!subscription) {
                QMessageBox::critical(this, "Erreur", "Impossible de souscrire au topic\n"+myTopic1);
                return;
            }
            QString myTopic2 = "gestion-clim-lab-c12/devices/temp-hum-sensor-1/up/relative_humidity_2";
            subscription = m_client->subscribe(myTopic2);
            if (!subscription) {
                QMessageBox::critical(this, "Erreur", "Impossible de souscrire au topic\n"+myTopic2);
                return;
            }
            break;
    }

    ui->statusBar->showMessage(message);
}

void MainWindow::receivedMessage(const QByteArray &message, const QMqttTopicName &topic)
{
    ui->statusBar->showMessage("Data",1000);
    QString unit;
    if(topic.name()=="gestion-clim-lab-c12/devices/temp-hum-sensor-1/up/relative_humidity_2")
    {
        unit = " %";
        humGauge->setValue(message.toDouble());
    }
    if(topic.name()=="gestion-clim-lab-c12/devices/temp-hum-sensor-1/up/temperature_1")
    {
        unit = " °C";
        myThermo->setValue(message.toDouble());
        ui->lbTemp->setText(message+unit);
    }
}
