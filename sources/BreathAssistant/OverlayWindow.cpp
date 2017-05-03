#include <BreathAssistant/OverlayWindow.hpp>
#include <QtCore/QEasingCurve>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtCore/QSize>
#include <QtGui/QBrush>
#include <QtGui/QGradient>
#include <QtGui/QLinearGradient>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>

OverlayWindow::OverlayWindow(QWidget* parentPtr) :
    QWidget(parentPtr),
    opacityAnimationEnabled(false),
    opacityAnimationDuration(0),
    overlayOpacity(0),
    overlayThickness(0)
{
    this->configureWindowProperties();
    this->initializeOpacityEffect();
    this->initializeOpacityAnimation();
}


void OverlayWindow::paintEvent(QPaintEvent* paintEventPtr)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::NoPen));

    for (int position = 1; position <= 4; position++) {
        this->configureOverlayBrush(painter, position);
        painter.drawRect(this->getOverlayRectangle(position));
    }

    paintEventPtr->accept();
}

void OverlayWindow::configureWindowProperties()
{
    this->setWindowFlags(
        Qt::FramelessWindowHint |
        Qt::ToolTip |
        Qt::WindowStaysOnTopHint |
        Qt::WindowTransparentForInput
    );

    this->setAttribute(Qt::WA_NoSystemBackground, true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

void OverlayWindow::initializeOpacityEffect()
{
    this->opacityEffect.setOpacity(0);
    this->setGraphicsEffect(&this->opacityEffect);
}

void OverlayWindow::initializeOpacityAnimation()
{
    this->opacityAnimation.setTargetObject(&this->opacityEffect);
    this->opacityAnimation.setPropertyName("opacity");
    this->opacityAnimation.setEasingCurve(QEasingCurve::OutCubic);

    QObject::connect(
        &this->opacityAnimation,
        &QPropertyAnimation::finished,
        this,
        &OverlayWindow::restartOpacityAnimation
    );
}

void OverlayWindow::configureOverlayBrush(QPainter& painter, int position)
{
    QPoint gradientStartPoint;
    QPoint gradientStopPoint;

    switch (position) {
        case 1:
            gradientStartPoint = QPoint(0, 0);
            gradientStopPoint = QPoint(0, this->overlayThickness);
            break;
        case 2:
            gradientStartPoint = QPoint(this->width(), 0);
            gradientStopPoint = QPoint(this->width() - this->overlayThickness, 0);
            break;
        case 3:
            gradientStartPoint = QPoint(0, this->height());
            gradientStopPoint = QPoint(0, this->height() - this->overlayThickness);
            break;
        case 4:
            gradientStartPoint = QPoint(0, 0);
            gradientStopPoint = QPoint(this->overlayThickness, 0);
            break;
    }

    QLinearGradient gradient(gradientStartPoint, gradientStopPoint);

    QColor transparentColor(this->overlayColor);
    transparentColor.setAlpha(0);

    gradient.setColorAt(0, this->overlayColor);
    gradient.setColorAt(1, transparentColor);

    QBrush brush(gradient);
    painter.setBrush(brush);
}

QRect OverlayWindow::getOverlayRectangle(int position) const
{
    QRect rectangle;

    switch (position) {
        case 1:
            rectangle = QRect(
                QPoint(0, 0),
                QSize(this->width(), this->overlayThickness)
            );
            break;
        case 2:
            rectangle = QRect(
                QPoint(this->width() - this->overlayThickness, 0),
                QSize(this->overlayThickness, this->height())
            );
            break;
        case 3:
            rectangle = QRect(
                QPoint(0, this->height() - this->overlayThickness),
                QSize(this->width(), this->overlayThickness)
            );
            break;
        case 4:
            rectangle = QRect(
                QPoint(0, 0),
                QSize(this->overlayThickness, this->height())
            );
            break;
    }

    return rectangle;
}

void OverlayWindow::setBreathRate(int breathRate)
{
    // 1000 milliseconds * 60 seconds / 2 times / breath rate
    this->opacityAnimationDuration = 1000 * 60 / 2 / breathRate;
}

void OverlayWindow::setOverlayOpacity(int opacity)
{
    this->overlayOpacity = qreal(opacity) / 100;
}

void OverlayWindow::setOverlayThickness(int thickness)
{
    this->overlayThickness = thickness;
}

void OverlayWindow::setOverlayColor(QColor& color)
{
    this->overlayColor = color;
}

void OverlayWindow::toggleOpacityAnimation(bool enabled)
{
    bool opacityAnimationIsRunning = this->opacityAnimationEnabled;
    this->opacityAnimationEnabled = enabled;

    if (opacityAnimationIsRunning == false) {
        this->restartOpacityAnimation();
    }
}

void OverlayWindow::restartOpacityAnimation()
{
    qreal opacity = this->opacityEffect.opacity();

    if (opacity == 0) {
        if (this->opacityAnimationEnabled == false) {
            return;
        }

        this->opacityAnimation.setEndValue(this->overlayOpacity);
    } else {
        this->opacityAnimation.setEndValue(0);
    }

    this->opacityAnimation.setDuration(this->opacityAnimationDuration);
    this->opacityAnimation.start();
}
