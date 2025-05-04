// src/main.cpp
#include <QApplication>
#include "MainWindow.h"
#include "core/PluginManager.h"
#include "plugins/FFmpegPlugin/FFmpegPlugin.h"

int main(int argc, char** argv) 
{

	PluginManager::instance().loadPlugin(new FFmpegPlugin);

	QApplication app(argc, argv);
	MainWindow w;
	w.show();
	return app.exec();
}
