// TODO
// - ERROR!!! why does this program stop making sound after a while

#include "al/app/al_App.hpp"
using namespace al;

#include "synths.h"
using namespace diy;

float pdhalf(float phase, float a) {  //
  float t = (1 - a) / 2;
  if (t <= 0) return 0;

  if (phase < t) {
    return phase * 0.5 / t;
  } else {
    return 0.5 + (phase - t) * 0.5 / (1 - t);
  }
}

struct MyApp : App {
  Phasor phase;
  Line frequency, gain;
  ADSR envelope;
  Line phase_distortion;

  void onCreate() override {
    frequency.set(220, 220, 0.05);
    gain.set(0, 0, 0.05);
    phase_distortion.set(0, 0, 0.05);

    envelope.set(0.08, 0.01, 0.707, 0.8);
    phase.frequency(220);
  }

  void onSound(AudioIOData& io) override {
    while (io()) {
      phase.frequency(frequency());
      float p = phase();
      p = pdhalf(p, phase_distortion());
      float f = sine(p) * envelope() * gain();
      io.out(0) = io.out(1) = f;
    }
  }

  bool onKeyDown(const Keyboard& k) override {
    if (k.key() >= '1' && k.key() <= '9') {
      int i = k.key() - '1';
      float value = norm(i, 0, 9);
      std::cout << value << std::endl;
      phase_distortion.set(value);
    }

    envelope.on();
    return false;
  }
  bool onKeyUp(const Keyboard& k) override {
    envelope.off();
    return false;
  }

  bool onMouseMove(const Mouse& m) override {
    frequency.set(mtof(map(m.x(), 0, width(), 20, 80)));
    gain.set(dbtoa(map(m.y(), height(), 0, -60, -1)));
    return false;
  }
};

int main() {
  MyApp app;
  app.start();
}
