//
// Created by hector on 15-04-19.
//

#ifndef EXAMPLE_SHAPE_H
#define EXAMPLE_SHAPE_H

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <zconf.h>
#include <cstring>


class ShapeWindows {
public:
    Window show(int width, int height);

    void move(int x, int y);

    void paint();

private:
    void paintCircle();
    void removeBorderCircle();
    struct MotifWmHints {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long input_mode;
        unsigned long status;
    };

    static MotifWmHints *getMotifWindowsHints(Display *display, Window window);

    static void removeTittleBar(Display *display, Window window);

private:
    Display *m_display;
    Window m_window;
    int m_width;
    int m_height;
    int m_screen;
};

#endif //EXAMPLE_SHAPE_H
