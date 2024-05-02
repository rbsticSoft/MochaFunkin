cmake_minimum_required(VERSION 3.14)
set(CMAKE_DEBUG_POSTFIX d)

project("FNFC")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -lvorbis -lvorbisfile")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/")

file(GLOB FNF_SOURCES
    "src/common/*.c"
    "src/common/platform/glad.c"
    "src/engine/*.c"
    "src/engine/gl/*.c"
    "src/game/*.c"
    "parson/parson.c"
)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(LibVorbisfile REQUIRED)
find_package(PNG REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(Freetype REQUIRED)
add_executable(FNFC ${FNF_SOURCES})


include_directories(
    "include" 
    "cglm/include" 
    "parson/"
    ${LIBXML2_INCLUDE_DIR} 
    ${OPENAL_INCLUDE_DIR} 
    ${VORBIS_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
)#${VORBIS_INCLUDE_DIR}${VORBISFILE_LIBRARY}

target_link_libraries(FNFC 
    ${PNG_LIBRARY} 
    ${OPENGL_LIBRARY} 
    ${LIBXML2_LIBRARY} 
    SDL2::SDL2 m 
    ${OPENAL_LIBRARY} 
    ${VORBISFILE_LIBRARY} 
    ${FREETYPE_LIBRARIES}
)
