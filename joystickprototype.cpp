#include "joystickprototype.h"
#include "ui_joystickprototype.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QtMath>
#include <QString>

JoystickPrototype::JoystickPrototype(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JoystickPrototype)
{
    ui->setupUi(this);

    joystickWidget = new JoystickWidget(this);
    ui->gridLayout->addWidget(joystickWidget);

    rotationWidget = new RotationWidget(this);
    ui->verticalLayout->addWidget(rotationWidget);
    grabKeyboard();
}

JoystickPrototype::~JoystickPrototype()
{
    delete ui;
}

void JoystickPrototype::keyPressEvent(QKeyEvent * event)
{
    if (event->type()==QEvent::KeyPress)
    {
        qDebug() << "key pressed " + event->text();
    }

    QString up = "Qt::Key_Up";

    switch(event->key())
    {
        case Qt::Key_Up:
            joystickWidget->yCenter -= 10;
            joystickWidget->m_MouseUp = true;
            break;

        case Qt::Key_Right:
            joystickWidget->xCenter += 10;
            joystickWidget->m_MouseRight = true;
            break;

        case Qt::Key_Left:
            joystickWidget->xCenter -= 10;
            joystickWidget->m_MouseLeft = true;
            break;

        case Qt::Key_Down:
            joystickWidget->yCenter += 10;
            joystickWidget->m_MouseDown = true;
            break;
        case Qt::Key_Z:
            rotationWidget->m_MouseLeft = true;
            rotationWidget->addValue(-5);
            break;

        case Qt::Key_X:
            rotationWidget->m_MouseRight = true;
            rotationWidget->addValue(5);
            break;
    }
    joystickWidget->update();
}

void JoystickPrototype::keyReleaseEvent(QKeyEvent * event)
{
    if(!event->isAutoRepeat())
    {
        if (event->type()==QEvent::KeyRelease)
        {
            qDebug() << "key released " + event->text();
        }

        switch(event->key())
        {
            case Qt::Key_Up:
                joystickWidget->m_MouseUp = false;
                break;

            case Qt::Key_Right:
                joystickWidget->m_MouseRight = false;
                break;

            case Qt::Key_Left:
                joystickWidget->m_MouseLeft = false;
                break;

            case Qt::Key_Down:
                joystickWidget->m_MouseDown = false;
                break;
            case Qt::Key_Z:
                rotationWidget->m_MouseLeft = false;
                break;

            case Qt::Key_X:
                rotationWidget->m_MouseRight = false;
                break;
        }
        joystickWidget->update();

        rotationWidget->resetValue();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RotationWidget::RotationWidget(QWidget *parent)
    : QScrollBar(Qt::Horizontal, parent)
{
    resetValue();
}

void RotationWidget::resetValue()
{
    if(!m_MouseLeft && !m_MouseRight)
        value = maximum()/2;
    addValue(0);
}

void RotationWidget::addValue(int amount)
{
    value += amount;
    setValue(value);
}

void RotationWidget::mouseReleaseEvent(QMouseEvent * event)
{
    Q_UNUSED(event)
    resetValue();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
JoystickWidget::JoystickWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

void JoystickWidget::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event)
    m_MousePressed = true;
}

void JoystickWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    update();
    m_MousePressed = false;
}

void JoystickWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (event->type() == QEvent::MouseMove)
    {
        xCenter = event->pos().x();
        yCenter = event->pos().y();
        update();
    }
}

void JoystickWidget::setCenter()
{
    xCenter = width()/2;
    yCenter = xCenter;
}

void JoystickWidget::paintEvent(QPaintEvent *event)
{
    if(!m_MousePressed && !m_MouseLeft && !m_MouseRight && !m_MouseUp && !m_MouseDown)
        setCenter();

    Q_UNUSED(event)

    radius = width()/2;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    //draw background circle white
    QPen outerPen(Qt::black, 2);
    painter.setPen(outerPen);
    QBrush outerBrush(Qt::white);
    painter.setBrush(outerBrush);
    painter.drawEllipse(0, 0, width(), width());

    //draw square lines for referential ease of use
    painter.drawLine(width()/2, 0, width()/2, width()/2 - 60);
    painter.drawLine(width()/2, width()/2 + 60, width()/2, width());
    painter.drawLine(0, width()/2, width()/2 - 60, width()/2);
    painter.drawLine(width()/2 + 60, width()/2, width(), width()/2);

    //draw frontground circle
    qreal centerC = width()/2;
    qreal num = yCenter - centerC;
    qreal den = xCenter - centerC;

    //check inner circle for collision with edges of larger circle
    if(qPow(qPow(den, 2) + qPow(num, 2), .5) > radius/2)
    {
        qreal theta = qAtan(num/den);

        qreal newY = qSin(theta) * (radius/2);
        qreal newX = qCos(theta) * (radius/2);

        if(den > 0)
        {
            xCenter = centerC + newX;
            yCenter = centerC + newY;
        }
        else
        {
            xCenter = centerC - newX;
            yCenter = centerC - newY;
        }

        if(theta > -1.58 && theta < -1.57)
            yCenter = centerC - radius/2;
        if(theta > 1.57 && theta < 1.58)
            yCenter = centerC + radius/2;
    }

    //using corner to draw instead of center point
    xCorner = xCenter - radius/2;
    yCorner = yCenter - radius/2;

    painter.drawEllipse(xCorner, yCorner, radius, radius);
}
