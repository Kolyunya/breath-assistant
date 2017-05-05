#ifndef OVERLAY_WINDOW_HPP
#define OVERLAY_WINDOW_HPP

#include <QtCore/QPropertyAnimation>
#include <QtCore/QRect>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsOpacityEffect>
#include <QtWidgets/QWidget>

class QPaintEvent;

class OverlayWindow : public QWidget
{
    Q_OBJECT

    public:
        explicit OverlayWindow(QWidget* parentPtr = Q_NULLPTR);

    protected:
        void paintEvent(QPaintEvent* paintEventPtr);

    private:
        void configureWindowProperties();
        void initializeOpacityEffect();
        void initializeOpacityAnimation();
        void configureBorderBrush(QPainter& painter, int position);
        void configureCornerBrush(QPainter& painter, int position);
        QRect getOverlayBorder(int position) const;
        QRect getOverlayCorner(int position) const;
        QGraphicsOpacityEffect opacityEffect;
        QPropertyAnimation opacityAnimation;
        bool opacityAnimationEnabled;
        int opacityAnimationDuration;
        qreal overlayOpacity;
        int overlayThickness;
        QColor overlayColor;

    public slots:
        void setBreathRate(int breathRate);
        void setOverlayOpacity(int opacity);
        void setOverlayThickness(int thickness);
        void setOverlayColor(QColor& color);
        void toggleOpacityAnimation(bool enabled);

    private slots:
        void restartOpacityAnimation();
};

#endif // OVERLAY_WINDOW_HPP
