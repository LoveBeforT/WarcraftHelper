project(d3d9)

include_directories (
	./
	../3rd/dxsdk
)

link_directories(../3rd/dxsdk/lib/x86)

file(GLOB D3D9_SRCS "./*.cpp" "./*.def")

file(GLOB D3D9_HEADERS "./*.h")

add_library(d3d9 SHARED ${D3D9_SRCS} ${D3D9_HEADERS})

target_link_libraries(d3d9 d3dx9.lib)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../output)
