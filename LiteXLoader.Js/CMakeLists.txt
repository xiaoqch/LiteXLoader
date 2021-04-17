cmake_minimum_required (VERSION 3.8)

############  Configs  ############
project ( LiteXLoader.Js )
set (SCRIPTX_BACKEND V8)
set (BACKEND_LIB_DIR_NAME V8)
set (BACKEND_LIBS
v8
v8_libbase
v8_libplatform
zlib
)
set (BACKEND_INCLUDE_DIRS
    ../engine/${BACKEND_LIB_DIR_NAME}/include
    ../engine/${BACKEND_LIB_DIR_NAME}/include/libplatform
)

############  Sources  ############
set (LITE_DLL_SOURCES
    ../src/dllmain.cpp
    ../src/LiteXRunner.cpp
)



############ Working Below ... ############
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

# Main
set (LIBRARY_OUTPUT_PATH ../Output) 
add_library( ${PROJECT_NAME} SHARED ${LITE_DLL_SOURCES} )

# LiteLoader
target_include_directories( ${PROJECT_NAME} PRIVATE ../libs )
target_include_directories( ${PROJECT_NAME} PRIVATE ../libs/LiteLoader/headers )
target_link_directories( ${PROJECT_NAME} PUBLIC ../libs/LiteLoader )

# ScriptX
include_directories( ${BACKEND_INCLUDE_DIRS} )
include_directories( ../src/ScriptX/src/include )
include_directories( ../src/ScriptX/src/include/ScriptX )
include( ../src/ScriptX/CMakeLists.txt )

# Link
add_dependencies(
    ${PROJECT_NAME}
    ScriptX
)
target_link_directories( ${PROJECT_NAME} PUBLIC ../engine/${BACKEND_LIB_DIR_NAME} )
target_link_libraries(
    ${PROJECT_NAME}
    LiteLoader
    ScriptX
    ${BACKEND_LIBS}
)