#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QtCore>
#include <QtCore/QList>
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
        Ui::MainWindow* uiPtr;
        QSystemTrayIcon* trayIconPtr;
        QList<OverlayWindow*> overlayWindows;

    private slots:
        void hideToTray();
        void showFromTray();
        void showOverlay(bool visible);
        void setOverlayColor(QAbstractButton* buttonPtr);
};

#endif // MAIN_WINDOW_HPP
