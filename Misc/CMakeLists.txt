
set(MISC_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(TARGET_NAME Misc)
add_library(${TARGET_NAME}
  src/Buffer/IndexedBuffer.cpp
  src/Buffer/RawBuffer.cpp
  src/IPC/posix/PosixPipeHost.cpp
  src/IPC/posix/PosixPipeWriter.cpp
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
    ./api/Misc
)
