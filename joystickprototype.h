#ifndef JOYSTICKWIDGET_H
#define JOYSTICKWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QScrollBar>


namespace Ui {
class JoystickWidget;
class JoystickPrototype;
class RotationWidget;
class KeyReciever;
}

class JoystickWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    JoystickWidget(QWidget *parent);
    bool m_MousePressed, m_MouseUp, m_MouseDown, m_MouseLeft, m_MouseRight;
    void setCenter();
    int xCenter, yCenter;

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    int xCoordinate, yCoordinate;
    int xVector, yVector;
    int xCorner, yCorner;
    int radius;
};

class RotationWidget : public QScrollBar
{
    Q_OBJECT

public:
    RotationWidget(QWidget *parent);
    bool m_MousePressed, m_MouseLeft, m_MouseRight;
    void addValue(int amount);
    void resetValue();

protected:
    void mouseReleaseEvent(QMouseEvent * event);
    int value;
};

class JoystickPrototype : public QWidget
{
    Q_OBJECT

public:
    explicit JoystickPrototype(QWidget *parent = 0);
    ~JoystickPrototype();
    JoystickWidget *joystickWidget;
    RotationWidget *rotationWidget;

protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void keyReleaseEvent(QKeyEvent * event);

private:
    Ui::JoystickPrototype *ui;
};

#endif // JOYSTICKWIDGET_H
