//
// Created by Hector Oliveros on 15-04-19.

#include <utility>
#include "Screen.h"


ScreenManager::ScreenManager(std::string primaryDisplayName, bool emulateHeadPose) {
    m_readBufferIdx = 0;
    m_emulateHeadPose = emulateHeadPose;
    m_primaryDisplayName = std::move(primaryDisplayName);
    m_primaryDisplayIdx = 0;
    m_display = XOpenDisplay(nullptr);
    m_screenNumber = DefaultScreen(m_display);
    m_window = RootWindow(m_display, m_screenNumber);
    loadMonitorInfo();
}


std::vector<XRRMonitorInfo> ScreenManager::listMonitors() {
    static int n, minWidth, maxWidth, minHeight, maxHeight;
    XRRGetScreenSizeRange(m_display, m_window, &minWidth, &minHeight,
                          &maxWidth, &maxHeight);
    m_xranrdRes = XRRGetScreenResources(m_display, m_window);
    XRRMonitorInfo *m;
    m = XRRGetMonitors(m_display, m_window, true, &n);
    if (n == -1 || m == nullptr || n == 0) {
        printf("ERROR: listMonitors()\n");
        return std::vector<XRRMonitorInfo>();
    }
    for (int i = 0; i < n; ++i) {
        if (XGetAtomName(m_display, m[i].name) == m_primaryDisplayName) {
            m_primaryDisplayIdx = i;
        }
    }
    std::vector<XRRMonitorInfo> screens(m, m + n);
    return screens;
}

void ScreenManager::loadMonitorInfo() {
    m_monitors = listMonitors();
}

std::array<int, 2> ScreenManager::relativePointToCoor(float x, float y) {
    bool isMovedToLeftWindows = false;
    bool isMovedToRightWindows = false;
    if (m_emulateHeadPose) {
        m_readBufferIdx = (m_readBufferIdx + 1) % READ_BUFFER_SIZE;
        m_readBufferX[m_readBufferIdx] = x;
        isMovedToLeftWindows = true;
        isMovedToRightWindows = true;
        for (float sample : m_readBufferX) {
            if (sample > 0.05) {
                isMovedToLeftWindows = false;
            }
            if (sample < 0.94) {
                isMovedToRightWindows = false;
            }
        }
    }
    int displayToSet = m_primaryDisplayIdx;
    if (isMovedToLeftWindows || (x < 0 && m_primaryDisplayIdx > 0)) {
        displayToSet--;
    } else if (isMovedToRightWindows || (x > 0 && m_primaryDisplayIdx < m_monitors.size() - 1)) {
        displayToSet++;
    }
    if (y > 1) {
        y = 1;
    } else if (y < 0) {
        y = 0;
    }
    auto m = &m_monitors[displayToSet];
    if (displayToSet != m_primaryDisplayIdx) {
        // set to center of the other m_screen
        return {
                m->x + (m->width / 2),
                m->y + (m->height / 2)
        };
    }
    return {
            int(m->x + (m->width * x)),
            int(m->y + (m->height * y))
    };
}
/*
// Like Arduino :)
int main() {
    auto sm = ScreenManager("HDMI-1-2");

}

*/
