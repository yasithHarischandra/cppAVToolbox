// -------------------------------------------------------
// File: src/plugins/FFmpegPlugin/FFmpegPlugin.h
// -------------------------------------------------------
#pragma once
#include "IConsoleToolPlugin.h"
#include <QObject>

/**
 * FFmpegPlugin implements IConsoleToolPlugin for invoking ffmpeg.
 * It provides a settings widget to configure the ffmpeg binary path
 * (via FFmpegSettingsWidget) and creates jobs for operations like VMAF.
 */
class FFmpegPlugin : public QObject, public IConsoleToolPlugin {
	Q_OBJECT
		Q_PLUGIN_METADATA(IID IConsoleToolPlugin_iid)
		Q_INTERFACES(IConsoleToolPlugin)

public:
	// Return unique plugin name
	QString name() const override { return "ffmpeg"; }

	// Create a widget to configure plugin (binary path)
	QWidget* createSettingsWidget() override;

	// Create a job given parameters (e.g. to compute VMAF)
	ExternalToolJob* createJob(QObject* parent) override;
};

