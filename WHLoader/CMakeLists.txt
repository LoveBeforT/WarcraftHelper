project(WHLoader)

include_directories (
	./
)

file(GLOB WHLoader_SRCS "./*.cpp")

add_library(WHLoader SHARED ${WHLoader_SRCS})

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../output)

set_target_properties(WHLoader PROPERTIES SUFFIX ".mix")
set_target_properties(WHLoader PROPERTIES OUTPUT_NAME "WarcraftHelper")
