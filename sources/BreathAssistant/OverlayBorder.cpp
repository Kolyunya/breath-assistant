#include <BreathAssistant/OverlayBorder.hpp>
#include <QtGui/QPainter>

OverlayBorder::OverlayBorder(QWidget* parentPtr) :
    OverlayComponent(parentPtr),
    location(0)
{

}

void OverlayBorder::setLocation(int location)
{
    this->location = location;
}

QSize OverlayBorder::getSize() const
{
    int width = this->overlayThickness;
    int height = this->overlayThickness;

    switch (this->location) {
        case 1:
        case 3:
            width = this->screenSize.width() - this->overlayThickness * 2;
            width = width >= 0 ? width : 0;

            height = ( height > this->screenSize.height() / 2 ) ? this->screenSize.height() / 2 : height;
            break;
        case 2:
        case 4:
            width = ( width > this->screenSize.width() / 2 ) ? this->screenSize.width() / 2 : width;

            height = this->screenSize.height() - this->overlayThickness * 2;
            height = height >= 0 ? height : 0;
            break;
    }

    QSize size(width, height);

    return size;
}

QPoint OverlayBorder::getPosition() const
{
    int x = 0;
    int y = 0;

    switch (this->location) {
        case 1:
            x = this->overlayThickness;
            x = x > this->screenSize.width() / 2 ? this->screenSize.width() / 2 : x;
            break;
        case 2:
            x = this->screenSize.width() - this->overlayThickness;
            x = x < this->screenSize.width() / 2 ? this->screenSize.width() / 2 : x;

            y = this->overlayThickness;
            y = y > this->screenSize.height() / 2 ? this->screenSize.height() / 2 : y;
            break;
        case 3:
            x = this->overlayThickness;
            x = x > this->screenSize.width() / 2 ? this->screenSize.width() / 2 : x;

            y = this->screenSize.height() - this->overlayThickness;
            y = y < this->screenSize.height() / 2 ? this->screenSize.height() / 2 : y;
            break;
        case 4:
            y = this->overlayThickness;
            y = y > this->screenSize.height() / 2 ? this->screenSize.height() / 2 : y;
            break;
    }

    QPoint position(x, y);

    return position;
}

QPen OverlayBorder::getPen() const
{
    QPen pen(Qt::NoPen);

    return pen;
}

QBrush OverlayBorder::getBrush() const
{
    QPoint gradientStart(0, 0);
    QPoint gradientStop(0, 0);

    switch (this->location) {
        case 1:
            gradientStop = QPoint(0, this->height());
            break;
        case 2:
            gradientStart = QPoint(this->width(), 0);
            break;
        case 3:
            gradientStart = QPoint(0, this->height());
            break;
        case 4:
            gradientStop = QPoint(this->width(), 0);
            break;
    }

    QLinearGradient gradient(gradientStart, gradientStop);

    QColor transparentColor(this->overlayColor);
    transparentColor.setAlpha(0);

    gradient.setColorAt(0, this->overlayColor);
    gradient.setColorAt(1, transparentColor);

    QBrush brush(gradient);

    return brush;
}

void OverlayBorder::draw(QPainter& painter)
{
    this->initializeOverlayRectangle();
    painter.drawRect(this->overlayRectangle);
}

void OverlayBorder::initializeOverlayRectangle()
{
    this->overlayRectangle.setSize(this->getSize());
}
