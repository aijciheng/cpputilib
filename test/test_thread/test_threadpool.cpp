#include "thread/threadpool.h"

#include <stdio.h>
#include <unistd.h>

#include <iostream>

class PrintTask : public Task {
 public:
    PrintTask(int value) : value_(value) {
    }

    ~PrintTask() {
        //printf("free print task\n");
    }

    void run() {
        printf("hello word \n");
        value_ = 3;
    }

 private:
    int value_;
};

void delete_fun(void *obj) {
    delete (PrintTask*)obj;
}

#define thread_count 10
#define work_count 100000
int main() {
    printf("thread_count : %d, work_count : %d\n", thread_count, work_count);
    ThreadPool thread_pool(thread_count);

    for (int i = 0; i < work_count; i++) {
        PrintTask *task = new PrintTask(i);
        if (thread_pool.execute(task, delete_fun) != THREADPOOL_OK) {
            printf("thread execute failed\n");
            exit(-1);
        }
    }
    printf("==================\n");
    thread_pool.wait();
    return 0;
}
