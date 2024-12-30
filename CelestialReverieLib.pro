TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

TARGET = CelestialReverieLib
DESTDIR = ../build
 
SOURCES += \
    # include/glm/detail/glm.cpp \
    src/simulation/celestial_body.cpp \
    src/simulation/celestial_system.cpp \
    src/simulation/celestial_reverie.cpp \
    src/simulation/simulate_algorithm.cpp \
    src/simulation/simulate_algorithm_parallel.cpp

include(include/include.pri)

win32: LIBS += -L$$PWD/lib/ -lgomp