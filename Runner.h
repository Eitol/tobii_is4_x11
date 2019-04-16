//
// Created by hector on 15-04-19.
//

#ifndef EXAMPLE_RUNNER_H
#define EXAMPLE_RUNNER_H

#include <tobii/tobii.h>
#include <tobii/tobii_streams.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <cstring>
#include <thread>
#include "Screen.h"
#include "Shape.h"
#include "RunnerConfig.h"

using namespace std;

class Runner {
public:
    explicit Runner(RunnerConfig *cfg);

    int start();

protected:
    static void headPoseCallback(tobii_head_pose_t const *head_pose, void *user_data);

    static void gazePointCallback(tobii_gaze_point_t const *gaze_point, void *user_data);

private:
    void initUI();

    tobii_device_t *initDevice();


    void destroy();

    void setup();

    void loop();

    void startEventLoop();

    tobii_error_t subscribe();

private:
    static void urlReceiver(char const *url, void *user_data);

private:
    tobii_device_t *device;
    tobii_api_t *api;
    bool emulateHeadPose = false;
    static Display *m_pDisplay;
    static ScreenManager *m_screenManager;
    static ShapeWindows *m_shape;
    RunnerConfig *m_cfg;
};


#endif //EXAMPLE_RUNNER_H
