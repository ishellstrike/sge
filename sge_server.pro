TEMPLATE = app
CONFIG  += console
CONFIG  -= app_bundle
CONFIG  -= qt
CONFIG  += c++11

VERSION = 0.1.0

CONFIG(debug, debug|release) {
    LIBS   += -L$$PWD/3rdparty/lib_debug/
    LIBS   += -llibboost_log-vc120-mt-gd-1_59 -lsquirrel -lsqstdlib  -lfreetype262d
}
else {
    LIBS   += -L$$PWD/3rdparty/lib_release/
    LIBS   += -llibboost_log-vc120-mt-1_59 -lboost_system-vc120-mt-1_59 -lboost_thread-vc120-mt-1_59 -lsquirrel -lsqstdlib  -lfreetype262
}

win32:LIBS += \
-L$$PWD/3rdparty/boost/stage/lib \
-lopengl32 -lglew32 -lglfw3dll -lOpenAL32 -llibogg -llibvorbisfile

unix:LIBS        += -lGL -lGLEW -lglfw3 -lfreetype -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -lXinerama -lXcursor
unix:INCLUDEPATH += /usr/include/freetype2

win32:INCLUDEPATH += $$PWD/3rdparty/include
win32:INCLUDEPATH += $$PWD/3rdparty/boost

HEADERS += \
    helper.h \
    logger.h \
    core/agents/agent.h \
    core/agents/agentfactory.h \
    core/agents/agents.hpp \
    core/agents/aggressive.h \
    core/agents/block.h \
    core/agents/blockbase.h \
    core/agents/chest.h \
    core/agents/creaturebase.h \
    core/agents/entity.h \
    core/agents/itembase.h \
    core/agents/itemspawner.h \
    core/agents/material.h \
    core/agents/onenter.h \
    core/agents/passsound.h \
    core/agents/pressureplate.h \
    core/agents/referencer.h \
    core/agents/script.h \
    core/agents/simpleinteract.h \
    core/agents/stacked.h \
    core/agents/structure.h \
    core/agents/tags.h \
    core/agents/transparent.h \
    core/agents/walkable.h \
    core/agents/wander.h \
    core/events/event.h \
    core/events/eventbus.h \
    core/events/eventdamage.h \
    core/events/eventissureorder.h \
    core/infrastructure/player.h \
    core/infrastructure/world.h \
    core/core_const.h \
    core/db.h \
    core/dynamicobject.h \
    core/level.h \
    core/levelgen.h \
    core/object.h \
    core/objecthelper.h \
    core/objectstatic.h \
    core/offset.h \
    core/recipe.h \
    core/scheme.h \
    core/sector.h \
    core/serialize.h \
    core/serializer.h \
    core/tree.h \
    core/uniqueid.h \
    rapidjson/internal/pow10.h \
    rapidjson/internal/stack.h \
    rapidjson/internal/strfunc.h \
    rapidjson/document.h \
    rapidjson/filestream.h \
    rapidjson/genericstream.h \
    rapidjson/prettywriter.h \
    rapidjson/rapidjson.h \
    rapidjson/reader.h \
    rapidjson/stringbuffer.h \
    rapidjson/writer.h \
    utfcpp/utf8/checked.h \
    utfcpp/utf8/core.h \
    utfcpp/utf8/unchecked.h \
    utfcpp/utf8.h \
    fpscounter.h \
    gametimer.h \
    remsnd.h \
    colorextender.h \
    random.h \
    core/agents/sound.h \
    prefecences.h \
    network.h \
    core/network/remoteclient.h \
    core/network/packet.h \
    core/network/packetrequestsector.h \
    core/network/packetholder.h \
    core/network/packetresponsesector.h \
    core/network/remoteserver.h \
    core/network/packetrequestplayers.h \
    core/network/packetresponseplayers.h \
    core/playeremplacer.h \
    core/emplacers.h

SOURCES += \
    helper.cpp \
    logger.cpp \
    core/agents/agent.cpp \
    core/agents/aggressive.cpp \
    core/agents/block.cpp \
    core/agents/blockbase.cpp \
    core/agents/chest.cpp \
    core/agents/creaturebase.cpp \
    core/agents/entity.cpp \
    core/agents/itembase.cpp \
    core/agents/itemspawner.cpp \
    core/agents/material.cpp \
    core/agents/onenter.cpp \
    core/agents/passsound.cpp \
    core/agents/pressureplate.cpp \
    core/agents/referencer.cpp \
    core/agents/script.cpp \
    core/agents/simpleinteract.cpp \
    core/agents/stacked.cpp \
    core/agents/structure.cpp \
    core/agents/tags.cpp \
    core/agents/transparent.cpp \
    core/agents/walkable.cpp \
    core/agents/wander.cpp \
    core/events/event.cpp \
    core/events/eventbus.cpp \
    core/events/eventdamage.cpp \
    core/events/eventissureorder.cpp \
    core/infrastructure/player.cpp \
    core/infrastructure/world.cpp \
    core/db.cpp \
    core/dynamicobject.cpp \
    core/level.cpp \
    core/levelgen.cpp \
    core/object.cpp \
    core/objecthelper.cpp \
    core/objectstatic.cpp \
    core/offset.cpp \
    core/recipe.cpp \
    core/network/remoteclient.cpp \
    core/scheme.cpp \
    core/sector.cpp \
    core/tree.cpp \
    core/uniqueid.cpp \
    main_server.cpp \
    fpscounter.cpp \
    gametimer.cpp \
    remsnd.cpp \
    colorextender.cpp \
    random.cpp \
    core/agents/sound.cpp \
    prefecences.cpp \
    network.cpp \
    core/network/packetrequestsector.cpp \
    core/network/packetholder.cpp \
    core/network/packetresponsesector.cpp \
    core/network/remoteserver.cpp \
    core/network/packetrequestplayers.cpp \
    core/network/packetresponseplayers.cpp \
    core/playeremplacer.cpp \
    core/emplacers.cpp
