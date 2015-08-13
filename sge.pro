TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
CONFIG +=  -E -dD

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
    sge_perfomance.cpp \
    FrameBuffer.cpp \
    TextureGenerator.cpp \
    experimental/scattering.cpp \
    space/space_object.cpp \
    space/spacesystem.cpp \
    random.cpp \
    experimental/starfield.cpp \
    geometry/umesh.cpp \
    geometry/vertex.cpp \
    geometry/vertexinfo.cpp \
    sge_ui/image_box.cpp \
    sge_texture_lab.cpp \
    sge_ui/dragnumbox.cpp \
    sge_ui/connector.cpp

VERSION = 0.1.0

win32:LIBS += -L$$PWD/3rdparty/lib/ -lopengl32 -lglew32 -lglfw3dll -lfreetype #-lassimp-vc120-mtd
unix:LIBS += -lGL -lGLEW -lglfw3 -lfreetype -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -lXinerama -lXcursor
unix:INCLUDEPATH += /usr/include/freetype2

win32:INCLUDEPATH += $$PWD/3rdparty/include
win32:DEPENDPATH += $$PWD/3rdparty/include

HEADERS += \
    logger.h \
    colorextender.h \
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
    resources/error_image.h \
    FrameBuffer.h \
    TextureGenerator.h \
    resources/random_noise.h \
    sge_fielsystem.h \
    experimental/sphereparamstorage.h \
    experimental/scattering.h \
    data/shaders/scattering/scatter_params.h \
    space/space_object.h \
    space/spacesystem.h \
    space/space_solver.h \
    random.h \
    experimental/starfield.h \
    geometry/umesh.h \
    geometry/vertex.h \
    geometry/vertexinfo.h \
    sge_ui/image_box.h \
    sge_texture_lab.h \
    sge_ui/dragnumbox.h \
    sge_ui/connector.h

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
    data/shaders/height.lib.glsl \
    data/shaders/celltexture.glsl \
    data/shaders/turbulence.lib.glsl \
    data/shaders/testgen1.glsl \
    data/shaders/gradient_builder.glsl \
    data/shaders/float.lib.glsl \
    data/textures/PerlinGrad2D.png \
    data/textures/PerlinPerm2D.png \
    data/shaders/scattering/common.glsl \
    data/shaders/scattering/copyInscatter1.glsl \
    data/shaders/scattering/copyInscatterN.glsl \
    data/shaders/scattering/copyIrradiance.glsl \
    data/shaders/scattering/earth.glsl \
    data/shaders/scattering/inscatter1.glsl \
    data/shaders/scattering/inscatterN.glsl \
    data/shaders/scattering/inscatterS.glsl \
    data/shaders/scattering/irradiance1.glsl \
    data/shaders/scattering/irradianceN.glsl \
    data/shaders/scattering/transmittance.glsl \
    data/shaders/starfield.glsl \
    data/textures/grass.png \
    data/textures/rock.png \
    data/textures/snow.png \
    data/textures/soil.png \
    README.md \
    LICENSE.txt
