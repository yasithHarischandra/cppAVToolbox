// src/MainWindow.cpp
#include "MainWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "plugins/FFmpegPlugin/CalculateVmafWidget.h"
#include "plugins/ffmpegplugin/FFmpegSettingsWidget.h"
#include "plugins/FFmpegPlugin/GenerateMpegTsWidget.h"
#include "plugins/FFmpegPlugin/MediaInfoWidget.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
    setupMenuBar();

	auto* central = new QWidget;
	auto* layout = new QVBoxLayout(central);
	layout->addWidget(new QLabel("AVToolbox is Ready"));
	setCentralWidget(central);
    setMinimumSize(800, 600);
	setWindowTitle("AVToolbox");
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);

    // FFmpeg menu
    QMenu* ffmpegMenu = menuBar->addMenu("FFMPEG");

    QAction* vmafAction = new QAction("Calculate VMAF", this);
    connect(vmafAction, &QAction::triggered, this, [=]() {
        // Always create a new widget instance to allow multiple windows
        CalculateVmafWidget* widget = new CalculateVmafWidget(this);
        widget->setWindowFlag(Qt::Window);
        widget->setAttribute(Qt::WA_DeleteOnClose);
        widget->show();
        widget->raise();
        widget->activateWindow();
        }
    );
    ffmpegMenu->addAction(vmafAction);

	QAction* mpegTsAction = new QAction("Generate MPEG-TS", this);
	connect(mpegTsAction, &QAction::triggered, this, [=]() {
		// Always create a new widget instance to allow multiple windows
		GenerateMpegTsWidget* widget = new GenerateMpegTsWidget(this);
		widget->setWindowFlag(Qt::Window);
		widget->setAttribute(Qt::WA_DeleteOnClose);
		widget->show();
		widget->raise();
		widget->activateWindow();
		}
	);
	ffmpegMenu->addAction(mpegTsAction);

	QAction* mediaInfoAction = new QAction("Media Info", this);
	connect(mediaInfoAction, &QAction::triggered, this, [=]() {
		// Always create a new widget instance to allow multiple windows
		MediaInfoWidget* widget = new MediaInfoWidget(this);
		widget->setWindowFlag(Qt::Window);
		widget->setAttribute(Qt::WA_DeleteOnClose);
		widget->show();
		widget->raise();
		widget->activateWindow();
		}
	);
	ffmpegMenu->addAction(mediaInfoAction);	

    QAction* ffmpegSettingsAction = new QAction("Settings", this);
    connect(ffmpegSettingsAction, &QAction::triggered, this, [=]() {
        // Always create a new widget instance to allow multiple windows
        FFmpegSettingsWidget* widget = new FFmpegSettingsWidget(this);
        widget->setWindowFlag(Qt::Window);
        widget->setAttribute(Qt::WA_DeleteOnClose);
        widget->show();
        widget->raise();
        widget->activateWindow();
        }
    );
    ffmpegMenu->addAction(ffmpegSettingsAction);

    setMenuBar(menuBar);
}