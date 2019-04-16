//
// Created by todova on 15-04-19.
//

#ifndef EXAMPLE_SCREEN_H
#define EXAMPLE_SCREEN_H


#include <iostream>
#include <map>
#include <string>
#include <iterator>
#include <X11/extensions/Xrandr.h>
#include <X11/Xlib.h>
#include <vector>

#define READ_BUFFER_SIZE 2

class ScreenManager {
public:
    ScreenManager(std::string primaryDisplayName, bool emulateHeadPose);

    std::array<int, 2> relativePointToCoor(float x, float y);
    std::vector<XRRMonitorInfo> listMonitors();

private:
    void loadMonitorInfo();

private:
    std::vector<XRRMonitorInfo> m_monitorInfo;
    Display *m_display;
    int m_screenNumber;
    Window m_window;
    std::vector<XRRMonitorInfo> m_monitors;
    XRRScreenResources *m_xranrdRes;
    std::string m_primaryDisplayName;
    int m_primaryDisplayIdx;
    bool m_emulateHeadPose;
    float m_readBufferX[READ_BUFFER_SIZE];
    int m_readBufferIdx;
};


#endif //EXAMPLE_SCREEN_H
