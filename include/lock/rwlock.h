#ifndef LOCK_RWLOCK_H_
#define LOCK_RWLOCK_H_

#include <pthread.h>

/**
 * 读写锁分装
 * 类命名风格参照std::mutex
 */
class rwlock {
 public:
    rwlock() {}

    ~rwlock() {
        pthread_rwlock_destroy(&rwlock_); 
    }

    void rdlock() {
        pthread_rwlock_rdlock(&rwlock_);
    }

    void wrlock() {
        pthread_rwlock_wrlock(&rwlock_);
    }

    void unlock() {
        pthread_rwlock_unlock(&rwlock_);
    }
    
 private:
    pthread_rwlock_t rwlock_ = PTHREAD_RWLOCK_INITIALIZER; 
};

/**
 * 读锁-使用简单封装
 * 类命名风格参照std::lock_guard
 */
class rdlock_guard {
 public:
    rdlock_guard(rwlock &lock) : rwlock_(lock) {
        rwlock_.rdlock();
    }
    
    ~rdlock_guard() {
        rwlock_.unlock();
    }

 private:
    rwlock &rwlock_;
};

/**
 * 写锁-使用简单封装
 * 类命名风格参照std::lock_guard
 */
class wrlock_guard {
 public:
    wrlock_guard(rwlock &lock) : rwlock_(lock) {
        rwlock_.wrlock();
    }

    ~wrlock_guard() {
        rwlock_.unlock();
    }
    
 private:
    rwlock &rwlock_;
};
#endif
