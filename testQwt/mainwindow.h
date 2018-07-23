#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include<qwt_thermo.h>
#include <QMqttClient>

#include "mygauge.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_pbConnexion_clicked();
    void brokerDisconnected();
    void stateChange();
    void receivedMessage(const QByteArray &message, const QMqttTopicName &topic);

private :
    QPalette colorTheme(const QColor &base) const;

private:
    Ui::MainWindow *ui;
    myGauge *tempGauge;
    myGauge *humGauge;
    QwtThermo *myThermo;
    QMqttClient *m_client;
};

#endif // MAINWINDOW_H
