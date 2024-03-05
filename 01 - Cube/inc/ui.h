#ifndef UI_H
#define UI_H

#include <QThread>

#include "object.h"

#include "../ui/ui_main.h"

void syncObjectAndDoubleSpinBox(double& targetObjValue, const QDoubleSpinBox& doubleSpinBox, const QCheckBox& RTCheck, const std::function<long long(object&)>& render, object& obj, QLabel& timeLabel);
void animationDisableWidgets(const Ui::mainWindow& mainUI, bool isDisable);
void onAnimationCheckChanged(int state, const Ui::mainWindow& mainUI, QThread& animationThread, const std::function<long long(object&)>& render, object &obj);
long long updateTimeLabel(QLabel& timeLabel, long long elapsedTime);

#endif
