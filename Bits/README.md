# Miscelaneous modules

The goal of this repo is to concentrate some helper structures and functionalities that I find a bit handy.
Currently, there just a few modules, but hopefully this will grow over time. The currently existing modules are:
- Buffer: Utility classes for holding generic buffers;
- MemoryPool: Memory pool structure supporting ctor/dtor flow;
- Thread: Threading module, where we have utility structures like threads that join when going out of scope, worker threads and thread pools;
- IPC: Interprocess communication utilities. For this one there's only named pipe IPC structure implemented (linux only, for now);

## Build

For building this project I used the CppM tool (https://github.com/Kape94/CppM). 

You can build the library with the samples by calling:

```bash
cppm install
cppm build --cmake_args -DBUILD_SAMPLES=ON
```

