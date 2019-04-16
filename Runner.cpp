//
// Created by hector on 15-04-19.
//

#include "Runner.h"
#include "RunnerConfig.h"

Runner::Runner(RunnerConfig *cfg) {
    m_cfg = cfg;
}

int Runner::start() {
    // Like Arduino :)
    setup();
    loop();
    return 1;
}

void Runner::initUI() {
    m_screenManager = new ScreenManager(m_cfg->getSelectedScreen(), emulateHeadPose);
    m_shape = new ShapeWindows();
    m_shape->show(m_cfg->getGazePointerSize(), m_cfg->getGazePointerSize());
}

tobii_device_t *Runner::initDevice() {
    tobii_error_t error = tobii_api_create(&api, nullptr, nullptr);
    assert(error == TOBII_ERROR_NO_ERROR);

    char url[256] = {0};
    error = tobii_enumerate_local_device_urls(api, urlReceiver, url);
    assert(error == TOBII_ERROR_NO_ERROR);
    if (*url == '\0') {
        printf("there are no connected devices");
        exit(1);
    }
    error = tobii_device_create(api, url, &device);
    assert(error == TOBII_ERROR_NO_ERROR);
    return device;

}

void Runner::destroy() {
    tobii_gaze_point_unsubscribe(device);
    tobii_head_pose_unsubscribe(device);
    tobii_device_destroy(device);
    tobii_api_destroy(api);
    /* close connection to m_display */
    XCloseDisplay(m_pDisplay);

}

void Runner::setup() {
    initDevice();
    subscribe();
    initUI();

}

void Runner::loop() {
    startEventLoop();
    destroy();
}

void Runner::startEventLoop() {
    while (true) {
        tobii_error_t error = tobii_wait_for_callbacks(1, &device);
        assert(error == TOBII_ERROR_NO_ERROR || error == TOBII_ERROR_TIMED_OUT);
        error = tobii_device_process_callbacks(device);
        assert(error == TOBII_ERROR_NO_ERROR);
    }
}

void Runner::urlReceiver(char const *url, void *user_data) {
    char *buffer = (char *) user_data;
    if (*buffer != '\0') return; // only keep first value

    if (strlen(url) < 256)
        strcpy(buffer, url);
}

tobii_error_t Runner::subscribe() {
    tobii_error_t error = tobii_head_pose_subscribe(device, headPoseCallback, nullptr);
    if (error != TOBII_ERROR_NO_ERROR) {
        emulateHeadPose = true;
    }
    error = tobii_gaze_point_subscribe(device, gazePointCallback, nullptr);
    assert(error == TOBII_ERROR_NO_ERROR);
    tobii_head_pose_unsubscribe(device);
    return error;
}

void Runner::headPoseCallback(tobii_head_pose_t const *head_pose, void *user_data) {
    /* Doesnt have linux support :/ */
    return;

    if (head_pose->position_validity == TOBII_VALIDITY_VALID)
        printf("Position: (%f, %f, %f)\n",
               head_pose->position_xyz[0],
               head_pose->position_xyz[1],
               head_pose->position_xyz[2]);

    printf("Rotation:\n");
    for (int i = 0; i < 3; ++i) {
        if (head_pose->rotation_validity_xyz[i] == TOBII_VALIDITY_VALID)
            printf("%f\n", head_pose->rotation_xyz[i]);
    }
}

void Runner::gazePointCallback(tobii_gaze_point_t const *gaze_point, void *user_data) {
    if (gaze_point->validity == TOBII_VALIDITY_VALID) {
        printf("Gaze point: %lf, %lf\n", gaze_point->position_xy[0], gaze_point->position_xy[1]);
        auto coord = m_screenManager->relativePointToCoor(gaze_point->position_xy[0], gaze_point->position_xy[1]);
        printf("Gaze point: %d, %d\n", coord[0], coord[1]);
        m_shape->move(coord[0], coord[1]);
    }
}

Display *Runner::m_pDisplay = nullptr;
ScreenManager *Runner::m_screenManager = nullptr;
ShapeWindows *Runner::m_shape = nullptr;