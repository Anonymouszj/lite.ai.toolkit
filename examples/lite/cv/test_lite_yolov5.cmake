# 1. setup 3rd-party dependences
message(">>>> Current project is [lite_yolov5] in : ${CMAKE_CURRENT_SOURCE_DIR}")
include(${CMAKE_SOURCE_DIR}/setup_3rdparty.cmake)

if (APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_BUILD_TYPE release)
endif ()

# 2. setup onnxruntime include
include_directories(${ONNXRUNTIME_INCLUDE_DIR})
link_directories(${ONNXRUNTIME_LIBRARY_DIR})

# 3. will be include into CMakeLists.txt at examples/ort
if(LITEHUB_BUILD_TEST_SRCS)
    set(LITE_YOLOV5_SRCS cv/test_lite_yolov5.cpp
            ${LITEHUB_ROOT_DIR}/ort/cv/yolov5.cpp
            ${LITEHUB_ROOT_DIR}/ort/core/ort_utils.cpp
            ${LITEHUB_ROOT_DIR}/ort/core/ort_handler.cpp
            )
else()
    set(LITE_YOLOV5_SRCS cv/test_lite_yolov5.cpp)
endif()

add_executable(lite_yolov5 ${LITE_YOLOV5_SRCS})

if(LITEHUB_BUILD_TEST_SRCS)
    target_link_libraries(lite_yolov5 onnxruntime ${OpenCV_LIBS})
else()
    target_link_libraries(lite_yolov5 litehub)  # link liblitehub
endif()

if (LITEHUB_COPY_BUILD)
    # "set" only valid in the current directory and subdirectory and does not broadcast
    # to parent and sibling directories
    # CMAKE_SOURCE_DIR means the root path of top CMakeLists.txt
    # CMAKE_CURRENT_SOURCE_DIR the current path of current CMakeLists.txt
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/litehub/bin)
    message("=================================================================================")
    message("output binary [app: lite_yolov5] to ${EXECUTABLE_OUTPUT_PATH}")
    message("=================================================================================")
endif ()