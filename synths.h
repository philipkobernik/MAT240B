#pragma once
#include <cmath>        // sin, cos, pow, log, ...
#include <vector>       // std::vector
#include "functions.h"  // mtof, dbtoa, map, ...

namespace diy {

const float SAMPLE_RATE = 44100;

struct Array : std::vector<float> {
  void operator()(float f) { push_back(f); }

  float raw(const float index) const {
    const unsigned i = floor(index);
    const float x0 = at(i);
    const float x1 = at((i == (size() - 1)) ? 0 : i + 1);  // looping semantics
    const float t = index - i;
    return x1 * t + x0 * (1 - t);
  }

  // void resize(unsigned n) { data.resize(n, 0); }
  // float& operator[](unsigned index) { return data[index]; }

  // allow for sloppy indexing (e.g., negative, huge) by fixing the index to
  // within the bounds of the array
  float get(float index) const {
    if (index < 0) index += size();
    if (index > size()) index -= size();
    return raw(index);  // defer to our method without bounds checking
  }
  float operator[](const float index) const { return get(index); }
  float phasor(float index) const { return get(size() * index); }
};

struct TableSine : Array {
  TableSine() {
    for (int i = 0; i < 1000; i++)  //
      push_back(::sin(2 * M_PI * i / 1000));
  }
};

// expects (0, 1)
float sine(float t) {
  static TableSine tableSine;
  return tableSine.phasor(t);
}

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

struct Cycle : Phasor {
  float operator()() {
    //
    return sine(Phasor::operator()());
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

struct ADSR {
  Line attack, decay, release;
  int state = 0;

  void set(float a, float d, float s, float r) {
    attack.set(0, 1, a);
    decay.set(1, s, d);
    release.set(s, 0, r);
  }

  void on() {
    attack.value = 0;
    decay.value = 1;
    state = 1;
  }

  void off() {
    release.value = decay.target;
    state = 3;
  }

  float operator()() {
    switch (state) {
      default:
      case 0:
        return 0;
      case 1:
        if (!attack.done()) return attack();
        if (!decay.done()) return decay();
        state = 2;
      case 2:  // sustaining...
        return decay.target;
      case 3:
        return release();
    }
  }
  void print() {
    printf("  state:%d\n", state);
    printf("  attack:%f\n", attack.seconds);
    printf("  decay:%f\n", decay.seconds);
    printf("  sustain:%f\n", decay.target);
    printf("  release:%f\n", release.seconds);
  }
};

struct QuasiBandlimited {
  //
  // from "Synthesis of Quasi-Bandlimited Analog Waveforms Using Frequency
  // Modulation" by Peter Schoffhauzer
  // (http://scp.web.elte.hu/papers/synthesis1.pdf)
  //
  const float a0 = 2.5;   // precalculated coeffs
  const float a1 = -1.5;  // for HF compensation

  // variables
  float osc;      // output of the saw oscillator
  float osc2;     // output of the saw oscillator 2
  float phase;    // phase accumulator
  float w;        // normalized frequency
  float scaling;  // scaling amount
  float DC;       // DC compensation
  float norm;     // normalization amount
  float last;     // delay for the HF filter

  float Frequency, Filter, PulseWidth;

  QuasiBandlimited() {
    reset();
    Frequency = 1.0;
    Filter = 0.85;
    PulseWidth = 0.5;
    recalculate();
  }

  void reset() {
    // zero oscillator and phase
    osc = 0.0;
    osc2 = 0.0;
    phase = 0.0;
  }

  void recalculate() {
    w = Frequency / SAMPLE_RATE;  // normalized frequency
    float n = 0.5 - w;
    scaling = Filter * 13.0f * powf(n, 4.0f);  // calculate scaling
    DC = 0.376 - w * 0.752;                    // calculate DC compensation
    norm = 1.0 - 2.0 * w;                      // calculate normalization
  }

  void frequency(float f) {
    Frequency = f;
    recalculate();
  }

  void filter(float f) {
    Filter = f;
    recalculate();
  }

  void set(float frequency, float filter) {
    Frequency = frequency;
    Filter = filter;
    recalculate();
  }

  void pulseWidth(float w) {
    PulseWidth = w;
    recalculate();
  }

  void step() {
    // increment accumulator
    phase += 2.0 * w;
    if (phase >= 1.0) phase -= 2.0;
    if (phase <= -1.0) phase += 2.0;
  }

  // process loop for creating a bandlimited saw wave
  float saw() {
    step();

    // calculate next sample
    osc = (osc + sinf(2 * M_PI * (phase + osc * scaling))) * 0.5;
    // compensate HF rolloff
    float out = a0 * osc + a1 * last;
    last = osc;
    out = out + DC;     // compensate DC offset
    return out * norm;  // store normalized result
  }

  // process loop for creating a bandlimited PWM pulse
  float pulse() {
    step();

    // calculate saw1
    osc = (osc + sinf(2 * M_PI * (phase + osc * scaling))) * 0.5;
    // calculate saw2
    osc2 =
        (osc2 + sinf(2 * M_PI * (phase + osc2 * scaling + PulseWidth))) * 0.5;
    float out = osc - osc2;  // subtract two saw waves
    // compensate HF rolloff
    out = a0 * out + a1 * last;
    last = osc;
    return out * norm;  // store normalized result
  }
};

}  // namespace diy
