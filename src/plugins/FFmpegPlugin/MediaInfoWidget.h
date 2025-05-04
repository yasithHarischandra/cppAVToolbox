// src/plugins/FFmpegPlugin/MediaInfoWidget.h
#pragma once

#include <QWidget>
class QLineEdit;
class QPushButton;
class QTableView;
class QPlainTextEdit;
class QStandardItemModel;

class ExternalToolJob;

class MediaInfoWidget : public QWidget {
    Q_OBJECT
public:
    explicit MediaInfoWidget(QWidget* parent = nullptr);

private slots:
    void browseFile();
    void updateInfo();

private:
    void setupUI();
    void parseAndDisplay(const QString& json);

    QLineEdit* _fileEdit;
    QPushButton* _browseBtn;
    QPushButton* _refreshBtn;
    QTableView* _tableView;
    QPlainTextEdit* _logOutput;
    QStandardItemModel* _model;
    ExternalToolJob* _job = nullptr;
};
