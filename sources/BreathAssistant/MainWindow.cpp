#include <BreathAssistant/MainWindow.hpp>
#include <BreathAssistant/OverlayWindow.hpp>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtGui/QColor>
#include <QtGui/QCursor>
#include <QtGui/QIcon>
#include <QtGui/QScreen>
#include <QtGui/QWindow>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSystemTrayIcon>
#include <ui_MainWindow.h>

MainWindow::MainWindow(QWidget* parentPtr) :
    QMainWindow(parentPtr),
    uiPtr(new Ui::MainWindow()),
    trayIconPtr(new QSystemTrayIcon(this))
{
    this->configureUserInterface();
    this->configureWindowPosition();
    this->configureTrayIcon();
    this->initializeOverlayWindows();
}

MainWindow::~MainWindow()
{
    for (OverlayWindow* overlayWindowPtr: this->overlayWindows) {
        delete overlayWindowPtr;
    }

    delete this->trayIconPtr;
    delete this->uiPtr;
}

void MainWindow::configureUserInterface()
{
    this->uiPtr->setupUi(this);

    // todo: refactor to new signal/slot syntax
    QObject::connect(
        this->uiPtr->overlayColorButtonGroup,
        SIGNAL(buttonClicked(QAbstractButton*)),
        this,
        SLOT(setOverlayColor(QAbstractButton*))
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

    QAbstractButton* overlayColorButton = this->uiPtr->overlayColorButtonGroup->checkedButton();
    this->setOverlayColor(overlayColorButton);
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

void MainWindow::setOverlayColor(QAbstractButton* button)
{
    QColor color = button->palette().background().color();

    for (OverlayWindow* overlayWindowPtr: this->overlayWindows) {
        overlayWindowPtr->setOverlayColor(color);
    }
}
