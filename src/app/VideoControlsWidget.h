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
    void onChangeVolume(float volume);
    void onChangeMuted(bool muted);
    void onChangeRate(qreal rate);

signals:
    void play();
    void pause();
    void stop();
    void changeVolume(float volume);
    void changeMuted(bool muted);
    void changeRate(qreal rate);
    void openButtonClicked();

private slots:
    void onPlayPauseButtonClicked();
    void onMuteButtonClicked();
    void onVolumeSliderValueChanged();
    void onRateActivated();

private:
    QHBoxLayout *m_pMainLayout;

    QPushButton *m_pOpenButton;

    QToolButton *m_pStopButton;
    QToolButton *m_pPlayPauseButton;
    QToolButton *m_pMuteButton;

    QSlider *m_pVolumeSlider;

    QComboBox *m_pRateBox;

    QMediaPlayer::PlaybackState m_playbackState;
    bool m_muted;
};

#endif // LUNAUI_VIDEOCONTROLSWIDGET_H
