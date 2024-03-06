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
    QImage resultImage(static_cast<int>(image.size()), static_cast<int>(image.size()), QImage::Format_RGB32);

    for (int i = 0; i < image.size(); i++)
        for (int j = 0; j < image.size(); j++)
            resultImage.setPixel(i, j, QColor(image[i][j].r, image[i][j].g, image[i][j].b).rgb());

    resultLabel.setPixmap(QPixmap::fromImage(resultImage).scaled(resultLabel.width(), resultLabel.height(), Qt::KeepAspectRatio));
}

long long updateTimeLabel(QLabel& timeLabel, const long long elapsedTime) {
    timeLabel.setText(QString::number(elapsedTime) + " мс");

    return elapsedTime;
}
