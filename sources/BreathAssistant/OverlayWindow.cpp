#include <BreathAssistant/OverlayBorder.hpp>
#include <BreathAssistant/OverlayCorner.hpp>
#include <BreathAssistant/OverlayWindow.hpp>
#include <QtCore/QEasingCurve>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>

OverlayWindow::OverlayWindow(QWidget* parentPtr) :
    QWidget(parentPtr),
    opacityAnimationEnabled(false),
    opacityAnimationDuration(0),
    overlayOpacity(0)
{
    this->configureWindowProperties();
    this->initializeOverlayBorders();
    this->initializeOverlayCorners();
    this->initializeOpacityEffect();
    this->initializeOpacityAnimation();
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

void OverlayWindow::initializeOverlayBorders()
{
    for (int location = 1; location <= 4; location++) {
        OverlayBorder* overlayBorderPtr = new OverlayBorder(this);
        overlayBorderPtr->setScreenSize(this->size());
        overlayBorderPtr->setLocation(location);

        this->components.append(overlayBorderPtr);
    }
}

void OverlayWindow::initializeOverlayCorners()
{
    for (int location = 1; location <= 4; location++) {
        OverlayCorner* overlayCornerPtr = new OverlayCorner(this);
        overlayCornerPtr->setScreenSize(this->size());
        overlayCornerPtr->setLocation(location);

        this->components.append(overlayCornerPtr);
    }
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
    this->opacityAnimation.setEasingCurve(QEasingCurve::OutQuad);

    QObject::connect(
        &this->opacityAnimation,
        &QPropertyAnimation::finished,
        this,
        &OverlayWindow::restartOpacityAnimation
    );
}

void OverlayWindow::setScreenSize(QSize screenSize)
{
    this->setFixedSize(screenSize);

    for (OverlayComponent* overlayComponent: this->components) {
        overlayComponent->setScreenSize(screenSize);
    }
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
    for (OverlayComponent* overlayComponent: this->components) {
        overlayComponent->setOverlayThickness(thickness);
    }
}

void OverlayWindow::setOverlayColor(QColor& color)
{
    for (OverlayComponent* overlayComponent: this->components) {
        overlayComponent->setOverlayColor(color);
    }
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
