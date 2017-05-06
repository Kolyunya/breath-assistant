#ifndef OVERLAY_CORNER_HPP
#define OVERLAY_CORNER_HPP

#include <BreathAssistant/OverlayComponent.hpp>
#include <QtCore/QRect>

class OverlayCorner : public OverlayComponent
{
    Q_OBJECT

    public:
        explicit OverlayCorner(QWidget* parentPtr = Q_NULLPTR);
        virtual ~OverlayCorner() noexcept = default;
        void setLocation(int location);

    protected:
        virtual QSize getSize() const override;
        virtual QPoint getPosition() const override;
        virtual QPen getPen() const override;
        virtual QBrush getBrush() const override;
        virtual void draw(QPainter& painter) override;

    private:
        void initializeOverlayRectangle();
        int location;
        QRect overlayRectangle;
};

#endif // OVERLAY_CORNER_HPP
