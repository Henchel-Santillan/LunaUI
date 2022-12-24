#include "VideoDialog.h"

#include <QApplication>
#include <QGuiApplication>
#include <QRect>
#include <QScreen>

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    VideoDialog dialog;

    // Get the screen size
    QScreen *screen = QGuiApplication::primaryScreen();
    dialog.resize(screen->geometry().width() - 100, screen->geometry().height() - 100);
    dialog.show();

    return application.exec();
}
