set(SRC_FILES
    _internal/PipeHost.cpp
    _internal/PipeWriter.cpp
)


if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(POSIX_SRCS
        _internal/posix/PosixPipeHost.cpp
        _internal/posix/PosixPipeWriter.cpp
    )
endif()

set(TARGET_NAME MiscIPC)

add_library(MiscIPC 
    ${SRC_FILES} 
    ${POSIX_SRCS}
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

target_include_directories(${TARGET_NAME} 
    PRIVATE 
        ${MISC_ROOT}
        ${CMAKE_CURRENT_SOURCE_DIR}
)
