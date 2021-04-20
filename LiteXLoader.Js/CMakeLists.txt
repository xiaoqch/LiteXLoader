cmake_minimum_required (VERSION 3.8)

############  Configs  ############
project ( "LiteXLoader.Js" )
set (SCRIPTX_BACKEND "V8")
set (BACKEND_LIB_DIR_NAME "V8")



############  Help Funcs  ############
macro(SUBDIRLIST result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

############ Working Below ... ############
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

# Main
set (LIBRARY_OUTPUT_PATH "../Output") 
file(GLOB_RECURSE LITE_DLL_SOURCES "../src/*.cpp")
add_library( ${PROJECT_NAME} SHARED ${LITE_DLL_SOURCES} )

# LiteLoader
target_include_directories( ${PROJECT_NAME} PRIVATE "../libs" )
target_include_directories( ${PROJECT_NAME} PRIVATE "../src/LiteLoader/headers" )
target_link_directories( ${PROJECT_NAME} PUBLIC "../libs/LiteLoader" )

# ScriptX
SUBDIRLIST (BACKEND_INCLUDE_DIRS "../engine/${BACKEND_LIB_DIR_NAME}/include")
include_directories( "../engine/${BACKEND_LIB_DIR_NAME}/include" )
include_directories( ${BACKEND_INCLUDE_DIRS} )
include_directories( "../src/ScriptX/src/include" )
include_directories( "../src/ScriptX/src/include/ScriptX" )
include( "../src/ScriptX/CMakeLists.txt" )

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
)

# Backend Libs Link
FILE(GLOB_RECURSE BACKEND_LIBS "../engine/${BACKEND_LIB_DIR_NAME}/*.lib")
FOREACH(LIB_PATH ${BACKEND_LIBS})
    STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" LIB_NAME ${LIB_PATH})
    target_link_libraries(${PROJECT_NAME} ${LIB_NAME})
ENDFOREACH(LIB_PATH)