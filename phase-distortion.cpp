// TODO
// - ERROR!!! why does this program stop making sound after a while

#include "al/app/al_App.hpp"
using namespace al;

struct Array : std::vector<float> {
  void operator()(float f) { push_back(f); }

  // input range: [0, size)
  // use linear interpolation
  // the `at` call checks bounds
  float linear(const float index) const {
    const unsigned i = floor(index);
    const float x0 = at(i);
    const float x1 = at((i == (size() - 1)) ? 0 : i + 1);  // looping semantics
    const float t = index - i;
    return x1 * t + x0 * (1 - t);
  }

  // input range: [-size, size * 2)
  // tries to correct the input range
  // defers to `linear` method
  float sloppy(float index) const {
    if (index < 0) index += size();
    if (index > size()) index -= size();
    return linear(index);  // defer to our method without bounds checking
  }

  // allows us to ask for `foo[0.134]`
  float operator[](const float index) const { return linear(index); }

  // input range: [0, 1)
  float phasor(float index) const { return sloppy(size() * index); }
};

struct TableSine : Array {
  TableSine() {
    for (int i = 0; i < 1000; i++)  //
      push_back(sin(2 * M_PI * i / 1000));
  }
};

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

struct MyApp : App {
  Phasor p;
  void onCreate() override {
    p.frequency(220);
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
      float f = sine(p());
      io.out(0) = io.out(1) = f;
    }
  }

  bool onKeyDown(const Keyboard& k) override {
    //
    printf("got here\n");
    return false;
  }

  bool onMouseMove(const Mouse& m) override {
    //
    printf("%d,%d\n", m.x(), m.y());
    p.frequency(880 * m.x() / width());
    return false;
  }
};

int main() {
  MyApp app;
  app.start();
}
