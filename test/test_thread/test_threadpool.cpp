#include "thread/threadpool.h"

#include <stdio.h>

#include <iostream>

class PrintTask : public Task {
 public:
    PrintTask(int value) : value_(value) {
    }

    void run() {
        printf("hello word%d\n", value_);
    }

 private:
    int value_;
};

void delete_fun(void *obj) {
    delete (PrintTask*)obj;
}

#define thread_count 3
#define work_count 100
int main() {
    printf("thread_count : %d, work_count : %d\n", thread_count, work_count);
    ThreadPool thread_pool(thread_count);

    for (int i = 0; i < work_count; i++) {
        PrintTask *task = new PrintTask(i);
        thread_pool.execute(task, delete_fun);
    }
    thread_pool.wait();
    return 0;
}
