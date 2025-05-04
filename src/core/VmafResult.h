#pragma once

#include <QString>
#include <QVector>

/**
 * Holds the results of a VMAF calculation.
 */
class VmafResult {
public:
    double averageScore = 0.0;
    QVector<double> frameScores;

    static VmafResult parseFromJson(const QString& jsonString);
};

