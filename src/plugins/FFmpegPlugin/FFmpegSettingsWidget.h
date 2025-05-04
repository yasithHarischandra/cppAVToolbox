// -------------------------------------------------------
// File: src/plugins/FFmpegPlugin/FFmpegSettingsWidget.h
// -------------------------------------------------------
#pragma once
#include <QWidget>

class QLineEdit;
class QPushButton;

/**
 * Widget for configuring the ffmpeg binary path.
 * Allows user to browse and set the executable location.
 */
class FFmpegSettingsWidget : public QWidget {
	Q_OBJECT
public:
	explicit FFmpegSettingsWidget(QWidget* parent = nullptr);

private:
	QLineEdit* _pathEdit;
	QPushButton* _browseBtn;
};
