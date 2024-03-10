#ifndef UI_H
#define UI_H

#include <QThread>

#include "object.h"
#include "render.h"

#include "../ui/ui_main.h"

// Object

void syncObjectAndDoubleSpinBox(double& targetObjValue, const QDoubleSpinBox& doubleSpinBox, const QCheckBox& RTCheck, const std::function<long long(object&)>& render, object& obj);

// Animation

void initAnimationThread(const QThread& animationThread, const std::function<long long(object&)>& render, object& obj, const QSpinBox& FPSSpinBox);
void onAnimationCheckChanged(int state, const Ui::mainWindow& mainUI, QThread& animationThread, const std::function<long long(object&)>& render, object &obj);
void animationDisableWidgets(const Ui::mainWindow& mainUI, bool isDisable);

// Render

std::function<long long(object&)> renderWithTimeUpdate(QLabel& resultLabel, QLabel& timeLabel, const std::function<std::map<std::string, std::vector<int>>(std::vector<std::vector<color>>&, object&, bool)>& renderFunc, const QSpinBox& resolutionSpinBox, const QCheckBox& coordSystemCheck);

// Actions

int selectFileDialog(const Ui::mainWindow& mainUI, object& obj);

// Result

void updateQLabelByImage(QLabel& resultLabel, const std::vector<std::vector<color>>& image);
#ifndef NDEBUG
long long updateTimeLabel(QLabel& timeLabel, long long elapsedTimeCalculation, long long elapsedTimeRender, long long elapsedTimeDebugPrint);
#else
long long updateTimeLabel(QLabel& timeLabel, long long elapsedTimeCalculation, long long elapsedTimeRender);
#endif

#endif
