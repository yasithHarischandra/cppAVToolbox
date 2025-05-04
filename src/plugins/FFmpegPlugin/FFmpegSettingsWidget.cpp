// -------------------------------------------------------
// File: src/plugins/FFmpegPlugin/FFmpegSettingsWidget.cpp
// -------------------------------------------------------
#include "FFmpegSettingsWidget.h"
#include "ConfigManager.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

FFmpegSettingsWidget::FFmpegSettingsWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("FFMPEG Settings");
    auto* layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("ffmpeg path:"));

    _pathEdit = new QLineEdit;
    _pathEdit->setText(ConfigManager::instance().toolPath("ffmpeg"));
    layout->addWidget(_pathEdit);

    _browseBtn = new QPushButton("Browse...");
    layout->addWidget(_browseBtn);

    // Update config when editing finishes
    connect(_pathEdit, &QLineEdit::editingFinished, this, [=]() {
        ConfigManager::instance().setToolPath("ffmpeg", _pathEdit->text());
        });

    // Browse file dialog
    connect(_browseBtn, &QPushButton::clicked, this, [=]() {
        QString file = QFileDialog::getOpenFileName(this, "Select ffmpeg executable", _pathEdit->text(), "Executables (*)");
        if (!file.isEmpty()) {
            _pathEdit->setText(file);
            ConfigManager::instance().setToolPath("ffmpeg", file);
        }
        });
};
