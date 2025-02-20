project(WarcraftHelper)

include_directories (
	./
	../3rd/detours
	../3rd/simpleini
)

file (
	GLOB CONFIG_FILES
	"./config/*.cpp"
	"./config/*.hpp"
)

file (
	GLOB GAME_FILES
	"./game/*.cpp"
	"./game/*.hpp"
)

file (
	GLOB PLUGIN_FILES
	"./plugin/*.cpp"
	"./plugin/*.hpp"
)

link_directories(../3rd/Detours)

source_group("config" FILES ${CONFIG_FILES})
source_group("game" FILES ${GAME_FILES})
source_group("plugin" FILES ${PLUGIN_FILES})

file(GLOB WH_SRCS "./*.cpp")

file(GLOB WH_HEADERS "./*.hpp")

add_library(WarcraftHelper SHARED ${WH_SRCS} ${WH_HEADERS} ${CONFIG_FILES} ${GAME_FILES} ${PLUGIN_FILES})

target_link_libraries(WarcraftHelper detours.lib)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../output)
