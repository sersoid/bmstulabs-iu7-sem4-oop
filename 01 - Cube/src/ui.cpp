#include "ui.h"
#include "util.h"

void syncObjectAndDoubleSpinBox(double& targetObjValue, const QDoubleSpinBox& doubleSpinBox, const QCheckBox& RTCheck, const std::function<long long(object&)>& render, object& obj, QLabel& timeLabel) {
    QObject::connect(&doubleSpinBox, &QDoubleSpinBox::valueChanged, [&](const double value) {
        targetObjValue = value;

        if (RTCheck.isChecked())
            updateTimeLabel(timeLabel, doWithElapsedTime(render, obj));
    });
}

void animationDisableWidgets(const Ui::mainWindow& mainUI, const bool isDisable) {
    mainUI.objectX->setDisabled(isDisable);
    mainUI.objectY->setDisabled(isDisable);
    mainUI.objectZ->setDisabled(isDisable);

    mainUI.objectRotateX->setDisabled(isDisable);
    mainUI.objectRotateY->setDisabled(isDisable);
    mainUI.objectRotateZ->setDisabled(isDisable);

    mainUI.renderRTCheck->setDisabled(isDisable);

    if (isDisable || ! mainUI.renderRTCheck->isChecked())
        mainUI.menuRenderButton->setDisabled(isDisable);
}

void onAnimationCheckChanged(const int state, const Ui::mainWindow& mainUI, QThread& animationThread, const std::function<long long(object&)>& render, object &obj) {
    animationDisableWidgets(mainUI, state == Qt::Checked);

    if (state == Qt::Checked)
        animationThread.start();
    else {
        animationThread.requestInterruption();
        animationThread.exit();
        render(obj);
    }
}

long long updateTimeLabel(QLabel& timeLabel, const long long elapsedTime) {
    timeLabel.setText(QString::number(elapsedTime) + " мс");

    return elapsedTime;
}
