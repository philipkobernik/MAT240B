#include "al/app/al_App.hpp"
using namespace al;

#include "synths.h"
using namespace diy;

struct MyApp : App {
  Line line;
  Cycle cycle;

  void onCreate() override {
    // line.set(0, 0.2, 0);
    cycle.frequency(330);
    //
  }

  void onAnimate(double dt) override {
    //
  }

  void onDraw(Graphics& g) override {
    g.clear(0.25);
    //
  }

  void onSound(AudioIOData& io) override {
    while (io()) {
      io.out(0) = io.out(1) = line() * cycle();
    }
  }

  bool onKeyDown(const Keyboard& k) override {
    //
    printf("got here\n");
    line.set(1, 0, 0.5);
    return false;
  }

  bool onMouseMove(const Mouse& m) override {
    //
    printf("%d,%d\n", m.x(), m.y());
    return false;
  }
};

int main() {
  MyApp app;
  app.start();
}
