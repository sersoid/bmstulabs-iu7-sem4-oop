#include <QFileDialog>

#include "error.h"
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

void initAnimationThread(const QThread& animationThread, const std::function<long long(object&)>& render, object& obj, const QSpinBox& FPSSpinBox) {
    const point center = obj.center, rotation = obj.rotation;

    while (! animationThread.isInterruptionRequested()) {
        const int FPSLimit = FPSSpinBox.value();
        const int frameTimeMs = 1000 / FPSLimit;
        const double stepMlt = 30.0 / FPSLimit;

        obj.rotation.x = std::fmod(obj.rotation.x + 2 * stepMlt, 360);
        obj.rotation.y = std::fmod(obj.rotation.y + 3 * stepMlt, 360);
        obj.rotation.z = std::fmod(obj.rotation.z + 4 * stepMlt, 360);

        const long long elapsedTime = render(obj);

        if (elapsedTime < frameTimeMs)
            QThread::msleep(std::chrono::milliseconds(frameTimeMs - elapsedTime).count());
    }

    obj.center = center;
    obj.rotation = rotation;

    render(obj);
}

void onAnimationCheckChanged(const int state, const Ui::mainWindow& mainUI, QThread& animationThread, const std::function<long long(object&)>& render, object &obj) {
    animationDisableWidgets(mainUI, state == Qt::Checked);

    if (state == Qt::Checked)
        animationThread.start();
    else {
        animationThread.requestInterruption();
        animationThread.exit();
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

std::function<long long(object&)> renderWithTimeUpdate(QLabel& resultLabel, QLabel& timeLabel, const std::function<std::map<std::string, std::vector<int>>(std::vector<std::vector<color>>&, object&, bool)>& renderFunc, const QSpinBox& resolutionSpinBox, const QCheckBox& coordSystemCheck) {
    return [&](object& obj) {
        std::map<std::string, std::vector<int>> rc;
        const int resolution = resolutionSpinBox.value();

        std::vector image(resolution, std::vector<color>(resolution));
        const long long elapsedTimeCalculation = doWithElapsedTime(renderFunc, rc, image, obj, coordSystemCheck.isChecked());
        const long long elapsedTimeRender = doWithElapsedTime(updateQLabelByImage, static_cast<void *>(nullptr), resultLabel, image);

#ifndef NDEBUG
        const long long elapsedTimeDebugPrint = doWithElapsedTime(printErrorDebug, static_cast<void *>(nullptr), rc);
        return updateTimeLabel(timeLabel, elapsedTimeCalculation, elapsedTimeRender, elapsedTimeDebugPrint);
#else
        return updateTimeLabel(timeLabel, elapsedTimeCalculation, elapsedTimeRender);
#endif
    };
}

// Actions

int selectFileDialog(const Ui::mainWindow& mainUI, object& obj) {
    int rc = OK;
    const QString objFileName = QFileDialog::getOpenFileName(mainUI.mainWidget, "Файл объекта", "./");

    if (! objFileName.isEmpty())
        rc |= loadObject(objFileName.toStdString(), obj);
    else
        rc |= FILE_DOESNT_EXIST;

    return rc;
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

#ifndef NDEBUG
long long updateTimeLabel(QLabel& timeLabel, const long long elapsedTimeCalculation, const long long elapsedTimeRender, const long long elapsedTimeDebugPrint) {
    timeLabel.setText("Вычисление: " + QString::number(elapsedTimeCalculation) + " мс | Отрисовка: " + QString::number(elapsedTimeRender) + " мс | Отладка: " + QString::number(elapsedTimeDebugPrint) + " мс");

    return elapsedTimeCalculation + elapsedTimeRender;
}
#else
long long updateTimeLabel(QLabel& timeLabel, const long long elapsedTimeCalculation, const long long elapsedTimeRender) {
    timeLabel.setText("Вычисление: " + QString::number(elapsedTimeCalculation) + " мс | Отрисовка: " + QString::number(elapsedTimeRender) + " мс");

    return elapsedTimeCalculation + elapsedTimeRender;
}
#endif
