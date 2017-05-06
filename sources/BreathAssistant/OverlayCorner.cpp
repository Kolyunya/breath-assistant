#include <BreathAssistant/OverlayCorner.hpp>
#include <QtGui/QPainter>

OverlayCorner::OverlayCorner(QWidget* parentPtr) :
    OverlayComponent(parentPtr),
    location(0)
{

}

void OverlayCorner::setLocation(int location)
{
    this->location = location;
}

QSize OverlayCorner::getSize() const
{
    int width = this->overlayThickness;
    int height = this->overlayThickness;

    QSize size(
        width > this->screenSize.width() / 2 ? this->screenSize.width() / 2 : width,
        height > this->screenSize.height() / 2 ? this->screenSize.height() / 2 : height
    );

    return size;
}

QPoint OverlayCorner::getPosition() const
{
    int x = 0;
    int y = 0;

    switch (this->location) {
        case 1:
            x = this->screenSize.width() - this->overlayThickness;
            x = x < this->screenSize.width() / 2 ? this->screenSize.width() / 2 : x;
            break;
        case 2:
            x = this->screenSize.width() - this->overlayThickness;
            x = x < this->screenSize.width() / 2 ? this->screenSize.width() / 2 : x;

            y = this->screenSize.height() - this->overlayThickness;
            y = y < this->screenSize.height() / 2 ? this->screenSize.height() / 2 : y;
            break;
        case 3:
            y = this->screenSize.height() - this->overlayThickness;
            y = y < this->screenSize.height() / 2 ? this->screenSize.height() / 2 : y;
            break;
        case 4:
            break;
    }

    QPoint position(x, y);

    return position;
}

QPen OverlayCorner::getPen() const
{
    QPen pen(Qt::NoPen);

    return pen;
}

QBrush OverlayCorner::getBrush() const
{
    int gradientCenterX = 0;
    int gradientCenterY = 0;

    switch (this->location) {
        case 1:
            gradientCenterY = this->height();
            break;
        case 2:
            break;
        case 3:
            gradientCenterX = this->width();
            break;
        case 4:
            gradientCenterX = this->width();
            gradientCenterY = this->height();
            break;
    }

    QPoint gradientCenter(gradientCenterX, gradientCenterY);
    int gradientRadius = qMin(this->width(), this->height());
    QRadialGradient gradient(gradientCenter, gradientRadius);

    QColor transparentColor(this->overlayColor);
    transparentColor.setAlpha(0);

    gradient.setColorAt(0, transparentColor);
    gradient.setColorAt(1, this->overlayColor);

    QBrush brush(gradient);

    return brush;
}

void OverlayCorner::draw(QPainter& painter)
{
    this->initializeOverlayRectangle();
    painter.drawRect(this->overlayRectangle);
}

void OverlayCorner::initializeOverlayRectangle()
{
    this->overlayRectangle.setSize(this->getSize());
}
