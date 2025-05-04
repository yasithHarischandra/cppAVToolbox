// -------------------------------------------------------
// File: src/plugins/FFmpegPlugin/GenerateMpegTsWidget.cpp
// -------------------------------------------------------
#include "GenerateMpegTsWidget.h"
#include "ConfigManager.h"
#include "ExternalToolJob.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QThreadPool>

GenerateMpegTsWidget::GenerateMpegTsWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Stream MPEG-TS");
    setMinimumSize(600, 400);
    setupUI();
}

void GenerateMpegTsWidget::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);

    // Input file row
    auto* inputLayout = new QHBoxLayout;
    _inputEdit = new QLineEdit;
    _browseInput = new QPushButton("Browse File");
    inputLayout->addWidget(_inputEdit);
    inputLayout->addWidget(_browseInput);
    mainLayout->addLayout(inputLayout);

    // URL and port row
    auto* destLayout = new QHBoxLayout;
    _urlEdit = new QLineEdit;
    _urlEdit->setText("udp://127.0.0.1");
    _portSpin = new QSpinBox;
    _portSpin->setRange(1024, 65535);
    _portSpin->setValue(4037);
    destLayout->addWidget(_urlEdit);
    destLayout->addWidget(_portSpin);
    mainLayout->addLayout(destLayout);

    // Loop checkbox, start and stop buttons
    auto* ctrlLayout = new QHBoxLayout;
    _loopCheck = new QCheckBox("Loop Input");
    _startButton = new QPushButton("Start Streaming");
    _stopButton = new QPushButton("Stop Streaming");
    _stopButton->setEnabled(false);
    ctrlLayout->addWidget(_loopCheck);
    ctrlLayout->addWidget(_startButton);
    ctrlLayout->addWidget(_stopButton);
    mainLayout->addLayout(ctrlLayout);

    // Log output
    _logOutput = new QPlainTextEdit;
    _logOutput->setReadOnly(true);
    mainLayout->addWidget(_logOutput);

    connect(_browseInput, &QPushButton::clicked, this, &GenerateMpegTsWidget::onBrowseInput);
    connect(_startButton, &QPushButton::clicked, this, &GenerateMpegTsWidget::onStartStream);
    connect(_stopButton, &QPushButton::clicked, this, &GenerateMpegTsWidget::onStopStream);
}

bool GenerateMpegTsWidget::validateInputs() {
    QString input = _inputEdit->text();
    QString url = _urlEdit->text();
    if (input.isEmpty() || url.isEmpty()) {
        QMessageBox::warning(this, "Input Required", "Please specify both input file and destination URL.");
        return false;
    }
    if (!QFileInfo::exists(input)) {
        QMessageBox::warning(this, "File Not Found", "The specified input file does not exist.");
        return false;
    }
    return true;
}

void GenerateMpegTsWidget::onBrowseInput() {
    QString file = QFileDialog::getOpenFileName(this, "Select media file");
    if (!file.isEmpty()) {
        _inputEdit->setText(file);
    }
}

void GenerateMpegTsWidget::onStartStream() {
    if (!validateInputs())
        return;

    QString input = _inputEdit->text();
    QString url = _urlEdit->text();
    int port = _portSpin->value();
    bool loop = _loopCheck->isChecked();

    QString exe = ConfigManager::instance().toolPath("ffmpeg");
    QStringList args;
    if (loop)
        args << "-stream_loop" << "-1";
    args << "-re" << "-i" << input;
    args << "-c" << "copy";
    args << "-f" << "mpegts";
    args << QString("%1:%2").arg(url).arg(port);

    // Log command
    QString cmd = exe;
    for (const auto& a : args) cmd += " " + a;
    _logOutput->appendPlainText("Running: " + cmd);

    // Create and start job
    _job = new ExternalToolJob(this);
    connect(_job, &ExternalToolJob::outputReceived, this, [=](const QString& out) { _logOutput->appendPlainText(out); });
    connect(_job, &ExternalToolJob::errorReceived, this, [=](const QString& err) { _logOutput->appendPlainText(err); });
    connect(_job, &ExternalToolJob::finished, this, [=](int code, QProcess::ExitStatus) {
        _logOutput->appendPlainText(code == 0 ? "Streaming stopped." : "Streaming failed.");
        _startButton->setEnabled(true);
        _stopButton->setEnabled(false);
        });

    _startButton->setEnabled(false);
    _stopButton->setEnabled(true);
    _job->start(exe, args, 0);
}

void GenerateMpegTsWidget::onStopStream() {
    if (_job) {
        _job->stop();
        _logOutput->appendPlainText("Stop requested.");
        _stopButton->setEnabled(false);
        _startButton->setEnabled(true);
    }
}
