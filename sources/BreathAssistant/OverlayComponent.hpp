#ifndef OVERLAYCOMPONENT_H
#define OVERLAYCOMPONENT_H

#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QPen>
#include <QtWidgets/QWidget>

class QPaintEvent;

class OverlayComponent : public QWidget
{
    Q_OBJECT

    public:
        explicit OverlayComponent(QWidget* parentPtr = Q_NULLPTR);
        virtual ~OverlayComponent() noexcept = default;
        virtual QSize sizeHint() const override;
        void setScreenSize(const QSize& screenSize);
        void setOverlayThickness(int overlayThickness);
        void setOverlayColor(const QColor& overlayColor);

    protected:
        void initialize();
        void initializeSize();
        void initializePosition();
        virtual void paintEvent(QPaintEvent* paintEventPtr) override;
        virtual void onSreenSizeChange();
        virtual void onOverlayThicknessChange();
        virtual void onOverlayColorChange();
        virtual QSize getSize() const = 0;
        virtual QPoint getPosition() const = 0;
        virtual QPen getPen() const = 0;
        virtual QBrush getBrush() const = 0;
        virtual void draw(QPainter& painter) = 0;
        QSize screenSize;
        int overlayThickness;
        QColor overlayColor;
};

#endif // OVERLAYCOMPONENT_H
