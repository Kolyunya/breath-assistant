#include <BreathAssistant/OverlayComponent.hpp>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

OverlayComponent::OverlayComponent(QWidget* parentPtr) :
    QWidget(parentPtr),
    overlayThickness(0)
{

}

QSize OverlayComponent::sizeHint() const
{
    QSize sizeHint = this->size();

    return sizeHint;
}

void OverlayComponent::setScreenSize(const QSize& screenSize)
{
    this->screenSize = screenSize;

    this->onSreenSizeChange();
}

void OverlayComponent::setOverlayThickness(int overlayThickness)
{
    this->overlayThickness = overlayThickness;

    this->onOverlayThicknessChange();
}

void OverlayComponent::setOverlayColor(const QColor& overlayColor)
{
    this->overlayColor = overlayColor;

    this->onOverlayColorChange();
}

void OverlayComponent::paintEvent(QPaintEvent* paintEventPtr)
{
    QPainter painter(this);

    QPen pen = this->getPen();
    painter.setPen(pen);

    QBrush brush = this->getBrush();
    painter.setBrush(brush);

    this->draw(painter);

    paintEventPtr->accept();
}

void OverlayComponent::onSreenSizeChange()
{
    // Override in child classes if needed.

    this->initialize();
}

void OverlayComponent::onOverlayThicknessChange()
{
    // Override in child classes if needed.

    this->initialize();
}

void OverlayComponent::onOverlayColorChange()
{
    // Override in child classes if needed.

    this->initialize();
}

void OverlayComponent::initialize()
{
    this->initializeSize();
    this->initializePosition();
}

void OverlayComponent::initializeSize()
{
    QSize size = this->getSize();
    this->setFixedSize(size);
}

void OverlayComponent::initializePosition()
{
    QPoint position = this->getPosition();
    this->move(position);
}
