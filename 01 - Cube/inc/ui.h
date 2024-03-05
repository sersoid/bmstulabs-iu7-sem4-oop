#ifndef UI_H
#define UI_H

#include <QDoubleSpinBox>
#include <QLabel>

#include "object.h"

void syncObjectAndDoubleSpinBox(double& targetObjValue, const QDoubleSpinBox& doubleSpinBox, const std::function<long long(object&)>& render, object& obj, QLabel& timeLabel);
long long updateTimeLabel(QLabel& timeLabel, long long elapsedTime);

#endif
