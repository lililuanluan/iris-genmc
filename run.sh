#!/bin/bash


N=500
./genmc --fuzz --fuzz-max=${N} --mutation-policy=no-mutation --disable-estimation --count-distinct-execs --disable-race-detection --is-interesting=new  --use-queue -- --std=c++11 -DIRIS_CACHELINE_SIZE=64 -DSPINLOOP_ASSUME  -DITERATIONS=1e7  test_lfringbuffer.cpp # 


./genmc --fuzz --fuzz-max=${N} --disable-estimation --count-distinct-execs --disable-race-detection --is-interesting=new  --use-queue -- --std=c++11 -DIRIS_CACHELINE_SIZE=64 -DSPINLOOP_ASSUME  -DITERATIONS=1e7  test_lfringbuffer.cpp # 