//
// Created by hector on 16-04-19.
//

#ifndef TOBII_IS4_X11_RUNNERCONFIG_H
#define TOBII_IS4_X11_RUNNERCONFIG_H

#include <string>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>

using namespace libconfig;
using namespace std;

#define CONFIG_FILE "/etc/tobii.cfg"
#define CFG_SCREEN_NAME "screen"
#define CFG_GAZE_POINTER_SIZE "gaze_pointer_size"
#define DEFAULT_CFG_GAZE_POINTER_SIZE 100

class RunnerConfig {
public:
    bool load();

private:
public:
    string getSelectedScreen() const;

    void setSelectedScreen(const string &mSelectedScreen);

    int getGazePointerSize() const;

    void setGazePointerSize(int mGazePointerSize);

private:
    string m_selectedScreen;
    int m_gazePointerSize;

    string loadField(const char *fieldName, Config *cfg);
};


#endif //TOBII_IS4_X11_RUNNERCONFIG_H
