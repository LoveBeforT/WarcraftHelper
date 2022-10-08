project(WarcraftHelper)

include_directories (
	./
	../ThirdParty/Detours
)

link_directories(../ThirdParty/Detours)

file(GLOB WH_SRCS "./*.cpp")

add_library(WarcraftHelper SHARED ${WH_SRCS})

target_link_libraries(WarcraftHelper detours.lib)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/output)
