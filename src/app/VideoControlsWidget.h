#ifndef LUNAUI_VIDEOCONTROLSWIDGET_H
#define LUNAUI_VIDEOCONTROLSWIDGET_H

#include <QMediaPlayer>
#include <QWidget>

class QComboBox;
class QHBoxLayout;
class QPushButton;
class QToolButton;
class QSlider;

class VideoControlsWidget : public QWidget {
    Q_OBJECT

public:
    explicit VideoControlsWidget(QWidget *pParent = nullptr);

public slots:
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onChangeRate(qreal rate);

signals:
    void play();
    void pause();
    void stop();
    void changeRate(qreal rate);
    void openButtonClicked();

private slots:
    void onPlayPauseButtonClicked();
    void onRateActivated();

private:
    QHBoxLayout *m_pMainLayout;

    QPushButton *m_pOpenButton;

    QToolButton *m_pStopButton;
    QToolButton *m_pPlayPauseButton;

    QComboBox *m_pRateBox;

    QMediaPlayer::PlaybackState m_playbackState;
    bool m_muted;
};

#endif // LUNAUI_VIDEOCONTROLSWIDGET_H
