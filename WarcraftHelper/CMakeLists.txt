project(WarcraftHelper)

include_directories (
	./
	../ThirdParty/Detours
)

link_directories(../ThirdParty/Detours)

file(GLOB WH_SRCS "./*.cpp")

file(GLOB WH_HEADERS "./*.h")

add_library(WarcraftHelper SHARED ${WH_SRCS} ${WH_HEADERS})

target_link_libraries(WarcraftHelper detours.lib)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../output)
