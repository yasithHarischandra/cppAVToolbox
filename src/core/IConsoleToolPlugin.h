// -------------------------------------------------------
// File: src/plugins/IConsoleToolPlugin.h
// -------------------------------------------------------
#pragma once
#include <QString>
#include <QWidget>
#include <QVariantMap>
#include "ExternalToolJob.h"

/**
 * Interface for console-tool plugins.
 * Plugins must implement name(), provide a settings widget,
 * and create jobs given parameter maps.
 */
class IConsoleToolPlugin {
public:
	virtual ~IConsoleToolPlugin() {}

	/**
	 * Unique plugin identifier, e.g. "ffmpeg" or "tsduck".
	 */
	virtual QString name() const = 0;

	/**
	 * Return a QWidget for configuring this plugin (e.g. binary path).
	 * Ownership: caller takes ownership.
	 */
	virtual QWidget* createSettingsWidget() = 0;

	/**
	 * Given a map of parameters, create a Job to perform the tool operation.
	 * Supported keys depend on plugin (e.g. "operation", "ref", "dist").
	 * Ownership: caller takes ownership of the returned Job.
	 */
	virtual ExternalToolJob* createJob(QObject* parent) = 0;
};

#define IConsoleToolPlugin_iid "com.avtoolbox.IConsoleToolPlugin"
Q_DECLARE_INTERFACE(IConsoleToolPlugin, IConsoleToolPlugin_iid)
