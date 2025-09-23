set(SRC_FILES 
  _internal/IndexedBuffer.cpp
  _internal/RawBuffer.cpp
)

set(TARGET_NAME MiscBuffer)
add_library(${TARGET_NAME} ${SRC_FILES})

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

target_include_directories(${TARGET_NAME} PRIVATE ${MISC_ROOT})