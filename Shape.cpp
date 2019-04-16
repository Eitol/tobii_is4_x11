//
// Created by hector on 15-04-19.
//

#include "Shape.h"

Window ShapeWindows::show(int width, int height) {
    m_width = width;
    m_height = height;

    /* open connection with the server */
    m_display = XOpenDisplay(NULL);
    if (m_display == NULL) {
        printf("Cannot open m_display\n");
        exit(1);
    }
    m_screen = DefaultScreen(m_display);

    /* create m_window, select events, map  */
    m_window = XCreateSimpleWindow(
            m_display,                /* m_display */
            RootWindow(m_display, m_screen), /* parent */
            0,                     /* x */
            0,                     /* y */
            m_width + 1,                    /* width */
            m_width + 1,                    /* height */
            5,                      /* border_width */
            BlackPixel(m_display, m_screen), /* border */
            -63                    /* background */
    );
    double alpha = 0.7;
    unsigned long opacity = (unsigned long) (0xFFFFFFFFul * alpha);
    Atom XA_NET_WM_WINDOW_OPACITY = XInternAtom(m_display, "_NET_WM_WINDOW_OPACITY", False);
    XChangeProperty(m_display, m_window, XA_NET_WM_WINDOW_OPACITY, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *) &opacity, 1L);
    XMapWindow(m_display, m_window);
    paintCircle();

    Atom window_type = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE", False);
    long value = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DOCK", False);
    XChangeProperty(m_display, m_window, window_type,
                    XA_ATOM, 32, PropModeReplace, (unsigned char *) &value, 1);

    removeTittleBar(m_display, m_window);
    return m_window;
}

void ShapeWindows::paintCircle() {
    Pixmap pixmap;
    GC gp;
    /* create the pixmap that specifies the shape */
    pixmap = XCreatePixmap(
            m_display,   // m_display
            m_window,    // m_window
            m_width + 1, // width
            m_width + 1, // height
            1            // depth
    );
    gp = XCreateGC(m_display, pixmap, 0, nullptr);
    XSetForeground(m_display, gp, WhitePixel(m_display, m_screen));
    XFillRectangle(m_display, pixmap, gp, 0, 0, 400, 300);
    XSetForeground(m_display, gp, BlackPixel(m_display, m_screen));
    XFillArc(
            m_display,  // m_display
            pixmap,   // drawable
            gp,       // gc ( graphics context )
            0,      // x
            0,      // y
            m_width,      // with
            m_height,      // height
            0,        // angle1
            360 * 64  // angle2
    );

    /* set the pixmap as the new m_window mask;
    the pixmap is slightly larger than the m_window
    to allow for the m_window border and title bar
    (as added by the m_window manager) to be visible */
    XShapeCombineMask(
            m_display,      /* display */
            m_window,       /* dest */
            ShapeBounding,  /* dest_kind */
            0,              /* x_off */
            0,              /* y_off */
            pixmap,         /* src */
            ShapeSet        /* op */
    );
}

void ShapeWindows::removeBorderCircle() {
    Pixmap pixmap;
    GC gp;
    /* create the pixmap that specifies the shape */
    pixmap = XCreatePixmap(
            m_display, // m_display
            m_window,  // m_window
            m_width + 1,     // width
            m_width + 1,     // height
            1        // depth
    );
    gp = XCreateGC(
            m_display, /* display */
            pixmap,    /* d */
            0,         /* valuemask */
            nullptr    /* values */
    );
    XSetForeground(
            m_display,    /* display */
            gp,             /* gc */
            WhitePixel(     /* foreground */
                    m_display, // display
                    m_screen   // screen
            )
    );
    XFillRectangle(m_display, pixmap, gp, 0, 0, 400, 300);
    XSetForeground(m_display, gp, BlackPixel(m_display, m_screen));
    XFillArc(
            m_display,  // m_display
            pixmap,   // drawable
            gp,       // gc ( graphics context )
            0,      // x
            0,      // y
            m_width,      // with
            m_height,      // height
            0,        // angle1
            360 * 64  // angle2
    );

    /* set the pixmap as the new m_window mask;
    the pixmap is slightly larger than the m_window
    to allow for the m_window border and title bar
    (as added by the m_window manager) to be visible */
    XShapeCombineMask(
            m_display,      /* display */
            m_window,       /* dest */
            ShapeClip,  /* dest_kind */
            0,              /* x_off */
            0,              /* y_off */
            pixmap,         /* src */
            ShapeSubtract   /* op */
    );
}

void ShapeWindows::move(int x, int y) {
    XMoveWindow(m_display, m_window, x - (m_width / 2), y - (m_height / 2));
    XFlush(m_display);
}


/* UTILS */

ShapeWindows::MotifWmHints *ShapeWindows::getMotifWindowsHints(Display *display, Window window) {
    Atom property;
    int result;
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *data;

    property = XInternAtom(display, "_MOTIF_WM_HINTS", False);
    result = XGetWindowProperty(display, window, property,
                                0, LONG_MAX, False, AnyPropertyType,
                                &actual_type, &actual_format,
                                &nitems, &bytes_after, &data);

    if (result == Success && data != NULL) {
        size_t data_size;
        size_t max_size;
        MotifWmHints *hints;

        data_size = nitems * sizeof(long);
        max_size = sizeof(*hints);

        hints = static_cast<MotifWmHints *>(calloc(1, max_size));

        memcpy(hints, data, data_size > max_size ? max_size : data_size);
        XFree(data);

        return hints;
    }

    return NULL;
}

void ShapeWindows::removeTittleBar(Display *display, Window window) {
    MotifWmHints *hints;
    Atom property;
    int nelements;

    hints = getMotifWindowsHints(display, window);
    if (hints == nullptr) {
        hints = static_cast<MotifWmHints *>(calloc(1, sizeof(*hints)));
        hints->decorations = (1L << 0);
    }

    hints->flags |= (1L << 1);
    hints->decorations = hints->decorations == 0 ? (1L << 0) : 0;

    property = XInternAtom(display, "_MOTIF_WM_HINTS", False);
    nelements = sizeof(*hints) / sizeof(long);

    XChangeProperty(display, window, property, property, 32, PropModeReplace,
                    (unsigned char *) hints, nelements);

    free(hints);
}

void ShapeWindows::paint() {

}


