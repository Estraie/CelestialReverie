QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = CelestialReverie

SOURCES += \
    include/glm/detail/glm.cpp \
    lib/glad.c \
    main.cpp \
    mainwindow.cpp \
    src/simulation/celestial_body.cpp \
    src/simulation/celestial_system.cpp \
    src/simulation/celestial_reverie.cpp \
    src/simulation/simulate_algorithm.cpp \
    src/rendering/celestialGLWidget.cpp
    
include(include/include.pri)
    
win32: LIBS += -L$$PWD/lib/ -lglfw3 -lgdi32    

HEADERS += \
    include/rendering/camera.h \
    include/rendering/celestialGLWidget.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    include/glm/CMakeLists.txt \
    include/glm/copying.txt
