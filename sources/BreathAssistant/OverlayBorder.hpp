#ifndef OVERLAY_BORDER_HPP
#define OVERLAY_BORDER_HPP

#include <BreathAssistant/OverlayComponent.hpp>
#include <QtCore/QRect>

class OverlayBorder : public OverlayComponent
{
    Q_OBJECT

    public:
        explicit OverlayBorder(QWidget* parentPtr = Q_NULLPTR);
        virtual ~OverlayBorder() noexcept = default;
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

#endif // OVERLAY_BORDER_HPP
