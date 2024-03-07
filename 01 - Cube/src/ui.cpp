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

template <size_t n, size_t m>
void updateQLabelByImage(QLabel& resultLabel, const std::array<std::array<color, n>, m>& image) {
    const int resolution = static_cast<int>(image.size());

    QImage resultImage(resolution, resolution, QImage::Format_RGB32);
    QRgb* resultImageBits = reinterpret_cast<QRgb*>(resultImage.bits());

    for (size_t y = 0; y < resolution; y++)
        for (size_t x = 0; x < resolution; x++)
            resultImageBits[y * resolution + x] = static_cast<unsigned int>(image[x][y]);

    resultLabel.setPixmap(QPixmap::fromImage(resultImage).scaled(resultLabel.width(), resultLabel.height(), Qt::KeepAspectRatio));
}

long long updateTimeLabel(QLabel& timeLabel, const long long elapsedTime) {
    timeLabel.setText(QString::number(elapsedTime) + " мс");

    return elapsedTime;
}
