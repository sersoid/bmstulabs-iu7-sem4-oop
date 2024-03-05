#include <QThread>

#include "object.h"
#include "render.h"
#include "util.h"
#include "ui.h"

#include "../ui/ui_about.h"
#include "../ui/ui_main.h"

int main(int argc, char* argv[]) {
    object obj = {
        .edges = {
            {{-1, -1, -1}, {-1, -1, 1}},
            {{-1, -1, -1}, {-1, 1, -1}},
            {{-1, -1, -1}, {1, -1, -1}},
            {{-1, -1, 1}, {-1, 1, 1}},
            {{-1, -1, 1}, {1, -1, 1}},
            {{-1, 1, -1}, {-1, 1, 1}},
            {{-1, 1, -1}, {1, 1, -1}},
            {{-1, 1, 1}, {1, 1, 1}},
            {{1, -1, -1}, {1, -1, 1}},
            {{1, -1, -1}, {1, 1, -1}},
            {{1, -1, 1}, {1, 1, 1}},
            {{1, 1, -1}, {1, 1, 1}},
        }
    };

    // Application

    QApplication app(argc, argv);

    // Windows

    Ui::mainWindow mainUI;
    Ui::aboutWindow aboutUI;
    QMainWindow mainWidget, aboutWidget;

    mainUI.setupUi(&mainWidget);
    aboutUI.setupUi(&aboutWidget);

    const std::function renderCall = [&](object& renderObj) { return updateTimeLabel(*mainUI.resultTimeLabel, doWithElapsedTime(render, renderObj, *mainUI.resultLabel, mainUI.renderCoordSystemCheck->isChecked(), mainUI.renderResolutionSpinBox->value())); };

    // Object

    syncObjectAndDoubleSpinBox(obj.center.x, *mainUI.objectX, obj, *mainUI.resultLabel, *mainUI.resultTimeLabel, *mainUI.renderResolutionSpinBox, *mainUI.renderRTCheck, *mainUI.renderCoordSystemCheck);
    syncObjectAndDoubleSpinBox(obj.center.y, *mainUI.objectY, obj, *mainUI.resultLabel, *mainUI.resultTimeLabel, *mainUI.renderResolutionSpinBox, *mainUI.renderRTCheck, *mainUI.renderCoordSystemCheck);
    syncObjectAndDoubleSpinBox(obj.center.z, *mainUI.objectZ, obj, *mainUI.resultLabel, *mainUI.resultTimeLabel, *mainUI.renderResolutionSpinBox, *mainUI.renderRTCheck, *mainUI.renderCoordSystemCheck);

    syncObjectAndDoubleSpinBox(obj.rotation.x, *mainUI.objectRotateX, obj, *mainUI.resultLabel, *mainUI.resultTimeLabel, *mainUI.renderResolutionSpinBox, *mainUI.renderRTCheck, *mainUI.renderCoordSystemCheck);
    syncObjectAndDoubleSpinBox(obj.rotation.y, *mainUI.objectRotateY, obj, *mainUI.resultLabel, *mainUI.resultTimeLabel, *mainUI.renderResolutionSpinBox, *mainUI.renderRTCheck, *mainUI.renderCoordSystemCheck);
    syncObjectAndDoubleSpinBox(obj.rotation.z, *mainUI.objectRotateZ, obj, *mainUI.resultLabel, *mainUI.resultTimeLabel, *mainUI.renderResolutionSpinBox, *mainUI.renderRTCheck, *mainUI.renderCoordSystemCheck);

    // Animation

    QThread animationThread;

    QObject::connect(&animationThread, &QThread::started, [&] {
        object objCopy = obj;

        while (! animationThread.isInterruptionRequested()) {
            const int frameTimeMs = 1000 / mainUI.animationFPSSpinBox->value();
            const double stepMlt = 30.0 / mainUI.animationFPSSpinBox->value();

            objCopy.rotation.x = std::fmod(objCopy.rotation.x + 2 * stepMlt, 360);
            objCopy.rotation.y = std::fmod(objCopy.rotation.y + 3 * stepMlt, 360);
            objCopy.rotation.z = std::fmod(objCopy.rotation.z + 4 * stepMlt, 360);

            const long long elapsedTime = renderCall(objCopy);

            if (elapsedTime < frameTimeMs)
                QThread::msleep(std::chrono::milliseconds(frameTimeMs - elapsedTime).count());
        }
    });

    QObject::connect(mainUI.animationCheck, &QCheckBox::stateChanged, [&](const int state) {
        if (state == Qt::Checked) {
            mainUI.objectX->setDisabled(true);
            mainUI.objectY->setDisabled(true);
            mainUI.objectZ->setDisabled(true);

            mainUI.objectRotateX->setDisabled(true);
            mainUI.objectRotateY->setDisabled(true);
            mainUI.objectRotateZ->setDisabled(true);

            mainUI.renderRTCheck->setDisabled(true);
            mainUI.menuRenderButton->setDisabled(true);

            animationThread.start();
        } else {
            mainUI.objectX->setEnabled(true);
            mainUI.objectY->setEnabled(true);
            mainUI.objectZ->setEnabled(true);

            mainUI.objectRotateX->setEnabled(true);
            mainUI.objectRotateY->setEnabled(true);
            mainUI.objectRotateZ->setEnabled(true);

            mainUI.renderRTCheck->setEnabled(true);

            if (! mainUI.renderRTCheck->isChecked())
                mainUI.menuRenderButton->setEnabled(true);

            animationThread.requestInterruption();
            animationThread.exit();
            renderCall(obj);
        }
    });

    // Render

    QObject::connect(mainUI.renderResolutionSpinBox, &QSpinBox::valueChanged, [&] { if (mainUI.renderRTCheck->isChecked()) renderCall(obj); });

    QObject::connect(mainUI.renderRTCheck, &QCheckBox::stateChanged, [&](const int state) { if (state) renderCall(obj); });
    QObject::connect(mainUI.renderCoordSystemCheck, &QCheckBox::stateChanged, [&] { if (mainUI.renderRTCheck->isChecked()) renderCall(obj); });

    QObject::connect(mainUI.menuRenderButton, &QPushButton::clicked, [&] { renderCall(obj); });

    // Actions

    // QObject::connect(mainUI.actionOpen, &QAction::triggered, ...);
    QObject::connect(mainUI.actionAbout, &QAction::triggered, [&aboutWidget]{ aboutWidget.show(); });

    // Initial

    renderCall(obj);
    mainWidget.show();

    return QApplication::exec();
}
