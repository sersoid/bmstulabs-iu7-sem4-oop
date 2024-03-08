#include "object.h"
#include "render.h"
#include "util.h"
#include "ui.h"

#include "../ui/ui_about.h"
#include "../ui/ui_main.h"

int main(int argc, char* argv[]) {
    // object obj = {
    //     .edges = {
    //         {{-1, -1, -1}, {-1, -1, 1}},
    //         {{-1, -1, -1}, {-1, 1, -1}},
    //         {{-1, -1, -1}, {1, -1, -1}},
    //         {{-1, -1, 1}, {-1, 1, 1}},
    //         {{-1, -1, 1}, {1, -1, 1}},
    //         {{-1, 1, -1}, {-1, 1, 1}},
    //         {{-1, 1, -1}, {1, 1, -1}},
    //         {{-1, 1, 1}, {1, 1, 1}},
    //         {{1, -1, -1}, {1, -1, 1}},
    //         {{1, -1, -1}, {1, 1, -1}},
    //         {{1, -1, 1}, {1, 1, 1}},
    //         {{1, 1, -1}, {1, 1, 1}},
    //     }
    // };

    object obj = {
        .edges = {
            // Верхняя окружность
            {{1.0, 0.0, 2.5}, {0.9239, 0.3827, 2.5}},
            {{0.9239, 0.3827, 2.5}, {0.707, 0.707, 2.5}},
            {{0.707, 0.707, 2.5}, {0.3827, 0.9239, 2.5}},
            {{0.3827, 0.9239, 2.5}, {0.0, 1.0, 2.5}},
            {{0.0, 1.0, 2.5}, {-0.3827, 0.9239, 2.5}},
            {{-0.3827, 0.9239, 2.5}, {-0.707, 0.707, 2.5}},
            {{-0.707, 0.707, 2.5}, {-0.9239, 0.3827, 2.5}},
            {{-0.9239, 0.3827, 2.5}, {-1.0, 0.0, 2.5}},
            {{-1.0, 0.0, 2.5}, {-0.9239, -0.3827, 2.5}},
            {{-0.9239, -0.3827, 2.5}, {-0.707, -0.707, 2.5}},
            {{-0.707, -0.707, 2.5}, {-0.3827, -0.9239, 2.5}},
            {{-0.3827, -0.9239, 2.5}, {0.0, -1.0, 2.5}},
            {{0.0, -1.0, 2.5}, {0.3827, -0.9239, 2.5}},
            {{0.3827, -0.9239, 2.5}, {0.707, -0.707, 2.5}},
            {{0.707, -0.707, 2.5}, {0.9239, -0.3827, 2.5}},
            {{0.9239, -0.3827, 2.5}, {1.0, 0.0, 2.5}},
            // Нижняя окружность
            {{1.0, 0.0, -2.5}, {0.9239, 0.3827, -2.5}},
            {{0.9239, 0.3827, -2.5}, {0.707, 0.707, -2.5}},
            {{0.707, 0.707, -2.5}, {0.3827, 0.9239, -2.5}},
            {{0.3827, 0.9239, -2.5}, {0.0, 1.0, -2.5}},
            {{0.0, 1.0, -2.5}, {-0.3827, 0.9239, -2.5}},
            {{-0.3827, 0.9239, -2.5}, {-0.707, 0.707, -2.5}},
            {{-0.707, 0.707, -2.5}, {-0.9239, 0.3827, -2.5}},
            {{-0.9239, 0.3827, -2.5}, {-1.0, 0.0, -2.5}},
            {{-1.0, 0.0, -2.5}, {-0.9239, -0.3827, -2.5}},
            {{-0.9239, -0.3827, -2.5}, {-0.707, -0.707, -2.5}},
            {{-0.707, -0.707, -2.5}, {-0.3827, -0.9239, -2.5}},
            {{-0.3827, -0.9239, -2.5}, {0.0, -1.0, -2.5}},
            {{0.0, -1.0, -2.5}, {0.3827, -0.9239, -2.5}},
            {{0.3827, -0.9239, -2.5}, {0.707, -0.707, -2.5}},
            {{0.707, -0.707, -2.5}, {0.9239, -0.3827, -2.5}},
            {{0.9239, -0.3827, -2.5}, {1.0, 0.0, -2.5}},
            // Вертикальные рёбра
            {{1.0, 0.0, 2.5}, {1.0, 0.0, -2.5}},
            {{0.9239, 0.3827, 2.5}, {0.9239, 0.3827, -2.5}},
            {{0.707, 0.707, 2.5}, {0.707, 0.707, -2.5}},
            {{0.3827, 0.9239, 2.5}, {0.3827, 0.9239, -2.5}},
            {{0.0, 1.0, 2.5}, {0.0, 1.0, -2.5}},
            {{-0.3827, 0.9239, 2.5}, {-0.3827, 0.9239, -2.5}},
            {{-0.707, 0.707, 2.5}, {-0.707, 0.707, -2.5}},
            {{-0.9239, 0.3827, 2.5}, {-0.9239, 0.3827, -2.5}},
            {{-1.0, 0.0, 2.5}, {-1.0, 0.0, -2.5}},
            {{-0.9239, -0.3827, 2.5}, {-0.9239, -0.3827, -2.5}},
            {{-0.707, -0.707, 2.5}, {-0.707, -0.707, -2.5}},
            {{-0.3827, -0.9239, 2.5}, {-0.3827, -0.9239, -2.5}},
            {{0.0, -1.0, 2.5}, {0.0, -1.0, -2.5}},
            {{0.3827, -0.9239, 2.5}, {0.3827, -0.9239, -2.5}},
            {{0.707, -0.707, 2.5}, {0.707, -0.707, -2.5}},
            {{0.9239, -0.3827, 2.5}, {0.9239, -0.3827, -2.5}}
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

    // Properties

    const std::function<long long(object&)> renderWithCameraCall = renderWithTimeUpdate(*mainUI.resultLabel, *mainUI.resultTimeLabel, renderWithCamera, *mainUI.renderResolutionSpinBox, *mainUI.renderCoordSystemCheck);
    const std::function<long long(object&)> renderWithoutCameraCall = renderWithTimeUpdate(*mainUI.resultLabel, *mainUI.resultTimeLabel, renderWithoutCamera, *mainUI.renderResolutionSpinBox, *mainUI.renderCoordSystemCheck);
    std::function<long long(object&)> render = renderWithoutCameraCall;

    // Object

    syncObjectAndDoubleSpinBox(obj.center.x, *mainUI.objectX, *mainUI.renderRTCheck, render, obj);
    syncObjectAndDoubleSpinBox(obj.center.y, *mainUI.objectY, *mainUI.renderRTCheck, render, obj);
    syncObjectAndDoubleSpinBox(obj.center.z, *mainUI.objectZ, *mainUI.renderRTCheck, render, obj);

    syncObjectAndDoubleSpinBox(obj.rotation.x, *mainUI.objectRotateX, *mainUI.renderRTCheck, render, obj);
    syncObjectAndDoubleSpinBox(obj.rotation.y, *mainUI.objectRotateY, *mainUI.renderRTCheck, render, obj);
    syncObjectAndDoubleSpinBox(obj.rotation.z, *mainUI.objectRotateZ, *mainUI.renderRTCheck, render, obj);

    // Animation

    QThread animationThread;

    QObject::connect(&animationThread, &QThread::started, [&] { initAnimationThread(animationThread, render, obj, *mainUI.animationFPSSpinBox); });
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

    QObject::connect(mainUI.actionAbout, &QAction::triggered, [&aboutWidget]{ aboutWidget.show(); });

    // Initial

    render(obj);
    mainWidget.show();

    return QApplication::exec();
}
