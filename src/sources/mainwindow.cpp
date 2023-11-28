#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tutorial.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    progressBar(new ProgressBar(this)),
    settings("CSU", "FlickFix")
{
    ui->setupUi(this);
    ui->groupBox_Video->installEventFilter(this);
    progressBar->hide();
    progressBar->raise();

    Player = new QMediaPlayer();
    Video = new QVideoWidget();
    Player->setVolume(ui->horizontalSlider_Volume->value());

    TempProjectFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/temp_project.mp4";
    TempOutputFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/temp_output0.mp4";

    ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButton_Seek_Backward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pushButton_Seek_Forward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->pushButton_Mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    // Connect the Player durationChanged changed signal to the durationChanged slot function
    connect(Player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);

    // Connect the Player mediaStatusChanged signal to the durationChanged slot function
    connect(Player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::mediaStatusChanged);

    ui->horizontalSlider_Position->setRange(0, Player->duration());

    // Set up the timer for slider position and duration label updates
    Timer = new QTimer(this);

    // Connect the Timer timeout signal to the updateSlider slot.
    // This calls updateSlider every time the Timer finishes an interval.
    connect(Timer, &QTimer::timeout, this, &MainWindow::updateSlider);

    // Timer runs in 100ms intervals, so that the slider is updated that often
    Timer->start(100);

    seekForwardTimer = new QTimer(this);
    seekBackwardTimer = new QTimer(this);

    startTime = QTime(0,0,0);
    endTime = totalTime;
    timeFormat = (currentTime.minute() < 10) ? "m:ss.zzz" : "mm:ss.zzz";

    ui->pushButton_Add_Audio->setVisible(false);
    setTrimElementVisibility(false);
    loadSettings();
}

MainWindow::~MainWindow()
{
    Timer->stop();
    delete Player;
    delete Video;
    delete Timer;
    delete ui;
}

// Handles the actions following the closing of the program
void MainWindow::closeEvent(QCloseEvent *event)
{
    // Check if the project has been saved
    if (isSaved)
    {
        deleteTempFiles(); // Clean up any temporary files
        event->accept();   // Close the program
    }
    else
    {
        // Prompt the user about unsaved changes with the options to 'Save', 'Discard', or 'Cancel'
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Unsaved Changes",
                                                                  "You have unsaved changes. Would you like to save before exiting?",
                                                                  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        // If the user clicked 'Save'
        if (reply == QMessageBox::Save)
        {
            // Save the project, determine if saving was successful or user cancelled the save
            bool saveSuccessful = on_action_Save_triggered();

            if (saveSuccessful) // Successful save
            {
                deleteTempFiles(); // Clean up temporary files
                event->accept();   // Close the program
            }
            else // Save was cancelled
            {
                event->ignore(); // Keep the program open
            }
        }
        // If the user clicked 'Discard'
        else if (reply == QMessageBox::Discard)
        {
            deleteTempFiles(); // Clean up temporary files
            event->accept();   // Close the program
        }
        else // The user clicked 'Cancel'
        {
            event->ignore(); // Keep the program open
        }
    }
}

// Deletes the temp files used by a video project
void MainWindow::deleteTempFiles()
{
    // Clear the media from the player
    Player->setMedia(QMediaContent());

    // Determine the temporary directory path
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QDir dir(tempPath);

    // Filter for temp file names with specific prefixes
    QStringList filters;
    filters << "temp_output*" << "temp_project*"
            << "trimmed_audio*" << "temp_output.PNG" << "intro.mp4"
            << "outro.mp4" << "temp_file.mp4";

    // Retrieve a list of files in the temp directory matching the filters
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);

    // Delete each file from the list
    foreach (QFileInfo file, files)
    {
        // Remove the file
        QFile::remove(file.absoluteFilePath());
    }
}

// Menu bar option to exit the program
void MainWindow::on_action_Exit_triggered()
{
    this->close(); // Close the program
}

// Menu bar option to learn about FlickFix
void MainWindow::on_action_About_FlickFix_triggered()
{
    QString message = "<h3><b>About FlickFix</b></h3>"
                      "<p>This project was created by Matthew Weigel for his Senior Project at Charleston Southern University.<br><br>"
                      "The icons used are from icons8.com and Qt</p>";

    QMessageBox::about(this, "About FlickFix", message);
}

// Menu bar option to learn about Qt
void MainWindow::on_action_About_Qt_triggered()
{
    // Display the "About Qt" dialog box
    QApplication::aboutQt();
}

// Menu bar option to open the FlickFix tutorial
void MainWindow::on_action_Tutorial_triggered()
{
    // Display the tutorial dialog
    Tutorial tutorialDialog(this);
    tutorialDialog.exec();
}

// Menu bar option import a new video
void MainWindow::on_action_Import_triggered()
{
    // Open a file dialog to let the user select a video file
    QString videoFile = QFileDialog::getOpenFileName(this, tr("Select Video File"), "", tr("Video Files (*.mp4 *.avi *.mkv *.mov *.wmv);;All Files (*)"));

    // Continue only if a file has been selected
    if (!videoFile.isEmpty())
    {
        // Checks if the file is an acceptible file format
        if (videoIsValid(videoFile))
        {
            // If valid, initiate the import process
            saveBeforeImport(videoFile);
        }
    }
}

// Menu Bar option Export which did not make it to first release
void MainWindow::on_action_Export_triggered()
{
    if (mediaIsLoaded) // Only allow exporting if a video has been imported
    {
        Export exportDialog(this);
        exportDialog.exec();
    }
}

// Validates a video that is trying to be imported - ensures that is an acceptible video file format,
// converts to mp4 if not mp4, and adds silent audio to videos without sound, both for more consistent
// and efficient video handling and editing.
bool MainWindow::videoIsValid(QString &videoFile)
{
    bool videoIsValid = false; // Flag to indicate if the video is valid or not
    QString extension = fileType(videoFile); // Get the extension from the video path

    // Check if the file has an accepted extension
    if (acceptibleFormats.contains(extension))
    {
        // Check if the file is not an MP4 file
        if (extension != "mp4")
        {
            // Convert the non-MP4 video file to MP4 format
            videoFile = convertToMP4(videoFile, false);
        }

        // Check if the video lacks an audio track
        if (!videoHasAudio(videoFile))
        {
            QStringList parameters; // FFmpeg parameters for adding silent audio

            // Define the path for the new video file with added silent audio
            SilentAudioFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/silent_audio.mp4";

            // Set FFmpeg parameters to add a silent audio track to the video
            parameters << "-y" << "-i" << videoFile << "-f" << "lavfi" << "-i" << "anullsrc=channel_layout=stereo:sample_rate=48000"
                       << "-c:v" << "copy" << "-c:a" << "aac" << "-shortest" << SilentAudioFile;

            // Execute the FFmpeg command with the given parameters
            QProcess::execute("ffmpeg", parameters);

            // Update the video file path to point to the new file with added silent audio
            videoFile = SilentAudioFile;
        }

        videoIsValid = true; // Mark the video as valid
    }
    else
    {
        // If the video has an unaccepted extension, show a warning to the user
        QMessageBox::warning(this, tr("Invalid file format"), tr("Only .mp4, .avi, .mkv, .mov, and .wmv files are accepted."));
    }

    return videoIsValid; // Return the validity status of the video
}

// Handles the action of saving a previous video before importing a new one
void MainWindow::saveBeforeImport(const QString &videoFile)
{
    // Check if a video file has already been imported and loaded
    if (mediaIsLoaded)
    {
        // Prompt the user about unsaved changes with the options to 'Save', 'Discard', or 'Cancel'
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Unsaved Changes",
                                                                  "You have unsaved changes. Would you like to save before exiting?",
                                                                  QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
        // If the user clicked 'Save'
        if (reply == QMessageBox::Save)
        {
            // Save the project, determine if saving was successful or user cancelled the save
            bool saveSuccessful = on_action_Save_triggered();

            // If the save operation was successful and not canceled
            if (saveSuccessful)
            {
                // Import the new video file
                importVideo(videoFile);
            }
        }
        // If the user clicked 'Discard'
        else if (reply == QMessageBox::Discard)
        {
            // Directly import the new video without saving current changes
            importVideo(videoFile);
        }
    }
    else
    {
        // If there's no media currently loaded, directly import the video file
        importVideo(videoFile);
    }
}

// Handles the importing of a new video
void MainWindow::importVideo(const QString &videoFile)
{
    // Check if a video file has already been imported and loaded
    if (mediaIsLoaded)
    {
        // Clean up and reset the temporary file count from the previously imported video
        deleteTempFiles();
        tempFileCount = 0;
        currentFile = 0;
    }

    // Copy the new video file to temporary locations for storing and processing
    QFile::copy(videoFile, TempOutputFile);
    QFile::copy(videoFile, TempProjectFile);

    // Update the project's saved state to "not saved" since a new video has been imported
    isSaved = false;

    // Initialize the video widget to display the imported video
    setupVideoWidget(videoFile);

    // Enable controls related to video playback and editing
    setEnabledVideoControls(true);
}

// Sets up the video widget for displaying to the user
void MainWindow::setupVideoWidget(const QString &videoFile)
{
    // Adjust the dimensions and position of the video within its container
    setVideoGeometry();

     // Assign the video to the corresponding widget and player
    Video->setParent(ui->groupBox_Video);
    Player->setVideoOutput(Video);

    // Load the imported video file into the player
    Player->setMedia(QUrl::fromLocalFile(videoFile));

    // Once a video is imported, remove the visible border of the video container
    ui->groupBox_Video->setStyleSheet("QGroupBox { border: none; }");

    // Hide the import instructions and icon in the video display area
    ui->label_Import_Instructions->setVisible(false);
    ui->label_Import_Icon->setVisible(false);

    // Display the video
    Video->setVisible(true);
    Video->show();
}

// Sets the video player geometry
void MainWindow::setVideoGeometry()
{
    // Given Aspect ratio: 16:9
    const double aspectRatio = 16.0 / 9.0;

    // Calculate width based on groupBox's height needed to maintain the 16:9 aspect ratio
    int widthFromHeight = ui->groupBox_Video->height() * aspectRatio;

    // Calculate height based on groupBox's width needed to maintain the 16:9 aspect ratio
    int heightFromWidth = ui->groupBox_Video->width() / aspectRatio;

    int finalWidth, finalHeight;

    // Check if the width (calculated from groupBox's full height to maintain 16:9 ratio) fits within groupBox's width
    if (widthFromHeight <= ui->groupBox_Video->width())
    {
        // The calculated width fits: so, use the full height of the groupBox and adjust the width to maintain the 16:9 ratio.
        finalWidth = widthFromHeight;
        finalHeight = ui->groupBox_Video->height();
    }
    else // If the calculated width is too wide for the groupBox
    {
        // Use the full width of the groupBox and adjust the height to maintain the 16:9 ratio.
        finalWidth = ui->groupBox_Video->width();
        finalHeight = heightFromWidth;
    }

    // Calculate centering offsets
    int xOffset = (ui->groupBox_Video->width() - finalWidth) / 2;
    int yOffset = (ui->groupBox_Video->height() - finalHeight) / 2;

    // Set the Video widget's geometry
    Video->setGeometry(xOffset, yOffset, finalWidth, finalHeight);
}

// Menu bar option to 'Save As' new video
bool MainWindow::on_action_Save_As_triggered()
{
    if (mediaIsLoaded) // Only allow a save if there is a video loaded
    {
        // Open a file dialog for the user to specify where they want to save their video project
        SavedProjectFile = QFileDialog::getSaveFileName(this, tr("Save Video File"), "", tr("MP4 Files (*.mp4)"));

        // Checks if the user has selected a file location
        if (!SavedProjectFile.isEmpty())
        {
            // Copy the current project from the temp file to the desired save location
            QFile::copy(TempProjectFile, SavedProjectFile);
            isSaved = true;  // Indicate that the project is saved
        }
        else
        {
            // User cancelled the save or did not select a file location
            isSaved = false;
        }
    }
    return isSaved;  // Return the current save state
}

// Menu bar option to update/overwrite save
bool MainWindow::on_action_Save_triggered()
{
    // Checks if a save location from a previous save has already been specified
    if (QFile::exists(SavedProjectFile))
    {
        // Remove the previously saved version
        QFile::remove(SavedProjectFile);

        // Copy the current project from the temp file to the saved project location
        QFile::copy(TempProjectFile, SavedProjectFile);
        isSaved = true;  // Indicate that the project is saved
    }
    else
    {
        // If a save location has not been specified, prompt the user to choose a location via the Save As dialog
        isSaved = on_action_Save_As_triggered();
    }
    return isSaved;  // Return the current save state
}

// Enables the video editing options
void MainWindow::setEnabledVideoControls(bool enabled)
{
    // Toggle the enable state of various video editing and control buttons
    ui->pushButton_Append_Video->setEnabled(enabled);
    ui->pushButton_Prepend_Video->setEnabled(enabled);
    ui->pushButton_Outro->setEnabled(enabled);
    ui->pushButton_Trim->setEnabled(enabled);
    ui->pushButton_Audio->setEnabled(enabled);
    ui->pushButton_Screenshot->setEnabled(enabled);
}

// Handles the dragging a video into the player
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    // Check if the drag event contains URLs (such as file paths)
    if (event->mimeData()->hasUrls())
    {
        // If URLs are present, accept the event for further processing
        event->acceptProposedAction();
    }
}

// Handles the dropping a video into the player after dragging
void MainWindow::dropEvent(QDropEvent *event)
{
    // Iterate over all URLs contained in the drop event
    foreach (const QUrl &url, event->mimeData()->urls())
    {
        // Get the video file path from the URL
        QString videoFile = url.toLocalFile();

        // Check if the dropped file has an acceptable format
        if (videoIsValid(videoFile))
        {
            // If the file format is valid, import the file
            saveBeforeImport(videoFile);
        }
    }
}

// Handles the import by clicking on video player option
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // Check if the event occurred in the video group box and is a mouse button press
    if (obj == ui->groupBox_Video && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        // If the mouse button pressed is the left button
        if(mouseEvent->button() == Qt::LeftButton)
        {
            // If no media is loaded yet, trigger the import action
            if (!mediaIsLoaded)
            {
                on_action_Import_triggered();
            }
        }
    }

    // Allow the parent class to process the event further
    return QMainWindow::eventFilter(obj, event);
}

// Handles the resizing of the video player when the program is resized
void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event); // Event parameter is unused

    setVideoGeometry(); // Set the geometry of the video
}

// This function is triggered when the media status is loaded or finishes
void MainWindow::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    // Check if media is loaded into the player
    if (status == QMediaPlayer::LoadedMedia)
    {
        // Update the flag to indicate that media is loaded
        mediaIsLoaded = true;

        // Get the total duration of the media
        vidDuration = Player->duration();

        // Update the duration to start from the beginning (0 seconds)
        updatePosition(0);

        // Set the play/pause button icon to the play icon
        ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

        is_Play = true; // Update the flag to indicate the button is play

        // Play the video and immediately pause to load a preview
        Player->play();
        Player->pause();
    }
    // Check if the media playback has reached the end
    else if (status == QMediaPlayer::EndOfMedia)
    {
        // Set the play/pause button icon to the play icon
        ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

        is_Play = true; // Update the flag to indicate the button is play
        mediaAtEnd = true; // Update the flag to indicate that media playback has finished

        // Manually set the media position to its end
        Player->setPosition(vidDuration);
    }
}

// This function is triggered whenever the duration of the video changes.
void MainWindow::durationChanged(qint64 duration)
{
    // Store the updated duration of the video
    vidDuration = duration;

    // Adjust the maximum value of the position slider based on the new duration
    ui->horizontalSlider_Position->setMaximum(vidDuration);
}

// This function is periodically called to update the position slider and labels while the video is playing.
void MainWindow::updateSlider()
{
    // Check that there is a loaded video
    if (mediaIsLoaded)
    {
        // Get the current playback position of the video
        qint64 position = Player->position();

        // Update the value of the slider based on the new position
        ui->horizontalSlider_Position->setValue(position);


        // Update the playback time label
        updatePosition(position); // This is called every 100ms due to the timer's configuration
    }
}

// This function updates the labels displaying the current playback time and total duration of the video.
void MainWindow::updatePosition(qint64 position)
{
    // Ensure that there's either a position or a video duration to process
    if (position || vidDuration)
    {
        // Convert the current position (in milliseconds) into hours:minutes:seconds format
        currentTime = QTime(0, position / (60 * 1000), (position / 1000) % 60, position % 1000);

        // Convert the total video duration (in milliseconds) into hours:minutes:seconds format
        totalTime = QTime(0, vidDuration / (60 * 1000), (vidDuration / 1000) % 60, vidDuration % 1000);

        // Update the labels with the current time and total video duration
        ui->label_current_Time->setText(currentTime.toString(timeFormat).left(7));
        ui->label_total_Time->setText(totalTime.toString(timeFormat).left(7));
    }
}

// The functions controls the change of video position when the slider is moved
void MainWindow::on_horizontalSlider_Position_valueChanged(int value)
{
    // Check if the video position slider is being manually clicked and dragged by the user
    if (ui->horizontalSlider_Position->isSliderDown())
    {
        // Set the player position to the new slider value
        Player->setPosition(value);
    }
}

// This function is triggered when the seek backward button is clicked
void MainWindow::on_pushButton_Seek_Backward_clicked()
{
    // Check if media is loaded before performing seek operation
    if (mediaIsLoaded)
    {
        // Calculate the new position by moving backwards by 100 milliseconds (0.1 second)
        qint64 newPosition = Player->position() - 100;

        // If the new position is less than 0 (start of the media), reset it to 0 to avoid negative position
        if (newPosition < 0) newPosition = 0;

        // Set the media player's position to the new position
        Player->setPosition(newPosition);

        // Update the position slider to reflect the new position
        ui->horizontalSlider_Position->setValue(newPosition);

        // If the media was at the end and the new position is within the duration of the video,
        // update the mediaAtEnd flag to false to indicate that media is no longer at the end.
        if (mediaAtEnd && newPosition < vidDuration)
        {
            mediaAtEnd = false;
        }
    }
    // If media is not loaded, no seek operation is performed
}


// This function is triggered when the seek forward button is clicked
void MainWindow::on_pushButton_Seek_Forward_clicked()
{
    // Check if media is loaded before performing seek operation
    if (mediaIsLoaded)
    {
        // Calculate the new position by moving forward by 100 milliseconds (0.1 second)
        qint64 newPosition = Player->position() + 100;

        // Check if the media playback is not already at the end
        if (!mediaAtEnd)
        {
            // If the new position is within the duration of the video
            if (newPosition <= vidDuration)
            {
                // Set the media player's position to the new position
                Player->setPosition(newPosition);
                // Update the position slider to reflect the new position
                ui->horizontalSlider_Position->setValue(newPosition);
            }
            else // If the new position exceeds the video duration
            {
                // Set the player's position to the end of the video
                Player->setPosition(vidDuration);
                // Update the position slider to the end of the video
                ui->horizontalSlider_Position->setValue(vidDuration);
            }
        }
    }
    // If media is not loaded, no seek operation is performed.
}


// Handles the function of holding the seek backward button
void MainWindow::on_pushButton_Seek_Backward_pressed()
{
    connect(seekBackwardTimer, &QTimer::timeout, this, &MainWindow::on_pushButton_Seek_Backward_clicked);
    seekBackwardTimer->start(100);
}

// Handles the function of releasing the seek backward button
void MainWindow::on_pushButton_Seek_Backward_released()
{
    seekBackwardTimer->stop();
}

// Handles the function of holding the seek forward button
void MainWindow::on_pushButton_Seek_Forward_pressed()
{
    connect(seekForwardTimer, &QTimer::timeout, this, &MainWindow::on_pushButton_Seek_Forward_clicked);
    seekForwardTimer->start(100);
}

// Handles the function of releasing the seek forward button
void MainWindow::on_pushButton_Seek_Forward_released()
{
    seekForwardTimer->stop();
}

// This function handles the behavior of the play/pause button
void MainWindow::on_pushButton_Play_Pause_clicked()
{
    // Check if media is loaded before proceeding.
    if (mediaIsLoaded)
    {
        // If the media is currently playing.
        if (is_Play)
        {
            // If the media playback has reached the end.
            if (mediaAtEnd)
            {
                // Reset the playback position to the start.
                Player->setPosition(0);
                // Update the mediaAtEnd flag to indicate playback is no longer at the end.
                mediaAtEnd = false;
            }
            // Update the is_Play flag to indicate media is no longer playing.
            is_Play = false;
            // Play the media.
            Player->play();
            // Update the button icon to show the pause symbol.
            ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        }
        else // If the media is not currently playing.
        {
            // Update the is_Play flag to indicate media is now playing.
            is_Play = true;
            // Pause the media.
            Player->pause();
            // Update the button icon to show the play symbol.
            ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        }
    }
    // If media is not loaded, no action is taken.
}


// Handles the action of clicking the mute button
void MainWindow::on_pushButton_Mute_clicked()
{
    // Static variable to remember the last volume level before muting
    static int lastVolume = 0;

    // Check if the media is currently not muted
    if (!is_Muted)
    {
        // Set the is_Muted flag to true indicating the media is now muted
        is_Muted = true;
        // Update the mute button icon to show a muted symbol
        ui->pushButton_Mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        // Store the current volume level before muting
        lastVolume = ui->horizontalSlider_Volume->value();
        // Set the volume slider to 0 to indicate no volume
        ui->horizontalSlider_Volume->setValue(0);
        // Mute the media player
        Player->setMuted(true);
    }
    else // If the media is currently muted
    {
        // Set the is_Muted flag to false indicating the media is now unmuted
        is_Muted = false;
        // Update the mute button icon to show a volume symbol
        ui->pushButton_Mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        // Restore the volume level to its previous state before muting
        ui->horizontalSlider_Volume->setValue(lastVolume);
        // Unmute the media player
        Player->setMuted(false);
    }
}

// Handles the action of changing the volume through the volume slider
void MainWindow::on_horizontalSlider_Volume_valueChanged(int value)
{
    // Set the media player's volume to the new value
    Player->setVolume(value);

    // If volume is raised from 0, unmute
    if(value > 0)
    {
        // Set the is_Muted flag to false as the volume is above 0
        is_Muted = false;
        // Ensure the media player is not muted
        Player->setMuted(false);
        // Update the mute button icon to show a volume symbol
        ui->pushButton_Mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    }
    // If volume is dropped to 0, mute
    else if(value == 0)
    {
        // Set the is_Muted flag to true as the volume is 0
        is_Muted = true;
        // Ensure the media player is muted
        Player->setMuted(true);
        // Update the mute button icon to show a muted symbol
        ui->pushButton_Mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    }
}


// Handles the action of clicking the screenshot button
void MainWindow::on_pushButton_Screenshot_clicked()
{
    // Check if media is loaded before taking a screenshot
    if (mediaIsLoaded)
    {
        // If the media is currently playing, pause it to take a screenshot
        if (Player->state() == QMediaPlayer::PlayingState)
        {
            // Update the is_Play flag to reflect the paused state
            is_Play = true;
            // Pause the media playback
            Player->pause();
            // Update the play/pause button icon to show the play symbol
            ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        }

        // Open a file dialog for the user to choose where to save the screenshot
        QString screenshotFile = QFileDialog::getSaveFileName(this, tr("Save Screenshot"), "", tr("PNG Files (*.png);;JPEG Files (*.jpg *.jpeg)"));

        // Check if a file path was selected
        if (!screenshotFile.isEmpty())
        {
            // Create the parameters for the ffmpeg command to take a screenshot
            QStringList parameters;
            parameters << "-y" << "-i" << TempProjectFile << "-ss" << currentTime.toString(timeFormat).left(7) << "-vframes" << "1" << screenshotFile;
            // Execute ffmpeg with the specified parameters to save the screenshot
            QProcess::execute("ffmpeg", parameters);
        }
    }
}

// Side menu Add Audio button
void MainWindow::on_pushButton_Audio_clicked()
{
    // If a video is loaded
    if (mediaIsLoaded)
    {
        // Make the audio adding control visible
        setTrimElementVisibility(false);
        setAudioElementVisibility(true);
    }
}

// Handles the action of clicking the Add Audio button
void MainWindow::on_pushButton_Add_Audio_clicked()
{
    // Open a file dialog to select an MP3 audio file
    QString audioFile = QFileDialog::getOpenFileName(this, tr("Select Audio File"), "", tr("MP3 Files (*.mp3)"));

    // Check if an audio file was selected
    if (!audioFile.isEmpty())
    {
        // Determine durations of video and audio
        double videoDuration = getMediaDuration(TempProjectFile);
        double audioDuration = getMediaDuration(audioFile);

        // If the audio duration is longer than the video, trim the audio to match video duration
        if (audioDuration > videoDuration)
        {
            audioFile = trimAudioFile(audioFile, videoDuration);
        }

        // Create a new temporary file for the clip with added audio
        newTempFile();
        QStringList parameters;

        // If the start time is at the beginning of the video
        if (startTime == QTime(0,0,0))
        {
            // Set parameters for ffmpeg command to mix original audio and new audio from the start
            parameters << "-y" << "-i" << TempProjectFile << "-i" << audioFile << "-filter_complex" << "[0:a][1:a]amix=inputs=2:duration=first:dropout_transition=2"
                       << "-c:v" << "copy" << TempOutputFile;
        }
        // If the start time is set to a specific time within the video
        else if (startTime < endTime)
        {
            // Calculate delay to align audio with the start time
            int startTimeInMilliseconds = QTime(0,0).msecsTo(startTime);
            QString delay = QString::number(startTimeInMilliseconds) + "|" + QString::number(startTimeInMilliseconds); // Delay for both channels
            // Set parameters for ffmpeg command to mix original audio and new audio with delay
            parameters << "-y" << "-i" << TempProjectFile << "-i" << audioFile << "-filter_complex" <<
                QString("[1:a]adelay=%1[aud];[0:a][aud]amix=inputs=2:duration=first:dropout_transition=2").arg(delay) << "-c:v" << "copy" << TempOutputFile;
        }

        // Create a process to execute the ffmpeg command and add the audio to the video
        QProcess *audioProcess = new QProcess(this);
        startProcess(audioProcess, parameters);
    }
}

// Function to get the duration of a media file
double MainWindow::getMediaDuration(const QString &mediaFile)
{
    // Create a process to execute ffprobe for fetching media duration
    QProcess processForDuration;
    QStringList durationParameters;

    // Setting up parameters for ffprobe to get the duration of the media file
    durationParameters << "-v" << "error" << "-show_entries" << "timeFormat=duration" << "-of" << "default=noprint_wrappers=1:nokey=1" << mediaFile;
    // Start the ffprobe process with the specified parameters
    processForDuration.start("ffprobe", durationParameters);
    // Wait for the process to finish
    processForDuration.waitForFinished();
    // Read the output of the process which contains the duration
    QString durationString = processForDuration.readAllStandardOutput();

    // Convert the duration string to a double and return it
    return durationString.toDouble();
}

// Function to trim the audio file to match the video duration
QString MainWindow::trimAudioFile(const QString &audioFile, double videoDuration)
{
    // Define the file path for the trimmed audio file
    QString trimmedAudioFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/trimmed_audio.mp3";

    // Create a process to execute ffmpeg for trimming the audio file
    QProcess processForTrim;
    QStringList parametersForTrim;

    // Setting up ffmpeg parameters to trim the audio file to the specified duration
    parametersForTrim << "-y" << "-i" << audioFile << "-t" << QString::number(videoDuration) << trimmedAudioFile;
    // Start the ffmpeg process with the specified parameters
    processForTrim.start("ffmpeg", parametersForTrim);
    // Wait for the process to finish
    processForTrim.waitForFinished();

    // Return the file path of the trimmed audio file
    return trimmedAudioFile;
}

// Function to handle the completion of video processing operations (like adding audio, trimming, etc.)
void MainWindow::onTempVideoUpdated(int exitCode, QProcess::ExitStatus exitStatus, QProcess *process)
{
    // Check if the process completed successfully
    if (exitCode == 0 && exitStatus == QProcess::NormalExit)
    {
        // Clear the current media from the player
        Player->setMedia(QMediaContent());
        // Remove the old temp project file
        QFile::remove(TempProjectFile);
        // Copy the newly created output file to the temp project file path
        QFile::copy(TempOutputFile, TempProjectFile);

        // Load the updated video into the player
        Player->setMedia(QUrl::fromLocalFile(TempProjectFile));

        // Set the flag indicating that the project has unsaved changes
        isSaved = false;
    }
    else // If the process did not complete successfully
    {
        // Show a warning message to the user about the error
        QMessageBox::warning(this, tr("Error"), tr("An error occured and the process was unable to be completed."));
    }

    // Log the exit code and status of the ffmpeg process
    qDebug() << "ffmpeg process finished with exit code" << exitCode << "and exit status" << exitStatus;

    // If there was an error, log the standard error output from the process
    if (exitCode != 0)
    {
        QString errorMessage = process->readAllStandardError();
        qDebug() << "ffmpeg process failed with error:" << errorMessage;
    }

    // If the process did not exit normally, log this information
    if (exitStatus != QProcess::NormalExit)
    {
        qDebug() << "ffmpeg process did not exit normally";
    }
}

// Button to open the intro creation window
void MainWindow::on_pushButton_Intro_clicked()
{
    Intro_Outro intro(true, this);
    intro.exec();
}

// Button to open the outro creation window
void MainWindow::on_pushButton_Outro_clicked()
{
    Intro_Outro outro(false, this);
    outro.exec();
}

// Handles the action of clicking the Append Video button
void MainWindow::on_pushButton_Append_Video_clicked()
{
    // Check if media is already loaded in the editor
    if (mediaIsLoaded)
    {
        // Open a file dialog to select a video file to append
        QString videoFile = QFileDialog::getOpenFileName(this, tr("Select Video File"), "", tr("Video Files (*.mp4 *.avi *.mkv *.mov *.wmv);;All Files (*)"));

        // Check if a video file was selected
        if (!videoFile.isEmpty())
        {
            // Check if the selected video file is in a valid format and compatible
            if (videoIsValid(videoFile))
            {
                // Append the selected video file to the current project
                addVideo(videoFile, true);
            }
        }
    }
    else // If no media is currently loaded
    {
        // Trigger the action to import a new video file
        on_action_Import_triggered();
    }
}

// Handles the action of clicking the Prepend Video button
void MainWindow::on_pushButton_Prepend_Video_clicked()
{
    // Check if media is already loaded in the editor
    if (mediaIsLoaded)
    {
        // Open a file dialog to select a video file to prepend
        QString videoFile = QFileDialog::getOpenFileName(this, tr("Select Video File"), "", tr("Video Files (*.mp4 *.avi *.mkv *.mov *.wmv);;All Files (*)"));

        // Check if a video file was selected
        if (!videoFile.isEmpty())
        {
            // Check if the selected video file is in a valid format and compatible
            if (videoIsValid(videoFile))
            {
                // Prepend the selected video file to the current project
                addVideo(videoFile, false);
            }
        }
    }
    else // If no media is currently loaded
    {
        // Trigger the action to import a new video file
        on_action_Import_triggered();
    }
}

// Handles the action of adding a new video
void MainWindow::addVideo(const QString &newVideoFile, bool append)
{
    QProcess *process = new QProcess(this);
    QStringList parameters, parametersA;
    newTempFile(); // Create a new temp file so that the added video can be undone

    if (append) // Append new video file
    {
        parameters << "-y" << "-i" << TempProjectFile << "-i" << newVideoFile << "-filter_complex"
                   << "[0:v]scale=1920:1080[v0];[1:v]scale=1920:1080[v1];[v0][0:a][v1][1:a] concat=n=2:v=1:a=1 [outv][outa]" // Scales to 1080p so that the videos can concat
                   << "-map" << "[outv]" << "-map" << "[outa]" << "-c:v" << "libx264" << "-preset" << "ultrafast" << "-fps_mode" << "vfr" << TempOutputFile;
    }
    else // Prepend new video file
    {
        parameters << "-y" << "-i" << newVideoFile << "-i" << TempProjectFile << "-filter_complex"
                   << "[0:v]scale=1920:1080[v0];[1:v]scale=1920:1080[v1];[v0][0:a][v1][1:a] concat=n=2:v=1:a=1 [outv][outa]" // Scales to 1080p so that the videos can concat
                   << "-map" << "[outv]" << "-map" << "[outa]" << "-c:v" << "libx264" << "-preset" << "ultrafast" << "-fps_mode" << "vfr" << TempOutputFile;
    }

    // Start the process
    startProcess(process, parameters);
}

// Function to check if the video file contains an audio stream
bool MainWindow::videoHasAudio(const QString &videoFile)
{
    // Create a process to execute ffprobe to check for audio streams
    QProcess processForStreams;
    QStringList streamParameters;

    // Setting up ffprobe parameters to detect audio streams in the video file
    streamParameters << "-select_streams" << "a" << "-show_entries" << "stream=codec_name" << "-of" << "default=noprint_wrappers=1:nokey=1" << videoFile;
    // Start the ffprobe process with the specified parameters
    processForStreams.start("ffprobe", streamParameters);
    // Wait for the process to finish
    processForStreams.waitForFinished();
    // Read the output of the process
    QString streamOutputString = processForStreams.readAllStandardOutput();

    // Return true if the output string is not empty, indicating the presence of an audio stream
    return !streamOutputString.isEmpty();
}

// Handles the action of clicking the Trim button
void MainWindow::on_pushButton_Trim_clicked()
{
    // Check if media is loaded before enabling trim functionality
    if (mediaIsLoaded)
    {
        // Hide the audio element UI and show the trim element UI
        setAudioElementVisibility(false);
        setTrimElementVisibility(true);
        // Initialize the start time to the beginning and end time to the total duration of the video
        startTime = QTime(0,0,0);
        endTime = totalTime;
    }
}

// Handles the action of setting the start time for trimming
void MainWindow::on_pushButton_Trim_Start_clicked()
{
    // Set the start time for trimming to the current time in the video
    startTime = currentTime;

    // If the start time is before the end time, update the start time label
    if (startTime < endTime)
    {
        ui->label_start_Time->setText(currentTime.toString(timeFormat).left(7));
    }
    else // If the start time is after the end time, reset the end time and update both labels
    {
        endTime = totalTime;
        ui->label_start_Time->setText(currentTime.toString(timeFormat).left(7));
        ui->label_end_Time->setText(endTime.toString(timeFormat).left(7));
    }
}

// Handles the action of setting the end time for trimming
void MainWindow::on_pushButton_Trim_End_clicked()
{
    // Set the end time for trimming to the current playback time
    endTime = currentTime;

    // If the end time is after the start time, update the end time label
    if (endTime > startTime)
    {
        ui->label_end_Time->setText(currentTime.toString(timeFormat).left(7));
    }
    else // If the end time is before the start time, reset the start time and update both labels
    {
        startTime = QTime(0,0,0);
        ui->label_start_Time->setText(startTime.toString(timeFormat).left(7));
        ui->label_end_Time->setText(currentTime.toString(timeFormat).left(7));
    }
}

// Handles the action of clicking the Trim Save button
void MainWindow::on_pushButton_Trim_Save_clicked()
{
    // Define the file path for the temporary output file after trimming
    TempOutputFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/temp_output.mp4";

    // Create a new process for the trimming operation
    QProcess *trimProcess = new QProcess(this);
    QStringList trimParameters;

    // Create a new temp file to store the trimmed video
    newTempFile();

    // Set up the ffmpeg parameters for trimming the video between the start and end times
    trimParameters << "-i" << TempProjectFile << "-ss" << startTime.toString(timeFormat).left(7) << "-to" << endTime.toString(timeFormat).left(7)
                   << "-c" << "copy" << TempOutputFile;

    // Start the trimming process with the specified parameters
    startProcess(trimProcess, trimParameters);
}

// Handles the action of clicking the Trim Remove button
void MainWindow::on_pushButton_Trim_Remove_clicked()
{
    // Ensure the user does not remove the entire video unless desired
    if (startTime == QTime(0,0,0) && endTime == totalTime)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Warning", "You are about to remove the entire video. Are you sure you want to continue?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
           removeVideo();
        }
    }
    else
    {
        newTempFile(); // Create a new temp file to store the edited video
        QProcess *trimProcess = new QProcess(this);
        QStringList trimParametersA, trimParametersB, trimParameters;

        // Removing a middle portion of the video
        if (startTime > QTime(0,0,0) && endTime < totalTime) {
            QString TempOutputFileA = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/temp_outputA.mp4";
            QString TempOutputFileB = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/temp_outputB.mp4";

            trimParametersA << "-y" << "-i" << TempProjectFile << "-to" << startTime.toString(timeFormat).left(7) << "-c:v" << "libx264" << "-preset" << "ultrafast" << TempOutputFileA;
            trimParametersB << "-y" << "-ss" << endTime.toString(timeFormat).left(7) << "-i" << TempProjectFile << "-c:v" << "libx264" << "-preset" << "ultrafast" << TempOutputFileB;

            QProcess::execute("ffmpeg", trimParametersA);
            QProcess::execute("ffmpeg", trimParametersB);

            trimParameters << "-y" << "-i" << TempOutputFileA << "-i" << TempOutputFileB << "-filter_complex" << "[0:v][0:a][1:v][1:a] concat=n=2:v=1:a=1 [outv][outa]"
                           << "-map" << "[outv]" << "-map" << "[outa]" << TempOutputFile;
        }

        // Removing a starting portion of the video
        else if (startTime == QTime(0,0,0) && endTime < totalTime)
        {
            trimParameters << "-ss" << endTime.toString(timeFormat).left(7) << "-i" << TempProjectFile << "-c" << "copy" << "-ss" << "00:00:00" << TempOutputFile;
        }

        // Removing an end portion of the video
        else if (startTime > QTime(0,0,0) && endTime == totalTime)
        {
            trimParameters << "-i" << TempProjectFile << "-t" << startTime.toString(timeFormat).left(7) << "-c" << "copy" << TempOutputFile;
        }

        // Start the process
        startProcess(trimProcess, trimParameters);
    }
}

// Toggles trim element visibility
void MainWindow::setTrimElementVisibility(bool visible)
{
    ui->pushButton_Trim_Start->setVisible(visible);
    ui->pushButton_Trim_End->setVisible(visible);
    ui->label_start_Time->setVisible(visible);
    ui->label_end_Time->setVisible(visible);
    ui->pushButton_Trim_Save->setVisible(visible);
    ui->pushButton_Trim_Remove->setVisible(visible);
}

// Toggles audio element visibility
void MainWindow::setAudioElementVisibility(bool visible)
{
    ui->pushButton_Trim_Start->setVisible(visible);
    ui->label_start_Time->setVisible(visible);
    ui->pushButton_Add_Audio->setVisible(visible);
}

// Creates a new temp file to hold an edit that was made, for undoing/redoing
void MainWindow::newTempFile()
{
    // Increment the count of temporary files to maintain uniqueness
    tempFileCount++;
    // Update the current file index to the latest temporary file
    currentFile = tempFileCount;
    // Construct the file path for the new temporary output file with the updated count
    // This ensures each edit has a unique temporary file associated with it
    TempOutputFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/temp_output" + QString::number(tempFileCount) + ".mp4";
}

// Menu bar action to undo a change that was made
void MainWindow::on_action_Undo_triggered()
{
    // Check if there is a previous edit to revert to
    if (currentFile > 0)
    {
        // Decrement the current file index to revert to the previous edit
        currentFile--;
        // Update the TempOutputFile path to point to the previous temporary file
        TempOutputFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/temp_output" + QString::number(currentFile) + ".mp4";

        // Clear the current media from the player
        Player->setMedia(QMediaContent());
        // Remove the current project file
        QFile::remove(TempProjectFile);
        // Copy the previous edit (temporary file) back to the project file location
        QFile::copy(TempOutputFile, TempProjectFile);

        // Load the undone edit into the media player
        Player->setMedia(QUrl::fromLocalFile(TempProjectFile));
    }
    // If there is no previous edit, no action is taken
}

// Menu bar action to redo a change that was made
void MainWindow::on_action_Redo_triggered()
{
    // Check if there is an edit available to redo
    if (currentFile < tempFileCount)
    {
        // Increment the current file index to move to the next edit
        currentFile++;
        // Update the TempOutputFile path to point to the next temporary file
        TempOutputFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/temp_output" + QString::number(currentFile) + ".mp4";

        // Clear the current media from the player
        Player->setMedia(QMediaContent());
        // Remove the current project file
        QFile::remove(TempProjectFile);
        // Copy the next edit (temporary file) back to the project file location
        QFile::copy(TempOutputFile, TempProjectFile);

        // Load the redone edit into the media player
        Player->setMedia(QUrl::fromLocalFile(TempProjectFile));
    }
    // If there is no previous edit, no action is taken
}

// Handles the starting of a process and the loading bar starting
void MainWindow::startProcess(QProcess* process, const QStringList &parameters)
{
    // Start the ffmpeg process with the provided parameters
    process->start("ffmpeg", parameters);

    // Show progress bar and center it
    progressBar->show();
    progressBar->move(ui->groupBox_Video->width()/2,
                      ui->groupBox_Video->height()/2);

    progressBar->startProgress(); // Start the progress bar

    // Connect the process's finished signal to a lambda function.
    // When the process finishes, it emits the finished signal with an exit code and an exit status.
    // The lambda function then calls processFinished with these parameters plus the process itself.
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus) {
            processFinished(exitCode, exitStatus, process);
        });
}

// Handles the finishing of a process and the loading bar stopping
void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus, QProcess *process)
{
    progressBar->stopProgress(); // Stop the progress bar

    // Delay hiding the progress bar so the user can see it finish
    QTimer::singleShot(500, [this]() {
        //setProgressBarVisibility(false); // Hide the progress bar when finished
        progressBar->hide();
    });

    onTempVideoUpdated(exitCode, exitStatus, process);
}

// Menu bar option to enable Light mode
void MainWindow::on_actionLight_mode_triggered()
{
    qApp->setStyleSheet("QMainWindow { background-color: #FAFAFA; }"
                        "QMainWindow QWidget { color: #000000; }"
                        "QMenuBar { background-color: #F0F0F0; color: #000000; }"
                        "QMenuBar::item:selected { color: #000000; }"
                        "QMenu:item { background-color: #F0F0F0; color: #000000; }"
                        "QMenu::item:selected { background-color: #87C8FF; }"
                        "QPushButton { background-color: #E4E4E4; color: #000000; font-size: 9pt; font-family: Segoe UI; }"
                        "QDialog { background-color: #FAFAFA; }"
                        "QListWidget { background-color: #FAFAFA; }"
                        "QTextEdit { background-color: #FAFAFA; }"
                        "QSpinBox { background-color: #E4E4E4; }");

    progressBar->setLightMode();

    settings.setValue("mode", "light");
}

// Menu bar option to enable Dark mode
void MainWindow::on_actionDark_mode_triggered()
{
    qApp->setStyleSheet("QMainWindow { background-color: #363636; }"
                        "QMainWindow QWidget { color: #FFFFFF; }"
                        "QMenuBar { background-color: #2A2A2A; color: #FFFFFF; }"
                        "QMenuBar::item:selected { color: #000000; }"
                        "QMenu:item { background-color: #F0F0F0; color: #000000; }"
                        "QMenu::item:selected { background-color: #87C8FF; }"
                        "QPushButton { background-color: #555555; color: #FFFFFF; font-size: 9pt; font-family: Segoe UI; }"
                        "QDialog { background-color: #363636; }"
                        "QListWidget { background-color: #363636; }"
                        "QTextEdit { background-color: #363636; }"
                        "QSpinBox { background-color: #555555; }");

    progressBar->setDarkMode();

    settings.setValue("mode", "dark");
}

// Handles the complete removal of a video from Video Trim
void MainWindow::removeVideo()
{
    Player->setMedia(QMediaContent());
    Video->setVisible(false);
    totalTime = QTime(0, vidDuration / (60 * 1000), (vidDuration / 1000) % 60, vidDuration % 1000);
    ui->label_total_Time->setText(totalTime.toString(timeFormat).left(7));
    ui->label_Import_Instructions->setVisible(true);
    ui->label_Import_Icon->setVisible(true);
    mediaIsLoaded = false;
    isSaved = true;
}

// Handles the action of loading the light/dark mode settings
void MainWindow::loadSettings()
{
    QString mode = settings.value("mode", "light").toString();

    if (mode == "light")
        on_actionLight_mode_triggered();
    else if (mode == "dark")
        on_actionDark_mode_triggered();
}

// Handles the action of converting a valid video file to MP4
QString MainWindow::convertToMP4(const QString &videoFile, bool isButton)
{
    // Create a temporary file for the conversion
    tempFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/temp_file.mp4";

    // Convert to mp4
    QStringList parameters;
    parameters << "-y" << "-i" << videoFile << "-codec" << "copy" << tempFile;
    QProcess::execute("ffmpeg", parameters);

    // If the conversion was prompted by the user using the button
    if (isButton)
    {
        // Prompt user to save the converted file
        QString conversionFile = QFileDialog::getSaveFileName(this, tr("Save Video File"), "", tr("MP4 Files (*.mp4)"));
        if (!conversionFile.isEmpty())
        {
            // Copy temporary file to the user-chosen location
            QFile::copy(tempFile, conversionFile);
        }
        // Remove temporary file
        QFile::remove(tempFile);
    }

    return tempFile;
}

// Identifies the file type of a video file
QString MainWindow::fileType(const QString &videoFile)
{
    // Get the video file timeFormat suffix
    QString suffix = QFileInfo(videoFile).suffix().toLower();

    // Returns file type suffix
    return suffix;
}
