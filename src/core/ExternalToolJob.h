#pragma once

#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QString>

/**
 * Runs an external tool and captures its output, with support for cancellation and timeouts.
 */
class ExternalToolJob : public QObject {
    Q_OBJECT

public:
    explicit ExternalToolJob(QObject* parent = nullptr);
    ~ExternalToolJob();

    void start(const QString& program, const QStringList& arguments, const QString& workingDir = "", int timeoutMs = 0);
    void stop();

    QByteArray readAllStandardOutput() const { return _buffer; }
    //QByteArray readAllStandardError()  const { return _process->readAllStandardError(); }

signals:
    void started();
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void outputReceived(const QString& text);
    void errorReceived(const QString& text);
    void errorOccurred(const QString& description);
    void timedOut();

private:
    QProcess* _process;
    QTimer* _timeoutTimer;
    QByteArray _buffer;
};
