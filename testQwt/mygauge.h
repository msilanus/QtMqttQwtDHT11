#ifndef MYGAUGE_H
#define MYGAUGE_H

#include <qwt_dial.h>
#include <qwt_dial_needle.h>
#include <qpainter.h>
#include <qwt_round_scale_draw.h>


class myGauge : public QwtDial
{
    Q_OBJECT
public:
    explicit myGauge(QWidget  *parent = nullptr, QString unit="");

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
private:
    QString m_unit;
};

#endif // MYGAUGE_H
