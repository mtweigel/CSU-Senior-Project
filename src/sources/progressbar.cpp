#include "ProgressBar.h"
#include "ui_ProgressBar.h"

ProgressBar::ProgressBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgressBar),
    progressTimer(new QTimer(this)) // Initialize the timer here
{
    ui->setupUi(this);
    connect(progressTimer, &QTimer::timeout, this, &ProgressBar::updateProgress);
}

ProgressBar::~ProgressBar()
{
    delete ui;
    delete progressTimer; // Don't forget to delete the timer
}

// Starts the progress bar progress
void ProgressBar::startProgress()
{
    ui->progressBar->setValue(0); // Reset the progress bar value
    progressTimer->start(100); // Start the timer
}

// Updates the progress bar until it hits 100
void ProgressBar::updateProgress()
{
    int value = ui->progressBar->value();

    if (value < 100)
    {
        ui->progressBar->setValue(value + 1);
    }
    else
    {
        stopProgress();
    }
}

// Stops the progress bar progress
void ProgressBar::stopProgress()
{
    progressTimer->stop();
    ui->progressBar->setValue(100); // Set to complete when stopped
}

// Sets progress bar to Light mode
void ProgressBar::setLightMode()
{
    ui->frame_progressBar->setStyleSheet("background-color: #FAFAFA;");
    ui->label_Loading->setStyleSheet("color: #000000");
}

// Sets progress bar to Dark mode
void ProgressBar::setDarkMode()
{
    ui->frame_progressBar->setStyleSheet("background-color: #363636;");
    ui->label_Loading->setStyleSheet("color: #FFFFFF");
}
