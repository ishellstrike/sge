TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += \
    logger.cpp \
    shader.cpp \
    json/json_reader.cpp \
    json/json_value.cpp \
    json/json_writer.cpp \
    lodepng/lodepng.cpp \
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
    basicjargshader.cpp \
    SphereTesselator.cpp \
    ClassicNoise.cpp \
    camera.cpp \
    geometry/cube.cpp \
    geometry/icosahedron.cpp \
    geometry/mesh.cpp \
    geometry/quad.cpp \
    geometry/vpnt.cpp \
    geometry/model.cpp \
    sge_ui/button.cpp \
    sge_ui/component.cpp \
    sge_ui/graph.cpp \
    sge_ui/label.cpp \
    sge_ui/list_container.cpp \
    sge_ui/panel.cpp \
    sge_ui/vertical_bar.cpp \
    sge_ui/win.cpp \
    sge_ui/wins.cpp \
    colorscheme.cpp \
    gbuffer.cpp \
    experimental/quadsphere.cpp \
    experimental/quadplane.cpp \
    resources/resourcecontroller.cpp \
    resources/material.cpp \
    resources/texture.cpp \
    resources/pixmap.cpp \
    sge_perfomance.cpp

VERSION = 0.1.0

win32:LIBS += -L$$PWD/3rdparty/lib/ -lopengl32 -lglew32 -lglfw3dll -lfreetype #-lassimp-vc120-mtd
unix:LIBS += -lGL -lGLEW -lglfw3 -lfreetype -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -lXinerama -lXcursor
unix:INCLUDEPATH += /usr/include/freetype2

win32:INCLUDEPATH += $$PWD/3rdparty/include
win32:DEPENDPATH += $$PWD/3rdparty/include

HEADERS += \
    logger.h \
    colorextender.h \
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
    basicjargshader.h \
    tangentcalc.h \
    SphereTesselator.h \
    utfcpp/utf8.h \
    helper.h \
    ClassicNoise.h \
    camera.h \
    geometry/icosahedron.h \
    geometry/cube.h \
    geometry/mesh.h \
    geometry/geometry.hpp \
    geometry/quad.h \
    geometry/vpnt.h \
    geometry/model.h \
    sge_ui/button.h \
    sge_ui/component.h \
    sge_ui/graph.h \
    sge_ui/label.h \
    sge_ui/list_container.h \
    sge_ui/panel.h \
    sge_ui/vertical_bar.h \
    sge_ui/wcontainer.h \
    sge_ui/win.h \
    sge_ui/wins.h \
    colorscheme.h \
    gbuffer.h \
    experimental/quadsphere.h \
    experimental/quadplane.h \
    resources/resourcecontroller.h \
    resources/material.h \
    resources/pixmap.h \
    resources/texture.h \
    sge_perfomance.h \
    resources/error_image.h

DISTFILES += \
    data/fonts/DejaVuSansMono.ttf \
    data/shaders/basic.glsl \
    data/shaders/color.glsl \
    data/shaders/font.glsl \
    data/textures/cone1.png \
    data/shaders/minimal.glsl \
    sge_ui/.git \
    data/shaders/noise.lib.glsl \
    data/shaders/atmosphere.lib.glsl \
    data/shaders/test1.glsl \
    data/shaders/test2.glsl \
    data/shaders/minimal_watertest.glsl \
    data/shaders/height.lib.glsl
