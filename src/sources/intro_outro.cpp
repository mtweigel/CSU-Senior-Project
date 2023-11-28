#include "intro_outro.h"
#include "ui_intro_outro.h"
#include "mainwindow.h"

Intro_Outro::Intro_Outro(bool isIntro, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Intro_Outro),
    mainWin(dynamic_cast<MainWindow*>(parent)),
    isIntro(isIntro)
{
    ui->setupUi(this);
    setup();
}

Intro_Outro::~Intro_Outro()
{
    delete ui;
}

void Intro_Outro::setup()
{
    this->setFixedSize(900, 450);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QPixmap pixmap(ui->label_Image->size());
    pixmap.fill(Qt::white);
    font.setPointSize(10);
    ui->label_Image->setPixmap(pixmap);

    // Connect the text edit changes to update the preview
    connect(ui->textEdit, &QTextEdit::textChanged, this, [&]() {
        updatePreview(ui->label_Image, ui->textEdit->toPlainText(), font, textColor, backgroundColor);
    });
}

void Intro_Outro::on_pushButton_Font_clicked()
{
    bool ok;
    font = QFontDialog::getFont(&ok, font, this); // Pass the current font

    if (ok)
    { // Update the preview only if the user pressed OK
        updatePreview(ui->label_Image, ui->textEdit->toPlainText(), font, textColor, backgroundColor);
    }
}

void Intro_Outro::on_pushButton_Text_Color_clicked()
{
    // Open the QColorDialog with the current textColor as the default and store the result in newColor
    QColor newColor = QColorDialog::getColor(textColor, this);

    // Check if the returned color is valid, meaning the user pressed "OK" and selected a color
    if (newColor.isValid())
    {
        // If the user pressed "OK," update the textColor with the selected color
        textColor = newColor;

        // Update the preview to reflect the new text color
        updatePreview(ui->label_Image, ui->textEdit->toPlainText(), font, textColor, backgroundColor);
    }
    // If the user pressed "Cancel," the newColor will be invalid, and textColor will remain unchanged
}

void Intro_Outro::on_pushButton_Background_clicked()
{
    // Open the QColorDialog with the current backgroundColor as the default and store the result in newColor
    QColor newColor = QColorDialog::getColor(backgroundColor, this);

    // Check if the returned color is valid, meaning the user pressed "OK" and selected a color
    if (newColor.isValid())
    {
        // If the user pressed "OK," update the backgroundColor with the selected color
        backgroundColor = newColor;

        // Update the preview to reflect the new background color
        updatePreview(ui->label_Image, ui->textEdit->toPlainText(), font, textColor, backgroundColor);
    }
    // If the user pressed "Cancel," the newColor will be invalid, and backgroundColor will remain unchanged
}

void Intro_Outro::updatePreview(QLabel *label, const QString &text, const QFont &font, const QColor &textColor, const QColor &backgroundColor)
{
    QImage preview(640, 360, QImage::Format_RGB32);
    preview.fill(backgroundColor);

    QPainter painter(&preview);
    painter.setFont(font);
    painter.setPen(textColor);
    painter.drawText(preview.rect(), Qt::AlignCenter, text);

    label->setPixmap(QPixmap::fromImage(preview));
}

void Intro_Outro::on_buttonBox_accepted()
{
    QString introText = ui->textEdit->toPlainText();
    int duration = ui->spinBox_Duration->value();

    // Create QImage and QPainter to draw the intro/outro card
    QImage introCard(1920, 1080, QImage::Format_RGB32);
    introCard.fill(backgroundColor);

    QPainter painter(&introCard);
    font.setPointSize(font.pointSize() * 2); // Scale the font so that the video matches the preview
    painter.setFont(font);  // Set the selected font (with the size and style) to the painter
    painter.setPen(textColor); // Set the text color
    painter.drawText(introCard.rect(), Qt::AlignCenter, introText);

    // Save the image as a temporary file that auto deletes
    QTemporaryFile tempFile;
    tempFile.open();
    introCard.save(tempFile.fileName(), "PNG");

    QProcess process;
    QStringList parameters;

    if (!mainWin->mediaIsLoaded && isIntro)
    {
        // Use FFmpeg to create a video from the image
        mainWin->newTempFile();  // Create a temp file to hold the video
        QProcess::execute("ffmpeg", QStringList() << "-y" << "-loop" << "1" << "-i" << tempFile.fileName() << "-c:v" << "libx264" << "-t" << QString::number(duration) << "-pix_fmt" << "yuv420p" << "-vf" << "scale=1920:1080" << mainWin->TempOutputFile);

        // Add silent audio track to intro
        mainWin->videoIsValid(mainWin->TempOutputFile);

        // Import the intro
        mainWin->importVideo(mainWin->TempOutputFile);
    }
    else
    {
        QString introOutroFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + (isIntro ? "/intro.mp4" : "/outro.mp4");
        QProcess::execute("ffmpeg", QStringList() << "-y" << "-loop" << "1" << "-i" << tempFile.fileName() << "-c:v" << "libx264" << "-t" << QString::number(duration) << "-pix_fmt" << "yuv420p" << "-vf" << "scale=1920:1080" << introOutroFile);

        // Add silent audio track to intro/outro
        mainWin->videoIsValid(introOutroFile);

        // Prepend or append the intro/outro to the existing video
        if (isIntro)
            mainWin->addVideo(introOutroFile, false); // Prepend video
        else
            mainWin->addVideo(introOutroFile, true); // Append video
    }
}

