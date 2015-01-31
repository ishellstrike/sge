TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += \
    logger.cpp \
    pixmap.cpp \
    texture.cpp \
    shader.cpp \
    json/json_reader.cpp \
    json/json_value.cpp \
    json/json_writer.cpp \
    lodepng/lodepng.cpp \
    helper.inl \
    colorextender.cpp \
    textureatlas.cpp \
    spritebatch.cpp \
    prefecences.cpp \
    gamewindow.cpp \
    gametimer.cpp \
    fpscounter.cpp \
    keyboard.cpp \
    mouse.cpp \
    singleton.cpp \
    font.cpp \
    json/json_internalarray.inl \
    json/json_internalmap.inl \
    json/json_valueiterator.inl \
    main.cpp \
    mesh.cpp \
    material.cpp \
    basicjargshader.cpp \
    vpnt.cpp \
    icosahedron.cpp \
    cube.cpp \
    SphereTesselator.cpp

VERSION = 0.1.0

win32:LIBS += -L$$PWD/3rdparty/lib/ -lopengl32 -lglew32 -lglfw3dll -lfreetype
unix:LIBS += -lGL -lGLEW -lglfw3 -lfreetype -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ljson_linux-gcc-4.8_libmt
unix:INCLUDEPATH += /usr/include/freetype2

win32:INCLUDEPATH += $$PWD/3rdparty/include
win32:DEPENDPATH += $$PWD/3rdparty/include

HEADERS += \
    logger.h \
    colorextender.h \
    texture.h \
    pixmap.h \
    fielsystem.h \
    shader.h \
    json/autolink.h \
    json/config.h \
    json/features.h \
    json/forwards.h \
    json/json.h \
    json/json_batchallocator.h \
    json/reader.h \
    json/value.h \
    json/writer.h \
    lodepng/lodepng.h \
    textureatlas.h \
    spritebatch.h \
    singleton.h \
    prefecences.h \
    sge.h \
    gamewindow.h \
    gametimer.h \
    fpscounter.h \
    keyboard.h \
    mouse.h \
    font.h \
    utfcpp/utf8.h \
    utfcpp/utf8/checked.h \
    utfcpp/utf8/core.h \
    utfcpp/utf8/unchecked.h \
    mesh.h \
    material.h \
    basicjargshader.h \
    vpnt.h \
    icosahedron.h \
    cube.h \
    tangentcalc.h \
    SphereTesselator.h

DISTFILES += \
    data/fonts/DejaVuSansMono.ttf \
    data/shaders/basic.glsl \
    data/shaders/color.glsl \
    data/shaders/font.glsl \
    data/textures/cone1.png \
    data/shaders/minimal.glsl

