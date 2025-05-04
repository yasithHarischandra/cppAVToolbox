// src/plugins/FFmpegPlugin/MediaInfoWidget.cpp
#include "MediaInfoWidget.h"
#include "ConfigManager.h"
#include "ExternalToolJob.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QPlainTextEdit>

MediaInfoWidget::MediaInfoWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Media Info");
    setMinimumSize(700, 400);
    setupUI();
}

void MediaInfoWidget::setupUI() {
    auto* layout = new QVBoxLayout(this);
    auto* hl = new QHBoxLayout;
    _fileEdit = new QLineEdit;
    _browseBtn = new QPushButton("Browse...");
    _refreshBtn = new QPushButton("Refresh");
    hl->addWidget(_fileEdit);
    hl->addWidget(_browseBtn);
    hl->addWidget(_refreshBtn);
    layout->addLayout(hl);

    _tableView = new QTableView;
    _model = new QStandardItemModel;
    _model->setHorizontalHeaderLabels({
        "Stream #","Type","Codec","Details"
        });
    _tableView->setModel(_model);
    _tableView->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(_tableView);

    // Log output
    _logOutput = new QPlainTextEdit;
    _logOutput->setReadOnly(true);
    layout->addWidget(_logOutput);

    connect(_browseBtn, &QPushButton::clicked, this, &MediaInfoWidget::browseFile);
    connect(_refreshBtn, &QPushButton::clicked, this, &MediaInfoWidget::updateInfo);
}

void MediaInfoWidget::browseFile() {
    QString f = QFileDialog::getOpenFileName(this, "Select Media File");
    if (!f.isEmpty()) {
        _fileEdit->setText(f);
        updateInfo();
    }
}

void MediaInfoWidget::updateInfo() {
    QString file = _fileEdit->text();
    QString output{ "" };
    if (file.isEmpty()) {
        QMessageBox::warning(this, "No file", "Please select a file first.");
        return;
    }
    if (_job) _job->stop();
    QString exe = ConfigManager::instance().toolPath("ffprobe");
    QStringList args = {
        "-v","quiet",
        "-print_format","json",
        "-show_format",
        "-show_streams",
        file
    };
    _job = new ExternalToolJob(this);
    connect(_job, &ExternalToolJob::outputReceived, this, [=](const QString& out) { 
        _logOutput->appendPlainText(out); 
        }
    );
    connect(_job, &ExternalToolJob::finished, this, [=](int c, QProcess::ExitStatus) {
        if (c == 0) {
            QByteArray out = _job->readAllStandardOutput();
            parseAndDisplay(QString::fromUtf8(out));
        }
        });
    _job->start(exe, args);
}

void MediaInfoWidget::parseAndDisplay(const QString& json) {
    _model->removeRows(0, _model->rowCount());
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isObject()) return;
    QJsonObject obj = doc.object();

    // Format info row 0
    if (obj.contains("format")) {
        auto fmt = obj["format"].toObject();
        QString container = fmt.value("format_name").toString();
        double dur = fmt.value("duration").toString().toDouble();
        qint64 size = fmt.value("size").toString().toLongLong();
        QList<QStandardItem*> row;
        row << new QStandardItem("-")
            << new QStandardItem("container")
            << new QStandardItem(container)
            << new QStandardItem(
                QString("duration=%1s size=%2 bytes")
                .arg(dur).arg(size));
        _model->appendRow(row);
    }

    // Streams
    if (obj.contains("streams")) {
        auto arr = obj["streams"].toArray();
        for (auto v : arr) {
            auto s = v.toObject();
            int idx = s.value("index").toInt();
            QString type = s.value("codec_type").toString();
            QString codec = s.value("codec_name").toString();
            QString details;
            if (type == "video") {
                details = QString("res %1x%2 pixfmt=%3 frames=%4")
                    .arg(s.value("width").toInt())
                    .arg(s.value("height").toInt())
                    .arg(s.value("pix_fmt").toString())
                    .arg(s.value("nb_frames").toString());
            }
            else if (type == "audio") {
                details = QString("ch=%1 rate=%2 fmt=%3")
                    .arg(s.value("channels").toInt())
                    .arg(s.value("sample_rate").toString())
                    .arg(s.value("sample_fmt").toString());
            }
            else {
                details = QString("...");
            }
            QList<QStandardItem*> row;
            row << new QStandardItem(QString::number(idx))
                << new QStandardItem(type)
                << new QStandardItem(codec)
                << new QStandardItem(details);
            _model->appendRow(row);
        }
    }
}
