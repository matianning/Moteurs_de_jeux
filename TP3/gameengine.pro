QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += main.cpp \
    GameComponent.cpp \
    GameObject.cpp \
    Transform.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    BasicIO.h \
    GameComponent.h \
    GameObject.h \
    Transform.h \
    mainwidget.h \
    geometryengine.h

RESOURCES += \
    Models.qrc \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[YOUR_PATH]
INSTALLS += target

DISTFILES += \
    TP2_ressources/grass.png \
    TP2_ressources/heightmap-1024x1024.png \
    TP2_ressources/rock.png \
    TP2_ressources/snowrocks.png \
    sphere.obj \
    sphere.off
