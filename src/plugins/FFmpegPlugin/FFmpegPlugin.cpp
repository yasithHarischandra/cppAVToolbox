// -------------------------------------------------------
// File: src/plugins/FFmpegPlugin/FFmpegPlugin.cpp
// -------------------------------------------------------
#include "FFmpegPlugin.h"
#include "FFmpegSettingsWidget.h"
#include "CalculateVmafWidget.h"
#include "ConfigManager.h"
#include "ExternalToolJob.h"
#include "VmafResult.h"
#include <QThreadPool>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

/**
 * Returns a settings widget where the user can set the ffmpeg binary path.
 */
QWidget* FFmpegPlugin::createSettingsWidget() {
    // FFmpegSettingsWidget contains a QLineEdit + Browse button
    return new FFmpegSettingsWidget;
}

ExternalToolJob* FFmpegPlugin::createJob(QObject* parent) {
    return new ExternalToolJob(parent);
}

