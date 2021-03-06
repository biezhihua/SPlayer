# 设置库名称
project(splayer_andorid)

# 设置cmake最低版本
cmake_minimum_required(VERSION 3.10.2)

# 设置C++版本
set(CMAKE_CXX_STANDARD 11)

# 设置Root目录
get_filename_component(ANDROID_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../../../ ABSOLUTE)

# 设置依赖库路径
get_filename_component(ANDROID_DISTRIBUTION_ANDROID_DIR ${ANDROID_ROOT_DIR}/distribution/android/ ABSOLUTE)

# 设置ffmpeg依赖库
add_library(splayer_ffmpeg_libavcodec SHARED IMPORTED)
set_target_properties(splayer_ffmpeg_libavcodec PROPERTIES IMPORTED_LOCATION ${ANDROID_DISTRIBUTION_ANDROID_DIR}/ffmpeg/lib/${ANDROID_ABI}/libavcodec.so)

add_library(splayer_ffmpeg_libavdevice SHARED IMPORTED)
set_target_properties(splayer_ffmpeg_libavdevice PROPERTIES IMPORTED_LOCATION ${ANDROID_DISTRIBUTION_ANDROID_DIR}/ffmpeg/lib/${ANDROID_ABI}/libavdevice.so)

add_library(splayer_ffmpeg_libavfilter SHARED IMPORTED)
set_target_properties(splayer_ffmpeg_libavfilter PROPERTIES IMPORTED_LOCATION ${ANDROID_DISTRIBUTION_ANDROID_DIR}/ffmpeg/lib/${ANDROID_ABI}/libavfilter.so)

add_library(splayer_ffmpeg_libavformat SHARED IMPORTED)
set_target_properties(splayer_ffmpeg_libavformat PROPERTIES IMPORTED_LOCATION ${ANDROID_DISTRIBUTION_ANDROID_DIR}/ffmpeg/lib/${ANDROID_ABI}/libavformat.so)

add_library(splayer_ffmpeg_libavutil SHARED IMPORTED)
set_target_properties(splayer_ffmpeg_libavutil PROPERTIES IMPORTED_LOCATION ${ANDROID_DISTRIBUTION_ANDROID_DIR}/ffmpeg/lib/${ANDROID_ABI}/libavutil.so)

add_library(splayer_ffmpeg_libswresample SHARED IMPORTED)
set_target_properties(splayer_ffmpeg_libswresample PROPERTIES IMPORTED_LOCATION ${ANDROID_DISTRIBUTION_ANDROID_DIR}/ffmpeg/lib/${ANDROID_ABI}/libswresample.so)

add_library(splayer_ffmpeg_libswscale SHARED IMPORTED)
set_target_properties(splayer_ffmpeg_libswscale PROPERTIES IMPORTED_LOCATION ${ANDROID_DISTRIBUTION_ANDROID_DIR}/ffmpeg/lib/${ANDROID_ABI}/libswscale.so)

## 设置openssl依赖库
#add_library(splayer_openssl_crypto STATIC IMPORTED)
#set_target_properties(splayer_openssl_crypto PROPERTIES IMPORTED_LOCATION ${ANDROID_DISTRIBUTION_ANDROID_DIR}/openssl/lib/${ANDROID_ABI}/libcrypto.a)
#
## 设置openssl依赖库
#add_library(splayer_openssl_ssl STATIC IMPORTED)
#set_target_properties(splayer_openssl_ssl PROPERTIES IMPORTED_LOCATION ${ANDROID_DISTRIBUTION_ANDROID_DIR}/openssl/lib/${ANDROID_ABI}/libssl.a)

# 根据API版本判断使用哪个版本的OpenGLES
if (${ANDROID_PLATFORM_LEVEL} LESS 12)
    message(FATAL_ERROR "OpenGL 2 is not supported before API level 11 \
                      (currently using ${ANDROID_PLATFORM_LEVEL}).")
    return()
elseif (${ANDROID_PLATFORM_LEVEL} LESS 18)
    add_definitions("-DDYNAMIC_ES3")
    set(GL3STUB_SRC gl3stub.c)
    set(OPENGL_LIB GLESv2)
else ()
    set(OPENGL_LIB GLESv3)
endif (${ANDROID_PLATFORM_LEVEL} LESS 12)

# 添加引擎子模块目录
add_subdirectory(${ANDROID_ROOT_DIR}/splayer_engine splayer_engine)

add_subdirectory(${ANDROID_ROOT_DIR}/splayer_renderer splayer_renderer)

# 添加源码子目录
file(GLOB ANDROID_CPP_FILES
        src/*.cpp
        *.cpp
        )

# 添加头文件子目录
file(GLOB ANDROID_H_FILES
        include/*.h
        *.h
        )

# 设置源文件集合
set(ANDROID_SOURCE_FILES ${ANDROID_CPP_FILES} ${ANDROID_H_FILES})

# 设置头文件
include_directories(

        # 设置ffmpeg头文件目录
        ${ANDROID_DISTRIBUTION_ANDROID_DIR}/ffmpeg/include

        # 设置openssl头文件目录
        ${ANDROID_DISTRIBUTION_ANDROID_DIR}/openssl/include

        # 引入引擎头文件
        ${ANDROID_ROOT_DIR}/splayer_engine/include
        ${ANDROID_ROOT_DIR}/splayer_engine

        # 引入渲染器头文件
        ${ANDROID_ROOT_DIR}/splayer_renderer/include
        ${ANDROID_ROOT_DIR}/splayer_renderer

        # 引入SoundTouch头文件
        ${ANDROID_ROOT_DIR}/splayer_soundtouch/include
        ${ANDROID_ROOT_DIR}/splayer_soundtouch

        # 引入自有代码头文件
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/renderer
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 设置库
add_library(${PROJECT_NAME}
        SHARED
        ${ANDROID_SOURCE_FILES})

# EGL
# https://woshijpf.github.io/android/2017/09/04/Android%E7%B3%BB%E7%BB%9F%E5%9B%BE%E5%BD%A2%E6%A0%88OpenGLES%E5%92%8CEGL%E4%BB%8B%E7%BB%8D.html
# 链接静态库
target_link_libraries(${PROJECT_NAME}
        splayer_engine
        splayer_renderer
        -landroid
        -llog
        -lEGL
        -lOpenSLES
        ${OPENGL_LIB}
        )
