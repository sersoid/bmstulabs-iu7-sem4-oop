#include "render.h"
#include "ui.h"
#include "util.h"

void syncObjectAndDoubleSpinBox(double& targetObjValue, const QDoubleSpinBox& doubleSpinBox, object& obj, QLabel& resultLabel, QLabel& timeLabel, const QSpinBox& resolutionSpinBox, const QCheckBox& RTCheck, const QCheckBox& coordSystemCheck) {
    QObject::connect(&doubleSpinBox, &QDoubleSpinBox::valueChanged, [&](const double value) {
        targetObjValue = value;

        if (RTCheck.isChecked())
            updateTimeLabel(timeLabel, doWithElapsedTime(render, obj, resultLabel, coordSystemCheck.isChecked(), resolutionSpinBox.value()));
    });
}

long long updateTimeLabel(QLabel& timeLabel, const long long elapsedTime) {
    timeLabel.setText(QString::number(elapsedTime) + " мс");

    return elapsedTime;
}
