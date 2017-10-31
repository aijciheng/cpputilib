#ifndef THREAD_THREADPOOL_H_
#define THREAD_THREADPOOL_H_ 

#include <list>
#include <mutex>
#include <condition_variable>

#include <pthread.h>

class Task {
 public:
    virtual ~Task() {};
    virtual void run() = 0;
};

class TaskWrapper {
 public:
    TaskWrapper(Task *task, void (*delete_fun)(void*))
        : task_(task),
          delete_fun_(delete_fun) {
    }

    ~TaskWrapper() {
        if (delete_fun_) {
            delete_fun_((void*)task_);
        }
    }

    Task *task() {
        return task_;
    }
        
 private:
    Task *task_; 
    void (*delete_fun_)(void*);
};

typedef enum {
    THREADPOOL_OK = 0,
    THREADPOOL_PARAM_ERROR,
    THREADPOOL_INIT_ERROR,
    THREADPOOL_HAS_DESTROY,
    THREADPOOL_ERROR
} THREADPOOL_STATE;

class ThreadPool {
 public:
    ThreadPool(int fix_thread_num) : is_destroy_(false) {
        is_init_ = true;
        for (int i = 0; i < fix_thread_num; i++) {
            pthread_t thread;
            if (pthread_create(&thread, NULL, run_thread, this) != 0) {
                is_init_ = false;
                break;
            }
            threads_.push_back(thread);
        }
    }

    ~ThreadPool() {
        /* delete task wrapper */
        for (std::list<TaskWrapper*>::iterator it = tasks_.begin();
                it != tasks_.end(); it++) {
            TaskWrapper *task_wrapper = *it;
            if (task_wrapper) delete task_wrapper;
        }
    }

    int execute(Task *task, void (*delete_fun)(void*)) {
        std::lock_guard<std::mutex> lock(lock_);
        if (is_destroy_) {
            return THREADPOOL_HAS_DESTROY;
        }
        if (!task) {
            return THREADPOOL_PARAM_ERROR;
        }

        TaskWrapper *task_wrapper = new TaskWrapper(task, delete_fun);
        if (!task_wrapper) {
            return THREADPOOL_ERROR;
        }
        
        tasks_.push_back(task_wrapper);
        cond_var_.notify_one();
        return THREADPOOL_OK;
    }

    TaskWrapper *get_task() {
        std::unique_lock<std::mutex> lock(lock_);
        while (tasks_.size() <= 0) {
            if (is_destroy_) return NULL;
            cond_var_.wait(lock); 
        }
        TaskWrapper *task_wrapper = tasks_.front();
        tasks_.pop_front();
        return task_wrapper; 
    }

    void wait() {
        /* flag destroy */
        is_destroy_ = true;

        /* delete thread */
        for (std::list<pthread_t>::iterator it = threads_.begin();
                it != threads_.end(); it++) {
            printf("%ld\n", *it);
            pthread_join(*it, NULL);
        }
    }

 private:
    static void *run_thread(void *obj) {
        if (!obj) return NULL;
        ThreadPool &thread_pool = *((ThreadPool*)obj);
        while (true) {
            TaskWrapper *task_wrapper = thread_pool.get_task();
            if (task_wrapper) {
                task_wrapper->task()->run();
                /* finished task */
                delete task_wrapper;
            } else {
                /* thread pool destroy */
                break;
            }
        }
    }

    std::list<TaskWrapper*> tasks_;
    std::list<pthread_t> threads_;
    std::mutex lock_;
    std::condition_variable cond_var_;
    bool is_init_;
    volatile bool is_destroy_;
};

#endif
