cmake_minimum_required(VERSION 3.10)
project(tobii_is4_x11)

set(CMAKE_CXX_STANDARD 14)

add_executable(tobii_is4_x11 main.cpp Shape.h Screen.cpp Screen.h Shape.cpp Runner.cpp Runner.h RunnerConfig.cpp RunnerConfig.h)
target_link_libraries(tobii_is4_x11 /usr/lib/tobii/libtobii_stream_engine.so)

#target_link_libraries(example pthread X11 Xtst Xi X11 Xtst cairo Xfixes Xcomposite Xext X11 Xrender Xrandr GL GLU glut )
target_link_libraries(tobii_is4_x11 config++ pthread X11 Xrandr  Xext)
