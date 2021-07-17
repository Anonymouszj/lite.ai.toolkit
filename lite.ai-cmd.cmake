# config lite.ai shared lib.
function(add_lite_ai_shared_library version soversion)
    configure_file (
            "${CMAKE_SOURCE_DIR}/lite/config.h.in"
            "${CMAKE_SOURCE_DIR}/lite/config.h"
    )

    # 2. glob headers files
    file(GLOB LITE_HEAD ${CMAKE_SOURCE_DIR}/lite/*.h)

    # 3. glob sources files
    file(GLOB LITE_SRCS ${CMAKE_SOURCE_DIR}/lite/*.cpp)
    set(LITE_DEPENDENCIES ${OpenCV_LIBS})

    if (ENABLE_ONNXRUNTIME)
        include(lite.ai-onnxruntime.cmake)
        set(LITE_SRCS ${LITE_SRCS} ${ORT_SRCS})
        set(LITE_DEPENDENCIES ${LITE_DEPENDENCIES} onnxruntime)
    endif()

    if (ENABLE_MNN)
        include(lite.ai-mnn.cmake)
        set(LITE_SRCS ${LITE_SRCS} ${MNN_SRCS})
        set(LITE_DEPENDENCIES ${LITE_DEPENDENCIES} mnn)
    endif()

    if (ENABLE_NCNN)
        include(lite.ai-ncnn.cmake)
        set(LITE_SRCS ${LITE_SRCS} ${NCNN_SRCS})
        set(LITE_DEPENDENCIES ${LITE_DEPENDENCIES} ncnn)
    endif()

    if (ENABLE_TNN)
        include(lite.ai-tnn.cmake)
        set(LITE_SRCS ${LITE_SRCS} ${TNN_SRCS})
        set(LITE_DEPENDENCIES ${LITE_DEPENDENCIES} tnn)
    endif()

    # 4. shared library
    add_library(lite.ai SHARED ${LITE_SRCS})
    target_link_libraries(lite.ai ${LITE_DEPENDENCIES})
    set_target_properties(lite.ai PROPERTIES VERSION ${version} SOVERSION ${soversion})

    if (LITE_AI_COPY_BUILD)
        message("Installing Lite.AI Headers ...")
        file(INSTALL ${LITE_HEAD} DESTINATION ${CMAKE_SOURCE_DIR}/build/lite.ai/include/lite)

    endif ()
    message(">>>> Added Shared Library: lite.ai !")

endfunction()

# add custom command for lite.ai shared lib.
function(add_lite_ai_custom_command)
    if (LITE_AI_BUILD_TEST)
        add_custom_command(TARGET lite.ai
                PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory ${EXECUTABLE_OUTPUT_PATH}
                COMMAND ${CMAKE_COMMAND} -E make_directory ${LIBRARY_OUTPUT_PATH}
                COMMAND ${CMAKE_COMMAND} -E echo "create ${LIBRARY_OUTPUT_PATH} done!"
                COMMAND ${CMAKE_COMMAND} -E echo "create ${EXECUTABLE_OUTPUT_PATH} done!"
                )
        # copy opencv & lite.ai libs.
        add_custom_command(TARGET lite.ai
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBRARY_OUTPUT_PATH} ${EXECUTABLE_OUTPUT_PATH}
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${OpenCV_LIBRARY_DIR} ${EXECUTABLE_OUTPUT_PATH}
                COMMAND ${CMAKE_COMMAND} -E rm -rf ${EXECUTABLE_OUTPUT_PATH}/cmake
                COMMAND ${CMAKE_COMMAND} -E echo "copy opencv and lite.ai libs done!"
                )
        # copy onnxruntime libs.
        if (ENABLE_ONNXRUNTIME)
            add_custom_command(TARGET lite.ai
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ONNXRUNTIME_LIBRARY_DIR} ${EXECUTABLE_OUTPUT_PATH}
                    COMMAND ${CMAKE_COMMAND} -E echo "copy onnxruntime libs done!"
                    )
        endif()
        # copy MNN libs.
        if (ENABLE_MNN)
            add_custom_command(TARGET lite.ai
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${MNN_LIBRARY_DIR} ${EXECUTABLE_OUTPUT_PATH}
                    COMMAND ${CMAKE_COMMAND} -E echo "copy MNN libs done!"
                    )
        endif()
        # copy NCNN libs.
        if (ENABLE_NCNN)
            add_custom_command(TARGET lite.ai
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${NCNN_LIBRARY_DIR} ${EXECUTABLE_OUTPUT_PATH}
                    COMMAND ${CMAKE_COMMAND} -E echo "copy NCNN libs done!"
                    )
        endif()
        # copy TNN libs.
        if (ENABLE_TNN)
            add_custom_command(TARGET lite.ai
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${TNN_LIBRARY_DIR} ${EXECUTABLE_OUTPUT_PATH}
                    COMMAND ${CMAKE_COMMAND} -E echo "copy TNN libs done!"
                    )
        endif()
    endif()
endfunction()

function(add_lite_executable executable_name field)
    if (NOT ${THIRDPARTY_SET_STATE})
        include(${CMAKE_SOURCE_DIR}/setup_3rdparty.cmake)
    endif()
    add_executable(${executable_name} ${field}/test_${executable_name}.cpp)
    target_link_libraries(${executable_name} lite.ai)  # link lite.ai
    message(">>>> Added Lite Executable: ${executable_name} !")
endfunction ()

function(add_ort_executable executable_name field)
    if (NOT ${THIRDPARTY_SET_STATE})
        include(${CMAKE_SOURCE_DIR}/setup_3rdparty.cmake)
    endif()
    add_executable(${executable_name} ${field}/test_${executable_name}.cpp)
    target_link_libraries(${executable_name} lite.ai)  # link lite.ai
    message(">>>> Added Ort Executable: ${executable_name} !")
endfunction ()

function(add_mnn_executable executable_name field)
    if (NOT ${THIRDPARTY_SET_STATE})
        include(${CMAKE_SOURCE_DIR}/setup_3rdparty.cmake)
    endif()
    add_executable(${executable_name} ${field}/test_${executable_name}.cpp)
    target_link_libraries(${executable_name} lite.ai)  # link lite.ai
    message(">>>> Added MNN Executable: ${executable_name} !")
endfunction ()

function(add_ncnn_executable executable_name field)
    if (NOT ${THIRDPARTY_SET_STATE})
        include(${CMAKE_SOURCE_DIR}/setup_3rdparty.cmake)
    endif()
    add_executable(${executable_name} ${field}/test_${executable_name}.cpp)
    target_link_libraries(${executable_name} lite.ai)  # link lite.ai
    message(">>>> Added NCNN Executable: ${executable_name} !")
endfunction ()

function(add_tnn_executable executable_name field)
    if (NOT ${THIRDPARTY_SET_STATE})
        include(${CMAKE_SOURCE_DIR}/setup_3rdparty.cmake)
    endif()
    add_executable(${executable_name} ${field}/test_${executable_name}.cpp)
    target_link_libraries(${executable_name} lite.ai)  # link lite.ai
    message(">>>> Added TNN Executable: ${executable_name} !")
endfunction ()