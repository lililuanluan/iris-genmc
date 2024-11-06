#!/bin/bash

./genmc --disable-estimation -- --std=c++11 -DIRIS_CACHELINE_SIZE=64 -DSPINLOOP_ASSUME  -DITERATIONS=1e3  test_lfringbuffer.cpp # 