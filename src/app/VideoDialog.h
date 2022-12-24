#ifndef LUNAUI_VIDEODIALOG_H
#define LUNAUI_VIDEODIALOG_H

#include "BaseDialog.h"

#include <QMediaPlayer>

class VideoControlsWidget;
class VideoRecentListWidget;
class VideoWidget;

class QAudioOutput;
class QCheckBox;
class QComboBox;
class QLabel;
class QSlider;
class QUrl;

class VideoDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit VideoDialog(QWidget *pParent = nullptr);

private slots:
    void open();
    void onDurationSliderMoved(int seconds);

    void onDurationChanged(qint64 duration);
    void onErrorChanged();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState status);
    void onPositionChanged(qint64 progress);

    void onDeviceActivated(int index);
    void onRecentListItemActivated(const QUrl &url);
    void onLoopStateChanged(int state);

private:
    VideoControlsWidget *m_pControlsWidget;
    VideoRecentListWidget *m_pRecentListWidget;
    VideoWidget *m_pVideoWidget;

    QComboBox *m_pDeviceBox;
    QCheckBox *m_pLoopCheckBox;

    QLabel *m_pDurationLabel;
    QSlider *m_pDurationSlider;

    QLabel *m_pErrorLabel;
    QLabel *m_pStatusLabel;

    QMediaPlayer *m_pMediaPlayer;
    QAudioOutput *m_pAudioOutput;
    qint64 m_duration;
};

#endif // LUNAUI_VIDEODIALOG_H
