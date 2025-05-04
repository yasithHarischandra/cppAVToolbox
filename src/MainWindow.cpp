// src/MainWindow.cpp
#include "MainWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "plugins/FFmpegPlugin/CalculateVmafWidget.h"

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

    ffmpegMenu->addAction(vmafAction);

    setMenuBar(menuBar);
}