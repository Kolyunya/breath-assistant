#include <BreathAssistant/MainWindow.hpp>
#include <QtWidgets/QApplication>

int main(int argc, char** argv)
{
    QApplication application(argc, argv);
    application.setOrganizationName("https://github.com/Kolyunya");
    application.setOrganizationDomain("https://github.com/Kolyunya");
    application.setApplicationName("Breath Assistant");

    MainWindow window;
    window.show();

    return application.exec();
}
