﻿#pragma execution_character_set("utf-8")

#include "gaugepanel.h"
#include "qpainter.h"
#include "qmath.h"
#include "qtimer.h"
#include "qlcdnumber.h"
#include "qdebug.h"

GaugePanel::GaugePanel(QWidget *parent) : QWidget(parent)
{
    minValue = 0;
    maxValue = 24;
    value = 0;
    precision = 0;
    scalePrecision = 0;

    scaleMajor = 5;
    scaleMinor = 10;
    startAngle = 45;
    endAngle = 45;

    animation = true;
    animationStep = 0.5;

    ringWidth = 10;
    ringColor = QColor(54, 192, 254);

    scaleColor = QColor(54, 192, 254);
    pointerColor = QColor(54, 192, 254);
    bgColor = Qt::transparent;
    textColor = QColor(222, 125, 44);
    unit = "";
    text = "";
    unitText = "V";

    reverse = false;
    currentValue = 0;
    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateValue()));
}

GaugePanel::~GaugePanel()
{
    if (timer->isActive()) {
        timer->stop();
    }
}

void GaugePanel::paintEvent(QPaintEvent *)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);

    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    //绘制背景
    if (bgColor != Qt::transparent) {
        painter.setPen(Qt::NoPen);
        painter.fillRect(this->rect(), bgColor);
    }

    painter.translate(width / 2, height / 2);
    painter.scale(side / 200.0, side / 200.0);

    //绘制圆环
    drawRing(&painter);

    //绘制刻度线
    drawScale(&painter);

    //绘制刻度值
    drawScaleNum(&painter);

    //绘制指示器
    drawPointer(&painter);

    //绘制当前值
    drawValue(&painter);
}

void GaugePanel::drawRing(QPainter *painter)
{
    int radius = 70;
    painter->save();

    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidthF(ringWidth);
    pen.setColor(ringColor);
    painter->setPen(pen);

    radius = radius - ringWidth;
    QRectF rect = QRectF(-radius, -radius, radius * 2, radius * 2);
    double angleAll = 360.0 - startAngle - endAngle;
    painter->drawArc(rect, (270 - startAngle - angleAll) * 16, angleAll * 16);

    painter->restore();
}

void GaugePanel::drawScale(QPainter *painter)
{
    int radius = 80;
    painter->save();

    painter->rotate(startAngle);
    int steps = (scaleMajor * scaleMinor);
    double angleStep = (360.0 - startAngle - endAngle) / steps;

    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setColor(scaleColor);

    for (int i = 0; i <= steps; i++) {
        if (i % scaleMinor == 0) {
            pen.setWidthF(1.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 8, 0, radius + 5);
        } else {
            pen.setWidthF(0.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 8, 0, radius - 3);
        }

        painter->rotate(angleStep);
    }

    painter->restore();
}

void GaugePanel::drawScaleNum(QPainter *painter)
{
    int radius = 95;
    painter->save();
    painter->setPen(scaleColor);

    QFont fo = this->font();
    fo.setPixelSize(12);
    fo.setBold(true);
    painter->setFont(fo);

    double startRad = (360 - startAngle - 90) * (M_PI / 180);
    double deltaRad = (360 - startAngle - endAngle) * (M_PI / 180) / scaleMajor;

    for (int i = 0; i <= scaleMajor; i++) {
        double sina = qSin(startRad - i * deltaRad);
        double cosa = qCos(startRad - i * deltaRad);
        double value = 1.0 * i * ((maxValue - minValue) / scaleMajor) + minValue;

        QString strValue = QString("%1").arg((double)value, 0, 'f', scalePrecision);
#if (QT_VERSION >= QT_VERSION_CHECK(5,11,0))
        double textWidth = fontMetrics().horizontalAdvance(strValue);
#else
        double textWidth = fontMetrics().width(strValue);
#endif
        double textHeight = fontMetrics().height();
        int x = radius * cosa - textWidth / 2;
        int y = -radius * sina + textHeight / 4;
        painter->drawText(x, y, strValue);
    }

    painter->restore();
}

void GaugePanel::drawPointer(QPainter *painter)
{
    int radius = 70;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);

    QPolygon pts;
    pts.setPoints(4, -5, 0, 0, -8, 5, 0, 0, radius);

    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);

    painter->restore();
}

void GaugePanel::drawValue(QPainter *painter)
{
    int radius = 100;
    painter->save();
    painter->setPen(textColor);

    QFont font;
    font.setPixelSize(20);
    font.setBold(true);
    font.setFamily("SimHei");
    painter->setFont(font);

    QString strValue = QString("%1").arg((double)currentValue*100, 0, 'f', precision);
    strValue = QString("%1").arg(strValue);
    QRectF valueRect(-radius, radius / 5, radius * 2, radius / 2.5);
    painter->drawText(valueRect, Qt::AlignCenter, "");
    painter->setPen(scaleColor);
    QRectF textRect(-radius, radius / 2.5, radius * 2, radius / 2.5);
    font.setPixelSize(16);
    painter->setFont(font);
    painter->drawText(textRect, Qt::AlignCenter, "");

    painter->setPen(scaleColor);
    font.setPixelSize(24);
    painter->setFont(font);
    QRectF textRectT(-radius, -radius / 2.5, radius * 2, radius / 2.5);
    painter->drawText(textRectT, Qt::AlignCenter, unitText);

    painter->restore();
}

void GaugePanel::updateValue()
{
    if (!reverse) {
        if (currentValue >= value) {
            currentValue = value;
            timer->stop();
        } else {
            currentValue += animationStep;
        }
    } else {
        if (currentValue <= value) {
            currentValue = value;
            timer->stop();
        } else {
            currentValue -= animationStep;
        }
    }

    this->update();
}

void GaugePanel::setUnitText(const QString &value)
{
    unitText = value;
}

double GaugePanel::getMinValue() const
{
    return this->minValue;
}

double GaugePanel::getMaxValue() const
{
    return this->maxValue;
}

double GaugePanel::getValue() const
{
    return this->value;
}

int GaugePanel::getPrecision() const
{
    return this->precision;
}

int GaugePanel::getScalePrecision() const
{
    return this->scalePrecision;
}

int GaugePanel::getScaleMajor() const
{
    return this->scaleMajor;
}

int GaugePanel::getScaleMinor() const
{
    return this->scaleMinor;
}

int GaugePanel::getStartAngle() const
{
    return this->startAngle;
}

int GaugePanel::getEndAngle() const
{
    return this->endAngle;
}

bool GaugePanel::getAnimation() const
{
    return this->animation;
}

double GaugePanel::getAnimationStep() const
{
    return this->animationStep;
}

int GaugePanel::getRingWidth() const
{
    return this->ringWidth;
}

QColor GaugePanel::getRingColor() const
{
    return this->ringColor;
}

QColor GaugePanel::getScaleColor() const
{
    return this->scaleColor;
}

QColor GaugePanel::getPointerColor() const
{
    return this->pointerColor;
}

QColor GaugePanel::getBgColor() const
{
    return this->bgColor;
}

QColor GaugePanel::getTextColor() const
{
    return this->textColor;
}

QString GaugePanel::getUnit() const
{
    return this->unit;
}

QString GaugePanel::getText() const
{
    return this->text;
}

QSize GaugePanel::sizeHint() const
{
    return QSize(200, 200);
}

QSize GaugePanel::minimumSizeHint() const
{
    return QSize(50, 50);
}

void GaugePanel::setRange(double minValue, double maxValue)
{
    //如果最小值大于或者等于最大值则不设置
    if (minValue >= maxValue) {
        return;
    }

    this->minValue = minValue;
    this->maxValue = maxValue;

    //如果目标值不在范围值内,则重新设置目标值
    //值小于最小值则取最小值,大于最大值则取最大值
    if (value < minValue) {
        setValue(minValue);
    } else if (value > maxValue) {
        setValue(maxValue);
    }

    this->update();
}

void GaugePanel::setRange(int minValue, int maxValue)
{
    setRange((double)minValue, (double)maxValue);
}

void GaugePanel::setMinValue(double minValue)
{
    setRange(minValue, maxValue);
}

void GaugePanel::setMaxValue(double maxValue)
{
    setRange(minValue, maxValue);
}

void GaugePanel::setValue(double value)
{
    //值和当前值一致则无需处理
    if (value == this->value) {
        return;
    }

    //值小于最小值则取最小值,大于最大值则取最大值
    if (value < minValue) {
        value = minValue;
    } else if (value > maxValue) {
        value = maxValue;
    }


    if (value > this->value) {
        reverse = false;
    } else if (value < this->value) {
        reverse = true;
    }

    this->value = value;
    emit valueChanged(value);

    if (!animation) {
        currentValue = this->value;
        this->update();
    } else {
        timer->start();
    }
}

void GaugePanel::setValue(int value)
{
    setValue((double)(value/100));
}

void GaugePanel::setPrecision(int precision)
{
    //最大精确度为 3
    if (precision <= 3 && this->precision != precision) {
        this->precision = precision;
        this->update();
    }
}

void GaugePanel::setScalePrecision(int scalePrecision)
{
    //最大精确度为 2
    if (scalePrecision <= 2 && this->scalePrecision != scalePrecision) {
        this->scalePrecision = scalePrecision;
        this->update();
    }
}

void GaugePanel::setScaleMajor(int scaleMajor)
{
    if (scaleMajor > 0 && this->scaleMajor != scaleMajor) {
        this->scaleMajor = scaleMajor;
        this->update();
    }
}

void GaugePanel::setScaleMinor(int scaleMinor)
{
    if (scaleMinor > 0 && this->scaleMinor != scaleMinor) {
        this->scaleMinor = scaleMinor;
        this->update();
    }
}

void GaugePanel::setStartAngle(int startAngle)
{
    if (this->startAngle != startAngle) {
        this->startAngle = startAngle;
        this->update();
    }
}

void GaugePanel::setEndAngle(int endAngle)
{
    if (this->endAngle != endAngle) {
        this->endAngle = endAngle;
        this->update();
    }
}

void GaugePanel::setAnimation(bool animation)
{
    if (this->animation != animation) {
        this->animation = animation;
        this->update();
    }
}

void GaugePanel::setAnimationStep(double animationStep)
{
    if (this->animationStep != animationStep) {
        this->animationStep = animationStep;
        this->update();
    }
}

void GaugePanel::setRingWidth(int ringWidth)
{
    if (this->ringWidth != ringWidth) {
        this->ringWidth = ringWidth;
        this->update();
    }
}

void GaugePanel::setRingColor(const QColor &ringColor)
{
    if (this->ringColor != ringColor) {
        this->ringColor = ringColor;
        this->update();
    }
}

void GaugePanel::setScaleColor(const QColor &scaleColor)
{
    if (this->scaleColor != scaleColor) {
        this->scaleColor = scaleColor;
        this->update();
    }
}

void GaugePanel::setPointerColor(const QColor &pointerColor)
{
    if (this->pointerColor != pointerColor) {
        this->pointerColor = pointerColor;
        this->update();
    }
}

void GaugePanel::setBgColor(const QColor &bgColor)
{
    if (this->bgColor != bgColor) {
        this->bgColor = bgColor;
        this->update();
    }
}

void GaugePanel::setTextColor(const QColor &textColor)
{
    if (this->textColor != textColor) {
        this->textColor = textColor;
        this->update();
    }
}

void GaugePanel::setUnit(const QString &unit)
{
    if (this->unit != unit) {
        this->unit = unit;
        this->update();
    }
}

void GaugePanel::setText(const QString &text)
{
    if (this->text != text) {
        this->text = text;
        this->update();
    }
}
