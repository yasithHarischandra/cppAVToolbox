// -------------------------------------------------------
// File: src/plugins/FFmpegPlugin/GenerateMpegTsWidget.h
// -------------------------------------------------------
#pragma once

#include <QWidget>

class QLineEdit;
class QPushButton;
class QSpinBox;
class QCheckBox;
class QPlainTextEdit;

class ExternalToolJob;

/**
 * Widget to generate an MPEG-TS stream from a media file without re-encoding.
 * User provides input file, destination URL, port, and loop option.
 * Includes start/stop controls and input validation.
 */
class GenerateMpegTsWidget : public QWidget {
    Q_OBJECT
public:
    explicit GenerateMpegTsWidget(QWidget* parent = nullptr);

private slots:
    void onBrowseInput();
    void onStartStream();
    void onStopStream();

private:
    void setupUI();
    bool validateInputs();

    QLineEdit* _inputEdit;
    QPushButton* _browseInput;
    QLineEdit* _urlEdit;
    QSpinBox* _portSpin;
    QCheckBox* _loopCheck;
    QPushButton* _startButton;
    QPushButton* _stopButton;
    QPlainTextEdit* _logOutput;

    ExternalToolJob* _job = nullptr;
};

