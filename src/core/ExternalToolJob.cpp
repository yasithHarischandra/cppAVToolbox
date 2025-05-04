#include "ExternalToolJob.h"

ExternalToolJob::ExternalToolJob(QObject* parent)
    : QObject(parent),
    _process(new QProcess(this)),
    _timeoutTimer(new QTimer(this))
{
    _timeoutTimer->setSingleShot(true);

    connect(_process, &QProcess::readyReadStandardOutput, this, [=]() {
        QByteArray chunk = _process->readAllStandardOutput();
        _buffer += chunk;
        emit outputReceived(QString::fromLocal8Bit(chunk));
        });

    connect(_process, &QProcess::readyReadStandardError, this, [=]() {
        emit errorReceived(QString::fromLocal8Bit(_process->readAllStandardError()));
        });

    connect(_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, [=](int code, QProcess::ExitStatus status) {
            _timeoutTimer->stop();
            emit finished(code, status);
        });

    connect(_process, &QProcess::started, this, &ExternalToolJob::started);

    connect(_process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
        emit errorOccurred(QString("Process error: %1").arg(error));
        });

    connect(_timeoutTimer, &QTimer::timeout, this, [=]() {
        stop();
        emit timedOut();
        });
}

ExternalToolJob::~ExternalToolJob() {
    stop();
}

void ExternalToolJob::start(const QString& program, const QStringList& arguments, const QString& workingDir, int timeoutMs) 
{
    if(workingDir != "")
        _process->setWorkingDirectory(workingDir);

    _process->start(program, arguments);
    if (timeoutMs > 0) {
        _timeoutTimer->start(timeoutMs);
    }
}

void ExternalToolJob::stop() {
    if (_process->state() != QProcess::NotRunning) {
        _timeoutTimer->stop();
        _process->kill();
        _process->waitForFinished();
    }
}
