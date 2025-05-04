#include "VmafResult.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

VmafResult VmafResult::parseFromJson(const QString& jsonString) {
    VmafResult result;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError || !doc.isObject())
        return result;

    QJsonObject root = doc.object();

    // Extract average from pooled_metrics.vmaf.mean
    if (root.contains("pooled_metrics") && root["pooled_metrics"].isObject()) {
        QJsonObject pooled = root.value("pooled_metrics").toObject();
        if (pooled.contains("vmaf") && pooled["vmaf"].isObject()) {
            QJsonObject vmafObj = pooled.value("vmaf").toObject();
            result.averageScore = vmafObj.value("mean").toDouble();
        }
    }

    // Parse per-frame scores
    if (root.contains("frames") && root["frames"].isArray()) {
        QJsonArray frames = root["frames"].toArray();
        for (const auto& frameVal : frames) {
            if (frameVal.isObject()) {
                QJsonObject frame = frameVal.toObject();
                result.frameScores.append(frame.value("metrics").toObject().value("vmaf").toDouble());
            }
        }
    }

    return result;
}
