#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "intro_outro.h"
#include "progressbar.h"
#include "export.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include <QTimer>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QPixmap>
#include <QDebug>
#include <QScreen>
#include <QStandardPaths>
#include <QElapsedTimer>
#include <QSettings>
#include <QWhatsThis>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class Intro_Outro;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    /* Menu Actions */
    void on_action_Import_triggered();
    bool on_action_Save_triggered();
    bool on_action_Save_As_triggered();
    void on_action_Exit_triggered();
    void on_action_Undo_triggered();
    void on_action_Redo_triggered();
    void on_actionLight_mode_triggered();
    void on_actionDark_mode_triggered();
    void on_action_About_Qt_triggered();
    void on_action_About_FlickFix_triggered();
    void on_action_Tutorial_triggered();

    /* Push Buttons */
    void on_pushButton_Seek_Backward_clicked();
    void on_pushButton_Seek_Backward_pressed();
    void on_pushButton_Seek_Backward_released();
    void on_pushButton_Seek_Forward_clicked();
    void on_pushButton_Seek_Forward_pressed();
    void on_pushButton_Seek_Forward_released();
    void on_pushButton_Play_Pause_clicked();
    void on_pushButton_Mute_clicked();
    void on_pushButton_Screenshot_clicked();
    void on_pushButton_Audio_clicked();
    void on_pushButton_Add_Audio_clicked();
    void on_pushButton_Intro_clicked();
    void on_pushButton_Outro_clicked();
    void on_pushButton_Append_Video_clicked();
    void on_pushButton_Prepend_Video_clicked();
    void on_pushButton_Trim_clicked();
    void on_pushButton_Trim_Start_clicked();
    void on_pushButton_Trim_End_clicked();
    void on_pushButton_Trim_Save_clicked();
    void on_pushButton_Trim_Remove_clicked();
    void on_action_Export_triggered();

    void durationChanged(qint64 duration);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void on_horizontalSlider_Position_valueChanged(int value);
    void on_horizontalSlider_Volume_valueChanged(int value);
    void onTempVideoUpdated(int exitCode, QProcess::ExitStatus exitStatus, QProcess *process);

    bool videoIsValid(QString &videoFile);
    bool videoHasAudio(const QString &videoFile);
    void setTrimElementVisibility(bool visible);
    void setAudioElementVisibility(bool visible);
    void newTempFile();
    void deleteTempFiles();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus, QProcess *process);
    void removeVideo();
    void importVideo(const QString &videoFile);
    void saveBeforeImport(const QString &fileName);
    void setupVideoWidget(const QString &videoFile);
    void setVideoGeometry();
    void setEnabledVideoControls(bool enabled);
    void addVideo(const QString &newVideoFile, bool append);
    void loadSettings();
    void updatePosition(qint64 position);
    void updateSlider();
    void startProcess(QProcess *process, const QStringList &parameters);
    double getMediaDuration(const QString &mediaFile);

    QString convertToMP4(const QString &videoFile, bool isButton);
    QString fileType(const QString &videoFile);
    QString trimAudioFile(const QString &audioFile, double videoDuration);

private:
    Ui::MainWindow *ui;
    Intro_Outro *intro_outro;
    ProgressBar *progressBar;
    Export *exportDialog;
    QMediaPlayer *Player;
    QVideoWidget *Video;
    QTimer *Timer;
    QTimer *seekForwardTimer;
    QTimer *seekBackwardTimer;
    QTimer *progressTimer;
    QTime currentTime;
    QTime totalTime;
    QTime startTime;
    QTime endTime;
    qint64 vidDuration;
    qint64 mLastValidPosition;
    int tempFileCount = 0;
    int currentFile = 0;
    bool is_Play = true;
    bool is_Muted = false;
    bool mediaAtEnd = false;
    bool mediaIsLoaded = false;
    bool isSaved = true;
    QString timeFormat;
    QString videoFile;
    QString SavedProjectFile;
    QString TempProjectFile;
    QString TempOutputFile;
    QString SilentAudioFile;
    QString trimmedAudioFile;
    QString tempFile;
    QSettings settings;
    QStringList acceptibleFormats = QStringList({"mp4", "avi", "mkv", "mov", "wmv"});;
};
#endif // MAINWINDOW_H
