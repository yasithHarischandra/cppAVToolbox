// -------------------------------------------------------
// File: src/core/ConfigManager.h
// -------------------------------------------------------
#pragma once
#include <QString>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>

/**
 * Singleton to manage configuration (e.g. tool paths).
 */
class ConfigManager {
public:
    static ConfigManager& instance() {
        static ConfigManager mgr;
        return mgr;
    }

    /**
     * Get the configured path for a given tool name, or default to toolName.
     */
    QString toolPath(const QString& toolName) const {
        return _settings.value("tools/" + toolName + "/path", toolName).toString();
    }

    /**
     * Set and persist the path for a given tool.
     */
    void setToolPath(const QString& toolName, const QString& path) {
        _settings.setValue("tools/" + toolName + "/path", path);
    }

private:
    ConfigManager();
    QSettings _settings;
};

