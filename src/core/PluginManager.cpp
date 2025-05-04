// core/PluginManager.cpp
#include "PluginManager.h"
#include "IConsoleToolPlugin.h"
// include other plugins...

PluginManager& PluginManager::instance() {
    static PluginManager inst;
    return inst;
}

void PluginManager::loadPlugin(IConsoleToolPlugin* plugin) {
    // Hard-coded plugin creation for now (can be dynamic with Qt plugins)
    _plugins.append(plugin);
    // _plugins.append(new TSDuckPlugin());
}

QList<IConsoleToolPlugin*> PluginManager::allPlugins() const {
    return _plugins;
}

IConsoleToolPlugin* PluginManager::pluginById(const QString& id) const {
    for (auto* plugin : _plugins) {
        if (plugin->name() == id)
            return plugin;
    }
    return nullptr;
}
