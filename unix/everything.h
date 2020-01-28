#ifndef __EVERYTHING__
#define __EVERYTHING__

#include <cmath>    // sin, cos, log
#include <cstdio>   // printf
#include <cstdlib>  // rand
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

namespace diy {

// constants specific to audio
//
const float SAMPLE_RATE = 44100;
const float pi =
    3.141592653589793238462643383279502884197169399375105820974944592307816406286;
/*
const float e =
    2.718281828459045235360287471352662497757247093699959574966967627724076630353;
    */

// common "helper" functions
//
float mtof(float m) { return 8.175799f * powf(2.0f, m / 12.0f); }
float ftom(float f) { return 12.0f * log2f(f / 8.175799f); }
float dbtoa(float db) { return 1.0f * powf(10.0f, db / 20.0f); }
float atodb(float a) { return 20.0f * log10f(a / 1.0f); }
float scale(float value, float low, float high, float low_, float high_) {
  return low_ + value / (high - low) * (high_ - low_);
}
float uniform(float low, float high) {
  return low + (high - low) * float(rand()) / RAND_MAX;
}

void say(float f) { printf("%f\n", f); }
void say(float f, float v) { printf("%f,%f\n", f, v); }

}  // namespace diy

#endif
