#include "CalculateVmafWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QProcess>
#include "PluginManager.h"

CalculateVmafWidget::CalculateVmafWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(800, 600);
    setWindowTitle("Calculate VMAF");

    setupUI();
}

void CalculateVmafWidget::setupUI() {
    _refLineEdit = new QLineEdit;
    _distLineEdit = new QLineEdit;
    _refBrowseButton = new QPushButton("Browse...");
    _distBrowseButton = new QPushButton("Browse...");
    _computeButton = new QPushButton("Compute VMAF");
    _outputTextEdit = new QPlainTextEdit;
    _resultLabel = new QLabel("Average VMAF: ");

    _outputTextEdit->setReadOnly(true);

    auto layout = new QVBoxLayout;

    auto refLayout = new QHBoxLayout;
    refLayout->addWidget(new QLabel("Reference Video:"));
    refLayout->addWidget(_refLineEdit);
    refLayout->addWidget(_refBrowseButton);

    auto distLayout = new QHBoxLayout;
    distLayout->addWidget(new QLabel("Distorted Video:"));
    distLayout->addWidget(_distLineEdit);
    distLayout->addWidget(_distBrowseButton);

    layout->addLayout(refLayout);
    layout->addLayout(distLayout);
    layout->addWidget(_computeButton);
    layout->addWidget(_resultLabel);
    layout->addWidget(_outputTextEdit);

    setLayout(layout);

    connect(_refBrowseButton, &QPushButton::clicked, this, &CalculateVmafWidget::onBrowseRef);
    connect(_distBrowseButton, &QPushButton::clicked, this, &CalculateVmafWidget::onBrowseDist);
    connect(_computeButton, &QPushButton::clicked, this, &CalculateVmafWidget::onComputeVmaf);
}

void CalculateVmafWidget::onBrowseRef() {
    QString file = QFileDialog::getOpenFileName(this, "Select Reference Video");
    if (!file.isEmpty()) {
        _refLineEdit->setText(file);
    }
}

void CalculateVmafWidget::onBrowseDist() {
    QString file = QFileDialog::getOpenFileName(this, "Select Distorted Video");
    if (!file.isEmpty()) {
        _distLineEdit->setText(file);
    }
}

void CalculateVmafWidget::onComputeVmaf() {
    QString refFile = _refLineEdit->text();
    QString distFile = _distLineEdit->text();

    if (refFile.isEmpty() || distFile.isEmpty()) {
        QMessageBox::warning(this, "Missing Input", "Please select both reference and distorted videos.");
        return;
    }

    // Get plugin
    auto* plugin = PluginManager::instance().pluginById("ffmpeg");
    if (!plugin) {
        QMessageBox::critical(this, "Plugin Error", "FFmpeg plugin not found.");
        return;
    }

    //set vmaf_output.json file path
    QString distFilePath = distFile;  // already obtained from UI
    QFileInfo distInfo(distFile);
    // 1) build a forward?slash path
    QString logPath = distInfo.absolutePath().replace('\\', '/');
    // logPath == "F:/Yasith/Projects/…/vmaf_output.json"

    // 2) escape the first colon
    QString escPath = logPath;
    escPath.replace(":/", "\\:/");
    // escPath == "F\\:/Yasith/Projects/…/vmaf_output.json"

    // 3) now build the filter
    QString filter = QString("libvmaf=log_path=%1:log_fmt=json").arg(escPath);

    // Get binary path
    QString ffmpegPath = ConfigManager::instance().toolPath("ffmpeg");

    // Prepare args
    QStringList args = {
  "-i", distFile,
  "-i", refFile,
  "-lavfi", "libvmaf=log_path=vmaf_output.json:log_fmt=json",
  "-f", "null", "-"
    };

    QString fullCommand = ffmpegPath;
    for (const QString& arg : args) {
        QString quotedArg = arg;
        quotedArg.replace("\"", "\\\"");
        fullCommand += " \"" + quotedArg + "\"";
    }
    _outputTextEdit->appendPlainText("Running command:\n" + fullCommand);


    // Create job from plugin
    _job = plugin->createJob(this);

    connect(_job, &ExternalToolJob::outputReceived, this, [=](const QString& out) {
        _outputTextEdit->appendPlainText(out);
        });

    connect(_job, &ExternalToolJob::errorReceived, this, [=](const QString& err) {
        _outputTextEdit->appendPlainText(err);
        });

    connect(_job, &ExternalToolJob::finished, this, [=](int code, QProcess::ExitStatus status) {
        if (status == QProcess::NormalExit && code == 0) 
        {
            QString distFile = _distLineEdit->text();
            QFileInfo distInfo(distFile);
            QString logPath = distInfo.absolutePath();

            QFile jsonFile(logPath + "/vmaf_output.json");
            if (jsonFile.open(QIODevice::ReadOnly)) {
                QString json = jsonFile.readAll();
                jsonFile.close();
                VmafResult result = VmafResult::parseFromJson(json);
                _resultLabel->setText(QString("Average VMAF: %1").arg(result.averageScore));
            }
            else {
                QMessageBox::warning(this, "Error", "Could not read vmaf.json file.");
            }
        }
        else {
            QMessageBox::critical(this, "VMAF Error", "ffmpeg exited with an error.");
        }
        });

    _job->start(ffmpegPath, args, distInfo.absolutePath(), 0);
}

