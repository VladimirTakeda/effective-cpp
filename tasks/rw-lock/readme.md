# RW-Lock

## Problem

In this task you should implement rw-lock (or shared mutex) primitive. There is already an implementation in `rw_lock.h`, however its incorrect.
Could you tell what exactly is incorrect in this implementation?

Implement Rwlock in `rw_lock.h` properly using condition variables. Note, that both Read and Write receive callbacks to call. These callbacks
are critical sections basically.

Note, that `shared_mutex` is **forbidden** in this task.

## Testing

For tasks with multithreading there is an additional `TSAN` target built with thread sanitizer. Your program should pass tests with it as well.

Apart from traditional `test.cpp`, there is also a benchmark in `bench.cpp`. There are no time limits currently, but your implementation should finish successfully.
