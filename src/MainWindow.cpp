// src/MainWindow.cpp
#include "MainWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
    setupMenuBar();

	auto* central = new QWidget;
	auto* layout = new QVBoxLayout(central);
	layout->addWidget(new QLabel("AVToolbox is Ready"));
	setCentralWidget(central);
	setWindowTitle("AVToolbox");
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);

    // FFmpeg menu
    QMenu* ffmpegMenu = menuBar->addMenu("FFMPEG");

    QAction* vmafAction = new QAction("Calculate VMAF", this);
    connect(vmafAction, &QAction::triggered, this, [=]() {
        if (!_vmafWidget) {
            _vmafWidget = new CalculateVmafWidget;
            _vmafWidget->setAttribute(Qt::WA_DeleteOnClose);
            connect(_vmafWidget, &QWidget::destroyed, this, [=]() {
                _vmafWidget = nullptr;
                });
        }
        _vmafWidget->show();
        _vmafWidget->raise();
        _vmafWidget->activateWindow();
        });

    ffmpegMenu->addAction(vmafAction);

    setMenuBar(menuBar);
}