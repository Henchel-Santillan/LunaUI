#include "VideoDialog.h"
#include "SerialViewDialog.h"

#include <QApplication>
#include <QGuiApplication>
#include <QRect>
#include <QScreen>

// Current configuration: primary screen width - 100 x primary screen height - 100
// Configuration for SerialViewDialog: primary screen width - 200 x primary screen height - 200
// TODO: Create LunaHome, UI entry point

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    //VideoDialog dialog;
    SerialViewDialog dialog;

    // Get the screen size
    QScreen *screen = QGuiApplication::primaryScreen();
    dialog.resize(screen->geometry().width() - 200, screen->geometry().height() - 200);
    dialog.show();

    return application.exec();
}
