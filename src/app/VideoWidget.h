#ifndef LUNAUI_VIDEOWIDGET_H
#define LUNAUI_VIDEOWIDGET_H

#include <QVideoWidget>

class QKeyEvent;
class QMouseEvent;

class VideoWidget : public QVideoWidget {
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *pParent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *pEvent) override;
    void mouseDoubleClickEvent(QMouseEvent *pEvent) override;
};

#endif // LUNAUI_VIDEOWIDGET_H
