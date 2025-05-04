// core/PluginManager.h
#pragma once
#include <QObject>
#include <QList>
#include <QString>
#include <QMap>
#include "IConsoleToolPlugin.h"

class PluginManager {
public:
    static PluginManager& instance();

    void loadPlugin(IConsoleToolPlugin* plugin);
    QList<IConsoleToolPlugin*> allPlugins() const;
    IConsoleToolPlugin* pluginById(const QString& id) const;

private:
    PluginManager() = default;
    QList<IConsoleToolPlugin*> _plugins;
};

