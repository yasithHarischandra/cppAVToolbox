// -------------------------------------------------------
// File: src/core/ConfigManager.cpp
// -------------------------------------------------------
#include "ConfigManager.h"

ConfigManager::ConfigManager()
	: _settings(QDir(QCoreApplication::applicationDirPath()).filePath("settings.ini"), QSettings::IniFormat)
{
	_settings.setFallbacksEnabled(false); // prevent use of system-wide/global config
}