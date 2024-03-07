#include "ui.h"

void syncObjectAndDoubleSpinBox(double& targetObjValue, const QDoubleSpinBox& doubleSpinBox, const QCheckBox& RTCheck, const std::function<long long(object&)>& render, object& obj) {
    QObject::connect(&doubleSpinBox, &QDoubleSpinBox::valueChanged, [&](const double value) {
        targetObjValue = value;

        if (RTCheck.isChecked())
            render(obj);
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

void updateQLabelByImage(QLabel& resultLabel, const std::vector<std::vector<color>>& image) {
    const int resolution = static_cast<int>(image.size()), buffer_step = resolution * 3, buffer_size = buffer_step * resolution;
    unsigned char* buffer = new unsigned char[buffer_size];

    for (size_t i = 0; i < resolution; i++)
        memcpy(buffer + i * buffer_step, image[i].data(), resolution * 3);

    const QImage resultImage(buffer, resolution, resolution, QImage::Format_RGB888);

    resultLabel.setPixmap(QPixmap::fromImage(resultImage).scaled(resultLabel.width(), resultLabel.height(), Qt::KeepAspectRatio));

    delete[] buffer;
}

long long updateTimeLabel(QLabel& timeLabel, const long long elapsedTimeCalculation, const long long elapsedTimeRender) {
    timeLabel.setText("Вычисление: " + QString::number(elapsedTimeCalculation) + " мс | Отрисовка: " + QString::number(elapsedTimeRender) + " мс");

    return elapsedTimeCalculation + elapsedTimeRender;
}
