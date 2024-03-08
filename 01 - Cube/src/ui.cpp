#include "ui.h"
#include "util.h"

// Object

void syncObjectAndDoubleSpinBox(double& targetObjValue, const QDoubleSpinBox& doubleSpinBox, const QCheckBox& RTCheck, const std::function<long long(object&)>& render, object& obj) {
    QObject::connect(&doubleSpinBox, &QDoubleSpinBox::valueChanged, [&](const double value) {
        targetObjValue = value;

        if (RTCheck.isChecked())
            render(obj);
    });
}

// Animation

void initAnimationThread(const QThread& animationThread, const std::function<long long(object&)>& render, const object& obj, const QSpinBox& FPSSpinBox) {
    object objCopy = obj;

    while (! animationThread.isInterruptionRequested()) {
        const int FPSLimit = FPSSpinBox.value();
        const int frameTimeMs = 1000 / FPSLimit;
        const double stepMlt = 30.0 / FPSLimit;

        objCopy.rotation.x = std::fmod(objCopy.rotation.x + 2 * stepMlt, 360);
        objCopy.rotation.y = std::fmod(objCopy.rotation.y + 3 * stepMlt, 360);
        objCopy.rotation.z = std::fmod(objCopy.rotation.z + 4 * stepMlt, 360);

        const long long elapsedTime = render(objCopy);

        if (elapsedTime < frameTimeMs)
            QThread::msleep(std::chrono::milliseconds(frameTimeMs - elapsedTime).count());
    }
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

// Render

std::function<long long(object&)> renderWithTimeUpdate(QLabel& resultLabel, QLabel& timeLabel, const std::function<void(std::vector<std::vector<color>>&, object&, bool, int)>& renderFunc, const QSpinBox& resolutionSpinBox, const QCheckBox& coordSystemCheck) {
    return [&](object& obj) {
        int resolution = resolutionSpinBox.value();

        std::vector image(resolution, std::vector<color>(resolution));
        const long long elapsedTimeCalculation = doWithElapsedTime(renderFunc, image, obj, coordSystemCheck.isChecked(), resolution);
        const long long elapsedTimeRender = doWithElapsedTime(updateQLabelByImage, resultLabel, image);

        return updateTimeLabel(timeLabel, elapsedTimeCalculation, elapsedTimeRender);
    };
}

// Result

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
