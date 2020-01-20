#pragma once
#include <cmath>  // sin, cos, pow, log, ...

namespace diy {

const float SAMPLE_RATE = 44100;

inline float map(float value, float low, float high, float Low, float High) {
  // like Max's [scale]
  return Low + (High - Low) * ((value - low) / (high - low));
}
inline float norm(float value, float low, float high) {
  return (value - low) / (high - low);
}
inline float lerp(float a, float b, float t) {
  // like Max's [mix]
  return (1.0f - t) * a + t * b;
}
inline float mtof(float m) { return 8.175799f * powf(2.0f, m / 12.0f); }
inline float ftom(float f) { return 12.0f * log2f(f / 8.175799f); }
inline float dbtoa(float db) { return 1.0f * powf(10.0f, db / 20.0f); }
inline float atodb(float a) { return 20.0f * log10f(a / 1.0f); }

// kind of like tanh: takes large numbers and puts them on (-1, 1) gently
inline float sigmoid(float x) { return 2.0f / (1.0f + expf(-x)) - 1.0f; }

// these expect a phase on (0, 1)
float saw(float phase) { return phase * 2 - 1; }
float rect(float phase) { return phase < 0.5 ? -1 : 1; }

// can you make this fewer instructions???
float tri(float phase) {
  float f = 2 * phase - 1;
  f = (f < -0.5) ? -1 - f : (f > 0.5 ? 1 - f : f);
  return 2 * f;
}

// line~
// cycle~ (phasor + sin)
// adsr~
// scale
// mtof

struct Phasor {
  float phase = 0;
  float increment = 1;

  // overloading the "call" operator
  float operator()() {
    float value = phase;

    // side effect
    phase += increment;
    if (phase > 1)  //
      phase -= 1;
    if (phase < 0)  //
      phase += 1;

    return value;
  }

  void frequency(float hz) { increment = hz / 44100; }
  void period(float seconds) { frequency(1 / seconds); }
};

// expects phase on (0, 1)
float sin(float phase) {
  //
  return ::sin(2 * M_PI * phase);
}

struct Cycle : Phasor {
  float operator()() {
    //
    return sin(Phasor::operator()());
  }
};

struct Line {
  float value = 0, target = 0, seconds = 1 / SAMPLE_RATE, increment = 0;

  void set() {
    // guard against divide by zero
    if (seconds <= 0) seconds = 1 / SAMPLE_RATE;
    // slope per sample
    increment = (target - value) / (seconds * SAMPLE_RATE);
  }
  void set(float v, float t, float s) {
    value = v;
    target = t;
    seconds = s;
    set();
  }
  void set(float t, float s) {
    target = t;
    seconds = s;
    set();
  }
  void set(float t) {
    target = t;
    set();
  }

  bool done() { return value == target; }

  float operator()() {
    float v = value;  // value to return

    // side effect
    if (value != target) {
      value += increment;
      bool overShoot = (increment < 0) ? (value < target) : (value > target);
      if (overShoot) value = target;
    }

    return v;
  }
};

}  // namespace diy