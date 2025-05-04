// src/MainWindow.h
#pragma once
#include <QMainWindow>

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);

protected:
	void setupMenuBar();
};
