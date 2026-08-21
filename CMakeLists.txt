cmake_minimum_required(VERSION 3.15)
project(buildoemu)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(APP_DIR ${CMAKE_CURRENT_LIST_DIR}/src/application)
set(ENGINE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/engine)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

include(engine.cmake)

add_executable(buildoemu
    ${APP_DIR}/ServerMain.cpp
    ${APP_DIR}/ServerApplication.cpp
    ${APP_DIR}/DedicatedServer.cpp
    
    ${APP_DIR}/player/PlayerClient.cpp
    ${APP_DIR}/player/PlayerManager.cpp
    ${APP_DIR}/player/PlayerInventory.cpp
    ${APP_DIR}/player/command/PlayerCommands.cpp
    ${APP_DIR}/player/command/PlayerCommandManager.cpp

    ${APP_DIR}/gt/Protocol.cpp

    ${APP_DIR}/common/Proton.cpp
    ${APP_DIR}/common/Log.cpp

    ${APP_DIR}/items/ItemInfo.cpp
    ${APP_DIR}/items/ItemInfoUtils.cpp
    ${APP_DIR}/items/ItemManager.cpp

    ${APP_DIR}/world/World.cpp
    ${APP_DIR}/world/WorldTileMap.cpp
    ${APP_DIR}/world/WorldObjectMap.cpp
    ${APP_DIR}/world/Tile.cpp
    ${APP_DIR}/world/TileExtra.cpp
    ${APP_DIR}/world/WorldManager.cpp
    #${APP_DIR}/world/WorldObject.cpp

    ${PROTON_FILES}
)

target_compile_features(buildoemu PRIVATE cxx_std_20)

target_include_directories(buildoemu PRIVATE
    ${APP_DIR}
    ${ENGINE_DIR}
    ${ENGINE_DIR}/util/boost
    ${ENGINE_DIR}/ClanLib-2.0/Sources
    ${ENGINE_DIR}/Network/enet/include
)

# Preprocessor defines proton needs.
target_compile_definitions(buildoemu PRIVATE
    _CONSOLE
    HAS_SOCKLEN_T
    BOOST_ALL_NO_LIB
    C_NO_ZLIB
    RTLINUX
    RT_CUSTOM_LOGMSG
)