#ifndef OVERLAY_WINDOW_HPP
#define OVERLAY_WINDOW_HPP

#include <QtCore/QList>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QSize>
#include <QtGui/QColor>
#include <QtWidgets/QGraphicsOpacityEffect>
#include <QtWidgets/QWidget>

class OverlayComponent;

class OverlayWindow : public QWidget
{
    Q_OBJECT

    public:
        explicit OverlayWindow(QWidget* parentPtr = Q_NULLPTR);

    private:
        void configureWindowProperties();
        void initializeOverlayBorders();
        void initializeOverlayCorners();
        void initializeOpacityEffect();
        void initializeOpacityAnimation();
        QList<OverlayComponent*> components;
        QGraphicsOpacityEffect opacityEffect;
        QPropertyAnimation opacityAnimation;
        bool opacityAnimationEnabled;
        int opacityAnimationDuration;
        qreal overlayOpacity;

    public slots:
        void setScreenSize(QSize screenSize);
        void setBreathRate(int breathRate);
        void setOverlayOpacity(int opacity);
        void setOverlayThickness(int thickness);
        void setOverlayColor(const QColor& color);
        void toggleOpacityAnimation(bool enabled);

    private slots:
        void restartOpacityAnimation();
};

#endif // OVERLAY_WINDOW_HPP
