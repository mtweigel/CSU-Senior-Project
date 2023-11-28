#include "tutorial.h"
#include "ui_tutorial.h"

Tutorial::Tutorial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tutorial)
{
    ui->setupUi(this);
    setup();
}

Tutorial::~Tutorial()
{
    delete ui;
}

void Tutorial::setup()
{
    this->setFixedSize(750, 565);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(ui->listWidget, &QListWidget::currentItemChanged, this, &Tutorial::onFeatureSelected);
}

void Tutorial::onFeatureSelected(QListWidgetItem *current)
{
    // Update the QTextEdit with the description of the currently selected feature
    if (current->text() == "Importing a Video")
    {
        ui->textEdit->setText("<h2>Importing a Video</h2>"
                              "<style> p { font-size: 12px; } </style>"
                              "<p>This feature allows you to import a video into the project. "
                              "To import a video, you can use the <strong>File &gt; Import</strong> menu option or simply drag and drop the video file into the video player.</p>"
                              "<p>Supported formats include MP4, AVI, MKV, MOV, and WMV.</p>");
    }
    else if (current->text() == "Saving the Project")
    {
        ui->textEdit->setText("<h2>Saving the Project</h2>"
                              "<style> p { font-size: 12px; } </style>"
                              "<p>You can save your current project by going to the <strong>File &gt; Save</strong> or <strong>Save As</strong> menu option. "
                              "You can also use the shortcut <strong>Ctrl + S</strong>.</p>"
                              "<p>Remember to save frequently to prevent loss of work!</p>");
    }
    else if (current->text() == "Using the Video Player")
    {
        ui->textEdit->setText("<h2>Using the Video Player</h2>"
                              "<style> p { font-size: 12px; } </style>"
                              "<p>Once you have imported a video, you can play the video by pressing the Play <img src=':/icons/play.png'> "
                              "button on the video player. While the video is playing, you can pause the video by pressing the Pause <img src=':/icons/pause.png'> button.</p>"
                              "<p> To skip forwards or backwards, click or hold down the Seek Forward <img src=':/icons/seek_forward.png'> and Seek Backward "
                              "<img src=':/icons/seek_backward.png'> buttons.</p>"
                              "<p> To change the volume of the video, move the volume slider left or right. To mute and unmute the video, press the "
                              "Volume <img src=':/icons/volume.png'> button or drag the volume to 0.</p>"
                              "<p> Anytime you would like to quickly change the video position, you can drag the video duration slider to the left or right.</p>");
    }
    else if (current->text() == "Adding Video")
    {
        ui->textEdit->setText("<h2>Adding Addtional Video</h2>"
                              "<style> p { font-size: 12px; } </style>"
                              "<p>You can add additional video clips to your video project. To add a new video to the end of your project, click the <strong>Append Video</strong> "
                              "button. To add a new video to the beginning of your project, click the <strong>Prepend Video</strong> button.</p>");
    }
    else if (current->text() == "Adding Intro/Outro")
    {
        ui->textEdit->setText("<h2>Adding Intro/Outro</h2>"
                              "<style> p { font-size: 12px; } </style>"
                              "<p>To add an intro or outro to your video, press the <strong>Add Intro</strong> or <strong>Add Outro</strong> buttons to bring up an editor where"
                              " you are able to add text, customize the font size, style, and color, background color, and duration of your intro or outro. While you are editing "
                              "each part, there is a preview window that shows what the intro/outro will look like.</p>"
                              "<p><strong>Note:</strong> The current design of this project allows prepended videos to come before an intro, and appended videos to come after an outro."
                              "This can be used to create transition scenes, but keep this in mind when adding your intro/outro.</p>"
                              "<p>Intro/Outro editor:</p><p><img src=':/icons/intro_editor.png'></p>"
                              "<p>Customizing font:</p><p><img src=':/icons/edit_font.png'></p>"
                              "<p>Changing color:</p><p><img src=':/icons/edit_color.png'></p");
    }
    else if (current->text() == "Trimming Video")
    {
        ui->textEdit->setText("<h2>Trimming Video</h2>"
                              "<style> p { font-size: 12px; } </style>"
                              "<p>To begin trimming your video project, press the <strong>Trim Video</strong> button. Next, choose both the start and end points of the "
                              "portion that you want to trim by moving the video duration slider and clicking the <strong>Start Point</strong> and <strong>End Point</strong> "
                              "buttons. Once those points have been chosen, you have two options.</p>"
                              "<p><strong>Save</strong>: This option saves the selected portion as the new video and removes the rest of the video.</p>"
                              "<p><strong>Remove</strong>: This option removes the selected portion from the video and keeps the rest of the video.</p>");
    }
    else if (current->text() == "Adding Music/Audio")
    {
        ui->textEdit->setText("<h2>Adding Music/Audio</h2>"
                              "<style> p { font-size: 12px; } </style>"
                              "<p>This feature allows you to add music or other audio to your video project. To do so, click the <strong>Background Music</strong> button and choose "
                              "the audio from your computer.</p>");
    }
    else if (current->text() == "Undoing Changes")
    {
        ui->textEdit->setText("<h2>Undoing and Redoing Changes</h2>"
                              "<style> p { font-size: 12px; } </style>"
                              "<p>This feature allows you to undo any changes that you have made to your video project. To undo a change, go to <strong>Edit &gt; Undo</strong> "
                              "<img src=':/icons/icons8-undo-24.png'> or use the shortcut <strong>Ctrl + Z</strong>.</p>"
                              "<p>To redo any changes that you have undone, go to <strong>Edit &gt; Redo</strong> <img src=':/icons/icons8-redo-24.png'> or use the shortcut "
                              "<strong>Ctrl + Y</strong>.</p>");
    }
    else if (current->text() == "Screen Capture")
    {
        ui->textEdit->setText("<h2>Screen Capture</h2>"
                              "<style> p { font-size: 12px; } </style>"
                              "<p>This feature allows you to take a screenshot of the current video position by pressing the <strong>Take Screenshot</strong> button, choosing a name "
                              "for the image, and saving it.</p>");
    }
    else if (current->text() == "Appearance Settings")
    {
        ui->textEdit->setText("<h2>Appearance Settings</h2>"
                              "<style> p { font-size: 12px; } </style>"
                              "<p>Customize the appearance of the application between light and dark mode by going to <strong>Option &gt; Theme</strong> and choosing between light and dark "
                              "mode. This customization will remain persistent between sessions.</p>"
                              "<p><img src=':/icons/dark_mode.png'></p>");
    }
    else if (current->text() == "Converting to MP4")
    {
        ui->textEdit->setText("<h2>Converting to MP4</h2>"
                              "<style> p { font-size: 12px; } </style>"
                              "<p>This feature allows the format conversion of any video that is .AVI, .MKV, .MOV, or .WMV to .MP4. To convert a video, press the "
                              "<strong>Convert to MP4</strong> button, choose a video to convert, and then save the newly converted video.</p>");
    }
    else if (current->text() == "Exiting the Program")
    {
        ui->textEdit->setText("<h2>Exiting the Program</h2>"
                              "<style> p { font-size: 12px; } </style>"
                              "<p>To exit FlickFix, you can either go to <strong>File &gt; Exit</strong> or hit the red X in the upper right corner of the program.</p>");
    }
}

void Tutorial::on_pushButton_Close_clicked()
{
    accept();
}

