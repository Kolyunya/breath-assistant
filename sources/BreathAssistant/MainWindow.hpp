#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QtCore/QList>
#include <QtCore/QSettings>
#include <QtWidgets/QMainWindow>

class OverlayWindow;
class QAbstractButton;
class QScreen;
class QSystemTrayIcon;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parentPtr = Q_NULLPTR);
        ~MainWindow();

    private:
        void configureUserInterface();
        void configureWindowPosition();
        void configureTrayIcon();
        void initializeOverlayWindows();
        void initializeOverlayWindow(QScreen* screenPtr);
        void loadUserPreferences();
        void loadBreathRate();
        void loadOverlayThickness();
        void loadOverlayOpacity();
        void loadOverlayColor();
        QSettings settings;
        Ui::MainWindow* uiPtr;
        QSystemTrayIcon* trayIconPtr;
        QList<OverlayWindow*> overlayWindows;

    private slots:
        void hideToTray();
        void showFromTray();
        void showOverlay(bool visible);
        void setOverlayColor(QAbstractButton* buttonPtr);
        void persistBreathRate(int breathRate);
        void persistOverlayThickness(int overlayThickness);
        void persistOverlayOpacity(int overlayOpacity);
        void persistOverlayColor(QColor overlayColor);
};

#endif // MAIN_WINDOW_HPP
