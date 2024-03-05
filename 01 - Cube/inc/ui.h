#ifndef UI_H
#define UI_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>

#include "object.h"

void syncObjectAndDoubleSpinBox(double& targetObjValue, const QDoubleSpinBox& doubleSpinBox, object& obj, QLabel& resultLabel, QLabel& timeLabel, const QSpinBox& resolutionSpinBox, const QCheckBox& RTCheck, const QCheckBox& coordSystemCheck);
long long updateTimeLabel(QLabel& timeLabel, long long elapsedTime);

#endif
