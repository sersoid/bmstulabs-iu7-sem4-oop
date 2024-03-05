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

    const std::function renderWithCameraCall = [&](object& renderObj) { return updateTimeLabel(*mainUI.resultTimeLabel, doWithElapsedTime(renderWithCamera, renderObj, *mainUI.resultLabel, mainUI.renderCoordSystemCheck->isChecked(), mainUI.renderResolutionSpinBox->value())); };
    const std::function renderWithoutCameraCall = [&](object& renderObj) { return updateTimeLabel(*mainUI.resultTimeLabel, doWithElapsedTime(renderWithoutCamera, renderObj, *mainUI.resultLabel, mainUI.renderCoordSystemCheck->isChecked(), mainUI.renderResolutionSpinBox->value())); };
    std::function render = renderWithoutCameraCall;

    // Object

    syncObjectAndDoubleSpinBox(obj.center.x, *mainUI.objectX, *mainUI.renderRTCheck, render, obj, *mainUI.resultTimeLabel);
    syncObjectAndDoubleSpinBox(obj.center.y, *mainUI.objectY, *mainUI.renderRTCheck, render, obj, *mainUI.resultTimeLabel);
    syncObjectAndDoubleSpinBox(obj.center.z, *mainUI.objectZ, *mainUI.renderRTCheck, render, obj, *mainUI.resultTimeLabel);

    syncObjectAndDoubleSpinBox(obj.rotation.x, *mainUI.objectRotateX, *mainUI.renderRTCheck, render, obj, *mainUI.resultTimeLabel);
    syncObjectAndDoubleSpinBox(obj.rotation.y, *mainUI.objectRotateY, *mainUI.renderRTCheck, render, obj, *mainUI.resultTimeLabel);
    syncObjectAndDoubleSpinBox(obj.rotation.z, *mainUI.objectRotateZ, *mainUI.renderRTCheck, render, obj, *mainUI.resultTimeLabel);

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

            const long long elapsedTime = render(objCopy);

            if (elapsedTime < frameTimeMs)
                QThread::msleep(std::chrono::milliseconds(frameTimeMs - elapsedTime).count());
        }
    });

    QObject::connect(mainUI.animationCheck, &QCheckBox::stateChanged, [&](const int state) { onAnimationCheckChanged(state, mainUI, animationThread, render, obj); });

    // Render

    QObject::connect(mainUI.renderResolutionSpinBox, &QSpinBox::valueChanged, [&] { if (mainUI.renderRTCheck->isChecked()) render(obj); });

    QObject::connect(mainUI.renderRTCheck, &QCheckBox::stateChanged, [&](const int state) { if (state) render(obj); });
    QObject::connect(mainUI.renderCoordSystemCheck, &QCheckBox::stateChanged, [&] { if (mainUI.renderRTCheck->isChecked()) render(obj); });
    QObject::connect(mainUI.renderExperimentCheck, &QCheckBox::stateChanged, [&](const int state) {
        render = state == Qt::Checked ? renderWithCameraCall : renderWithoutCameraCall;
        render(obj);
    });

    QObject::connect(mainUI.menuRenderButton, &QPushButton::clicked, [&] { render(obj); });

    // Actions

    // QObject::connect(mainUI.actionOpen, &QAction::triggered, ...);
    QObject::connect(mainUI.actionAbout, &QAction::triggered, [&aboutWidget]{ aboutWidget.show(); });

    // Initial

    render(obj);
    mainWidget.show();

    return QApplication::exec();
}
