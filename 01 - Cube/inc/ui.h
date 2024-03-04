#ifndef __H_UI
#define __H_UI

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>

#include "object.h"

void syncObjectAndDoubleSpinBox(double& targetObjValue, const QDoubleSpinBox& doubleSpinBox, object& obj, QLabel& resultLabel, QLabel& timeLabel, QSpinBox& resolutionSpinBox, const QCheckBox& RTCheck, QCheckBox& coordSystemCheck);
long long updateTimeLabel(QLabel& timeLabel, long long elapsedTime);

#endif
