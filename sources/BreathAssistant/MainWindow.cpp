#include <BreathAssistant/MainWindow.hpp>
#include <BreathAssistant/OverlayWindow.hpp>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtGui/QColor>
#include <QtGui/QCursor>
#include <QtGui/QIcon>
#include <QtGui/QPalette>
#include <QtGui/QScreen>
#include <QtGui/QWindow>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSystemTrayIcon>
#include <ui_MainWindow.h>

MainWindow::MainWindow(QWidget* parentPtr) :
    QMainWindow(parentPtr),
    uiPtr(new Ui::MainWindow()),
    trayIconPtr(new QSystemTrayIcon(this)),
    overlayColorWindowPtr(Q_NULLPTR)
{
    this->configureUserInterface();
    this->configureWindowPosition();
    this->configureTrayIcon();
    this->initializeOverlayWindows();
    this->loadUserPreferences();
}

MainWindow::~MainWindow()
{
    delete this->uiPtr;
}

void MainWindow::configureUserInterface()
{
    this->uiPtr->setupUi(this);

    QObject::connect(
        this->uiPtr->breathRateSlider,
        &QSlider::valueChanged,
        this,
        &MainWindow::persistBreathRate
    );

    QObject::connect(
        this->uiPtr->overlayThicknessSlider,
        &QSlider::valueChanged,
        this,
        &MainWindow::persistOverlayThickness
    );

    QObject::connect(
        this->uiPtr->overlayOpacitySlider,
        &QSlider::valueChanged,
        this,
        &MainWindow::persistOverlayOpacity
    );

    QObject::connect(
        this->uiPtr->overlayColorButton,
        &QAbstractButton::clicked,
        this,
        &MainWindow::showOverlayColorWindow
    );

    QObject::connect(
        this->uiPtr->overlayToggleButton,
        &QPushButton::toggled,
        this,
        &MainWindow::showOverlay
    );

    QObject::connect(
        this->uiPtr->hideToTrayButton,
        &QPushButton::clicked,
        this,
        &MainWindow::hideToTray
    );
}

void MainWindow::configureWindowPosition()
{
    QDesktopWidget* desktop = QApplication::desktop();
    int screenNumber = desktop->screenNumber(QCursor::pos());
    QRect desktopGeometry = desktop->screenGeometry(screenNumber);
    QPoint desktopCenter = desktopGeometry.center();
    QPoint windowCenter = this->rect().center();
    QPoint windowTopLeft = desktopCenter - windowCenter;

    this->move(windowTopLeft);
}

void MainWindow::configureTrayIcon()
{
    QIcon trayIcon(":/icons/tray.png");
    this->trayIconPtr->setIcon(trayIcon);

    QObject::connect(
        this->trayIconPtr,
        &QSystemTrayIcon::activated,
        this,
        &MainWindow::showFromTray
    );
}

void MainWindow::initializeOverlayWindows()
{
    QList<QScreen*> screens = QApplication::screens();
    for (QScreen* screenPtr: screens) {
        this->initializeOverlayWindow(screenPtr);
    }
}

void MainWindow::loadUserPreferences()
{
    this->loadBreathRate();
    this->loadOverlayThickness();
    this->loadOverlayOpacity();
    this->loadOverlayColor();
}

void MainWindow::loadBreathRate()
{
    QVariant breathRateVariant = this->settings.value("breath-rate");
    if (breathRateVariant.isNull() == false) {
        int breathRate = breathRateVariant.toInt();
        this->uiPtr->breathRateSlider->valueChanged(breathRate);

        for (OverlayWindow* overlayWindowPtr: this->overlayWindows) {
            overlayWindowPtr->setBreathRate(breathRate);
        }
    }
}

void MainWindow::loadOverlayThickness()
{
    QVariant overlayThicknessVariant = this->settings.value("overlay-thickness");
    if (overlayThicknessVariant.isNull() == false) {
        int overlayThickness = overlayThicknessVariant.toInt();
        this->uiPtr->overlayThicknessSlider->valueChanged(overlayThickness);

        for (OverlayWindow* overlayWindowPtr: this->overlayWindows) {
            overlayWindowPtr->setOverlayThickness(overlayThickness);
        }
    }
}

void MainWindow::loadOverlayOpacity()
{
    QVariant overlayOpacityVarian = this->settings.value("overlay-opacity");
    if (overlayOpacityVarian.isNull() == false) {
        int overlayOpacity = overlayOpacityVarian.toInt();
        this->uiPtr->overlayOpacitySlider->valueChanged(overlayOpacity);

        for (OverlayWindow* overlayWindowPtr: this->overlayWindows) {
            overlayWindowPtr->setOverlayOpacity(overlayOpacity);
        }
    }
}

void MainWindow::loadOverlayColor()
{
    QVariant overlayColorVariant = this->settings.value("overlay-color");
    QColor overlayColor = Qt::white;
    if (overlayColorVariant.isNull() == false) {
        overlayColor = overlayColorVariant.value<QColor>();
    }

    this->setOverlayColor(overlayColor);
}

void MainWindow::initializeOverlayWindow(QScreen* screenPtr)
{
    OverlayWindow* overlayWindowPtr = new OverlayWindow(this);

    QObject::connect(
        this->uiPtr->breathRateSlider,
        &QSlider::valueChanged,
        overlayWindowPtr,
        &OverlayWindow::setBreathRate
    );

    QObject::connect(
        this->uiPtr->overlayOpacitySlider,
        &QSlider::valueChanged,
        overlayWindowPtr,
        &OverlayWindow::setOverlayOpacity
    );

    QObject::connect(
        this->uiPtr->overlayThicknessSlider,
        &QSlider::valueChanged,
        overlayWindowPtr,
        &OverlayWindow::setOverlayThickness
    );

    overlayWindowPtr->setBreathRate(this->uiPtr->breathRateSlider->value());
    overlayWindowPtr->setOverlayOpacity(this->uiPtr->overlayOpacitySlider->value());
    overlayWindowPtr->setOverlayThickness(this->uiPtr->overlayThicknessSlider->value());
    overlayWindowPtr->setScreenSize(screenPtr->size());

    overlayWindowPtr->move(screenPtr->geometry().topLeft());
    overlayWindowPtr->show();

    this->overlayWindows.append(overlayWindowPtr);
}

void MainWindow::hideToTray()
{
    this->hide();
    this->trayIconPtr->show();
}

void MainWindow::showFromTray()
{
    this->showNormal();
    this->trayIconPtr->hide();
}

void MainWindow::showOverlay(bool visible)
{
    for (OverlayWindow* overlayWindowPtr: this->overlayWindows) {
        overlayWindowPtr->toggleOpacityAnimation(visible);
    }
}

void MainWindow::showOverlayColorWindow()
{
    this->overlayColorWindowPtr = new QColorDialog(this);

    QObject::connect(
        this->overlayColorWindowPtr,
        &QColorDialog::colorSelected,
        this,
        &MainWindow::setOverlayColor
    );

    this->overlayColorWindowPtr->show();
}

void MainWindow::setOverlayColor(const QColor& color)
{
    for (OverlayWindow* overlayWindowPtr: this->overlayWindows) {
        overlayWindowPtr->setOverlayColor(color);
    }

    this->updateOverlayColorButton(color);

    this->persistOverlayColor(color);
}

void MainWindow::updateOverlayColorButton(const QColor& color)
{
    QAbstractButton* overlayColorButton = this->uiPtr->overlayColorButton;
    QPalette overlayColorButtonPalette = overlayColorButton->palette();
    overlayColorButtonPalette.setColor(QPalette::Button, color);
    overlayColorButton->setPalette(overlayColorButtonPalette);
}

void MainWindow::persistBreathRate(int breathRate)
{
    this->settings.setValue("breath-rate", breathRate);
}

void MainWindow::persistOverlayThickness(int overlayThickness)
{
    this->settings.setValue("overlay-thickness", overlayThickness);
}

void MainWindow::persistOverlayOpacity(int overlayOpacity)
{
    this->settings.setValue("overlay-opacity", overlayOpacity);
}

void MainWindow::persistOverlayColor(QColor overlayColor)
{
    // A very dirty hack to ignore the first clicked signal which is fired by the
    // button group for some reason right after application startup.
    static bool isFirstSignal = true;
    if (isFirstSignal == true) {
        isFirstSignal = false;
        return;
    }

    this->settings.setValue("overlay-color", overlayColor);
}
