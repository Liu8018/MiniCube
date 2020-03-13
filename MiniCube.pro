TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        MiniCubeFunctions.cpp \
        MiniCubeSolver.cpp \
        MiniCubeVisualizer.cpp \
        main.cpp

HEADERS += \
    MiniCubeFunctions.h \
    MiniCubeSolver.h \
    MiniCubeVisualizer.h

INCLUDEPATH += /home/liu/libraries/opencv345/include
LIBS += -L/home/liu/libraries/opencv345/lib -lopencv_core -lopencv_highgui -lopencv_imgproc
