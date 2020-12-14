QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += main.cpp \
    BoundingSphere.cpp \
    gameObject.cpp \
    physicsEngine.cpp \
    transform.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    BasicIO.h \
    BoundingSphere.h \
    IntersectData.h \
    gameObject.h \
    mainwidget.h \
    geometryengine.h \
    physicsEngine.h \
    transform.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[YOUR_PATH]
INSTALLS += target

DISTFILES += \
    TP2_ressources/grass.png \
    TP2_ressources/heightmap-1024x1024.png \
    TP2_ressources/rock.png \
    TP2_ressources/snowrocks.png
