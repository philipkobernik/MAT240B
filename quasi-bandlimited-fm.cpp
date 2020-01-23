#include "al/app/al_App.hpp"
using namespace al;

#include "synths.h"
using namespace diy;

struct MyApp : App {
  QuasiBandlimited synth;
  Line frequency, gain;
  Line virtual_filter;

  void onCreate() override {
    frequency.set(220, 220, 0.05);
    gain.set(0, 0, 0.05);
    virtual_filter.set(1, 1, 0.05);
  }

  void onSound(AudioIOData& io) override {
    while (io()) {
      synth.set(frequency(), virtual_filter());
      float f = synth.saw();
      io.out(0) = io.out(1) = f * gain();
    }
  }

  bool onKeyDown(const Keyboard& k) override {
    if (k.key() >= '1' && k.key() <= '9') {
      int i = k.key() - '1';
      float value = norm(i, 0, 9);
      std::cout << value << std::endl;
      virtual_filter.set(value);
    }

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
