#include "ui.h"
#include "util.h"

void syncObjectAndDoubleSpinBox(double& targetObjValue, const QDoubleSpinBox& doubleSpinBox, const std::function<long long(object&)>& render, object& obj, QLabel& timeLabel) {
    QObject::connect(&doubleSpinBox, &QDoubleSpinBox::valueChanged, [&](const double value) {
        targetObjValue = value;

        updateTimeLabel(timeLabel, doWithElapsedTime(render, obj));
    });
}

long long updateTimeLabel(QLabel& timeLabel, const long long elapsedTime) {
    timeLabel.setText(QString::number(elapsedTime) + " мс");

    return elapsedTime;
}
