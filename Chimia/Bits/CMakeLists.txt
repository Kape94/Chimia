
set(BITS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(TARGET_NAME Chimia_Bits)
add_library(${TARGET_NAME}
  src/IPC/PipeHost.cpp
  src/IPC/PipeWriter.cpp
  src/Process/ChildProcess.cpp
  src/Process/ProcessInfo.cpp
  src/Thread/TaskQueue.cpp
  src/Thread/ThreadPool.cpp
  src/Thread/ThreadPoolWorker.cpp
)

target_include_directories(${TARGET_NAME} 
  PUBLIC 
    ./api
  PRIVATE
    ./api/Bits
)

target_link_libraries(${TARGET_NAME}
  PUBLIC
    Chimia_Core
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
