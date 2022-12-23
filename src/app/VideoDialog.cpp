#include "VideoDialog.h"

#include "VideoControlsWidget.h"
#include "VideoRecentListWidget.h"
#include "VideoWidget.h"

#include <QAudioDevice>
#include <QAudioOutput>
#include <QComboBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMediaDevices>
#include <QMessageBox>
#include <QSizePolicy>
#include <QSlider>
#include <QStandardPaths>
#include <QStringList>
#include <QUrl>
#include <QVBoxLayout>


// ========== CONSTRUCTOR DEFINITION

VideoDialog::VideoDialog(QWidget *pParent)
    : BaseDialog(pParent)
    , m_pControlsWidget(new VideoControlsWidget)
    , m_pRecentListWidget(new VideoRecentListWidget)
    , m_pVideoWidget(new VideoWidget)
    , m_pDeviceBox(nullptr)
    , m_pDurationLabel(new QLabel)
    , m_pDurationSlider(new QSlider(Qt::Horizontal))
    , m_pErrorLabel(new QLabel)
    , m_pStatusLabel(new QLabel)
    , m_pMediaPlayer(new QMediaPlayer(this))
    , m_pAudioOutput(new QAudioOutput(this))
    , m_duration(0)
{   
    // Media Player
    m_pMediaPlayer->setAudioOutput(m_pAudioOutput);
    m_pMediaPlayer->setVideoOutput(m_pVideoWidget);

    QObject::connect(m_pMediaPlayer, &QMediaPlayer::durationChanged, this, &VideoDialog::onDurationChanged);
    QObject::connect(m_pMediaPlayer, &QMediaPlayer::errorChanged, this, &VideoDialog::onErrorChanged);
    QObject::connect(m_pMediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &VideoDialog::onMediaStatusChanged);
    QObject::connect(m_pMediaPlayer, &QMediaPlayer::positionChanged, this, &VideoDialog::onPositionChanged);

    // Controls widget
    QObject::connect(m_pControlsWidget, &VideoControlsWidget::openButtonClicked, this, &VideoDialog::open);

    QObject::connect(m_pControlsWidget, &VideoControlsWidget::play, m_pMediaPlayer, &QMediaPlayer::play);
    QObject::connect(m_pControlsWidget, &VideoControlsWidget::pause, m_pMediaPlayer, &QMediaPlayer::pause);
    QObject::connect(m_pControlsWidget, &VideoControlsWidget::stop, m_pMediaPlayer, &QMediaPlayer::stop);
    QObject::connect(m_pControlsWidget, &VideoControlsWidget::stop, m_pVideoWidget, QOverload<>::of(&QVideoWidget::update));
    QObject::connect(m_pControlsWidget, &VideoControlsWidget::changeVolume, m_pAudioOutput, &QAudioOutput::setVolume);
    QObject::connect(m_pControlsWidget, &VideoControlsWidget::changeMuted, m_pAudioOutput, &QAudioOutput::setMuted);
    QObject::connect(m_pControlsWidget, &VideoControlsWidget::changeRate, m_pMediaPlayer, &QMediaPlayer::setPlaybackRate);

    // Recent List Widget
    QObject::connect(m_pRecentListWidget, &VideoRecentListWidget::itemActivated, this, &VideoDialog::onRecentListItemActivated);

    // Set up the viewer layout
    QHBoxLayout *pViewerLayout = new QHBoxLayout;
    pViewerLayout->addWidget(m_pVideoWidget);
    pViewerLayout->addWidget(m_pRecentListWidget);

    // Set up the info layout
    m_pErrorLabel->setWordWrap(true);
    m_pErrorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_pStatusLabel->setWordWrap(true);
    m_pStatusLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QHBoxLayout *pInfoLayout = new QHBoxLayout;
    pInfoLayout->addWidget(m_pErrorLabel);
    pInfoLayout->addWidget(m_pStatusLabel);

    // Set up the duration layout
    m_pDurationLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    m_pDurationSlider->setTickPosition(QSlider::NoTicks);
    m_pDurationSlider->setMinimum(0);
    QObject::connect(m_pDurationSlider, &QAbstractSlider::sliderMoved, this, &VideoDialog::onSliderMoved);

    QHBoxLayout *pDurationLayout = new QHBoxLayout;

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    // Set up the audio output device combo box
    m_pDeviceBox = new QComboBox;
    m_pDeviceBox->addItem(QString("Default"), QVariant::fromValue(QMediaDevices::defaultAudioOutput()));

    for (const auto &device : QMediaDevices::audioOutputs()) {
        m_pDeviceBox->addItem(device.description(), QVariant::fromValue(device));
    }

    QObject::connect(m_pDeviceBox, QOverload<int>::of(&QComboBox::activated), this, &VideoDialog::onDeviceActivated);

    // Add the audio output device combo box to the horizontal box layout
    pDurationLayout->addWidget(m_pDeviceBox);
#endif

    pDurationLayout->addWidget(m_pDurationSlider);
    pDurationLayout->addWidget(m_pDurationLabel);

    // Set up the controls layout
    QVBoxLayout *pControlsLayout = new QVBoxLayout;
    pControlsLayout->addWidget(m_pControlsWidget);
    pControlsLayout->addLayout(pDurationLayout);

    // Set up the main layout
    QVBoxLayout *pDialogLayout = new QVBoxLayout;
    pDialogLayout->addLayout(pViewerLayout);
    pDialogLayout->addLayout(pInfoLayout);
    pDialogLayout->addLayout(pControlsLayout);

    this->insertLayoutAt(0, pDialogLayout);

    if (!m_pMediaPlayer->isAvailable()) {
        // Show an alert if it is determined that the MediaPlayer instance is not available
        QMessageBox::warning(this, "Media Player Warning", "Media Player not available.");

        // Disable controls
        pControlsLayout->setEnabled(false);
        pViewerLayout->setEnabled(false);
    }
}


// ========== PRIVATE SLOTS

void VideoDialog::open() {
    QFileDialog dialog;
    dialog.setWindowTitle("Open Dialog");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    dialog.setOptions(QFileDialog::ReadOnly);

    if (dialog.exec() == QDialog::Accepted) {
        auto path = dialog.selectedFiles().at(0);
        auto url = dialog.selectedUrls().at(0);

        // Add to the Recent List
        m_pRecentListWidget->push(path, url);

        // Start the MediaPlayer
        m_pMediaPlayer->setSource(url);
    }
}

void VideoDialog::onSliderMoved(int seconds) {
    m_pMediaPlayer->setPosition(seconds);
}

void VideoDialog::onDurationChanged(qint64 duration) {
    m_duration = duration;
    m_pDurationSlider->setMaximum(duration);
}

void VideoDialog::onErrorChanged() {
    if (m_pMediaPlayer->error() != QMediaPlayer::NoError) {
        // Update the error label with the QMediaPlayer Error
        m_pErrorLabel->setText("[ERROR] " + m_pMediaPlayer->errorString());
    } else {
        // Otherwise, clear it.
        m_pErrorLabel->clear();
    }
}

void VideoDialog::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    QString label("[STATUS] ");

    switch (status) {
        case QMediaPlayer::LoadingMedia:
            label.append("Loading");
            break;
        case QMediaPlayer::BufferingMedia:
        case QMediaPlayer::BufferedMedia:
            label.append("Buffering");
            break;
        case QMediaPlayer::StalledMedia:
            label.append("Stalled");
            break;
        case QMediaPlayer::InvalidMedia:
            label.append("Invalid Media");
            onErrorChanged();
            break;
        case QMediaPlayer::NoMedia:
            label.append("No Media");
            break;
        case QMediaPlayer::LoadedMedia:
            label.append("Loaded Media");
            break;
        default:
            label.append("");
            break;
    }

    m_pStatusLabel->setText(label);
}

void VideoDialog::onPositionChanged(qint64 progress) {
    // Set the slider value
    if (!m_pDurationSlider->isSliderDown()) 
        m_pDurationSlider->setValue(progress);

    // Update the duration label
    qint64 durationInfo = progress / 1000;
    QString timeString("%1 %2 %3");

    if (durationInfo != 0 || m_duration != 0) {
        QTime current((durationInfo / 3600) % 60, (durationInfo / 60) % 60, durationInfo % 60, (durationInfo * 1000) % 1000);
        QTime total((m_duration / 3600) % 60, (m_duration / 60) % 60, m_duration % 60, (m_duration * 1000) % 1000);
        QString timeFormat = (m_duration > 3600 /* 1 hour */) ? "hh:mm:ss" : "mm:ss";
        timeString = current.toString(timeFormat) + " | " + total.toString(timeFormat); 
    }

    m_pDurationLabel->setText(timeString);
}

void VideoDialog::onDeviceActivated(int index) {
    auto device = m_pDeviceBox->itemData(index).value<QAudioDevice>();
    m_pMediaPlayer->audioOutput()->setDevice(device);
}

void VideoDialog::onRecentListItemActivated(const QUrl &url) {
    m_pMediaPlayer->setSource(url);
}
