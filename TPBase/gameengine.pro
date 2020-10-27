QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += main.cpp \
    Cube.cpp \
    GameComponent.cpp \
    GameObject.cpp \
    Transform.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    BasicIO.h \
    Cube.h \
    GameComponent.h \
    GameObject.h \
    Transform.h \
    mainwidget.h \
    geometryengine.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[YOUR_PATH]
INSTALLS += target
