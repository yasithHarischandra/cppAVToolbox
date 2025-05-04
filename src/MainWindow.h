// src/MainWindow.h
#pragma once
#include <QMainWindow>
#include "plugins/FFmpegPlugin/CalculateVmafWidget.h"

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);

protected:
	void setupMenuBar();

	CalculateVmafWidget* _vmafWidget = nullptr;
};
