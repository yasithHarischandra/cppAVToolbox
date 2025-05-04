#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>

#include "ExternalToolJob.h"
#include "ConfigManager.h"
#include "VmafResult.h"

class CalculateVmafWidget : public QWidget {
    Q_OBJECT
public:
    explicit CalculateVmafWidget(QWidget* parent = nullptr);

private slots:
    void onComputeVmaf();
    void onBrowseRef();
    void onBrowseDist();

private:
    QLineEdit* _refLineEdit;
    QLineEdit* _distLineEdit;
    QPushButton* _refBrowseButton;
    QPushButton* _distBrowseButton;
    QPushButton* _computeButton;
    QPlainTextEdit* _outputTextEdit;
    QLabel* _resultLabel;

    ExternalToolJob* _job = nullptr;

    void setupUI();
};
