#include <stdio.h>
#include <pthread.h>
#include <cstdlib>

#include <string.h>
#include "genmc.hpp"

#include "lfringbuffer.h"
#include "sslfqueue.h"
#include "utils.h"
#include "helper.h"



struct buffer_t {
    char* b;
    int    size;
    int    alloc_size;
    int    data;
    buffer_t& operator=(const buffer_t& other) {
        b = other.b;
        size = other.size;
        alloc_size = other.alloc_size;
        data = other.data;
        return *this;
    }
};


struct ThreadData {
    sslfqueue<buffer_t>* q;
    lfringbuffer* rbuf;
};

#ifndef ITERATIONS
#define ITERATIONS (int)1e5
#endif

void* recyle(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);

    sslfqueue<buffer_t>* q = data->q;
    lfringbuffer* rbuf = data->rbuf;

    int i = 1;
    while (i <= ITERATIONS) {
        buffer_t b;

#ifdef SPINLOOP_ASSUME
        __VERIFIER_assume(q->poll(b) == true);
#else 
        while (!q->poll(b));
#endif
        assert(Atoi(b.b, b.size) == b.data);

        rbuf->release(b.alloc_size);
        ++i;
    }

    return nullptr;
}





int main() {
    lfringbuffer rbuf(1024);
    sslfqueue<buffer_t> q;

    char* p1;
    char* p2;
    char* p3;
    assert(512 == rbuf.acquire(512, p1));
    assert(p1);

    assert(256 == rbuf.acquire(256, p2));
    assert(p2);

    assert(p2 - p1 == 512);

    assert(0 == rbuf.acquire(512, p1));

    assert(rbuf.freespace() == 256);

    assert(0 == rbuf.acquire(512, p3));

    rbuf.release(512);

    assert(768 == rbuf.acquire(512, p3));

    printf("rbuf.freespace(): %lu\n", rbuf.freespace());
    assert(rbuf.freespace() == 0);

    rbuf.release(256);
    printf("rbuf.freespace(): %lu\n", rbuf.freespace());
    assert(256 == rbuf.freespace());
    rbuf.release(768);
    printf("rbuf.freespace(): %lu\n", rbuf.freespace());
    assert(1024 == rbuf.freespace());



    pthread_t recyler;
    ThreadData data = { &q, &rbuf };
    pthread_create(&recyler, nullptr, recyle, &data);

    int i = 1;
    while (i <= ITERATIONS) {
        // const char* s = to_string(i);
        char s[100]{};
        to_string(s, i);

        buffer_t b;
        char* ptr = s;
        int size;


#ifdef SPINLOOP_ASSUME
        __VERIFIER_assume(size = rbuf.acquire(Strlen(s), ptr));
#else 
        while (!(size = rbuf.acquire(Strlen(s), ptr)));
#endif


        b.b = ptr;
        Memcpy(b.b, s, Strlen(s));
        b.size = Strlen(s);
        b.alloc_size = size;
        b.data = i;



#ifdef SPINLOOP_ASSUME
        __VERIFIER_assume(true == q.offer(b));
#else 
        while (!q.offer(b));
#endif

        ++i;
    }



    pthread_join(recyler, nullptr);
    printf("passed\n");
    return 0;
}