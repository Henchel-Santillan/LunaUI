#include "VideoDialog.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    VideoDialog dialog;
    dialog.show();
    return application.exec();
}
