QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += main.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h

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
