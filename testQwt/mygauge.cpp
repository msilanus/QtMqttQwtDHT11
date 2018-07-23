#include <QPainter>
#include "mygauge.h"

myGauge::myGauge(QWidget  *parent, QString unit) : QwtDial(parent)
{
    m_unit = unit;
    this->setLineWidth(4);
    this->setWrapping(false);
    QwtDialSimpleNeedle *nd = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow, Qt::white, Qt::red);
    this->setNeedle(nd);

    QwtRoundScaleDraw *scaleDraw = new QwtRoundScaleDraw();
    scaleDraw->setSpacing( 8 );
    scaleDraw->enableComponent( QwtAbstractScaleDraw::Backbone, false );
    scaleDraw->setTickLength( QwtScaleDiv::MinorTick, 4 );
    scaleDraw->setTickLength( QwtScaleDiv::MediumTick, 4 );
    scaleDraw->setTickLength( QwtScaleDiv::MajorTick, 10 );

    this->scaleDraw()->setPenWidth( 2 );
    this->setScaleDraw( scaleDraw );

    this->setWrapping( false );
    this->setReadOnly( true );

    this->setOrigin( 135.0 );
    this->setScaleArc( 0.0, 270.0 );
}

void myGauge::paintEvent(QPaintEvent *event)
{

    QwtDial::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::white);
    QFont font;
    font.setPointSize(11);
    painter.setFont(font);
    QString text = QString("%1 ").arg(value())+m_unit;
    auto c = rect().center();
    auto Size = painter.fontMetrics().size(Qt::TextSingleLine, text);
    painter.drawText(QPointF(c.x() -Size.width()/2, c.y()+4*Size.height()), text);

}
