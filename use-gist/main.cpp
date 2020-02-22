#include "al/app/al_App.hpp"
using namespace al;

#include "Gist.h"

#include <vector>
using namespace std;

float mtof(float m) { return 8.175799f * powf(2.0f, m / 12.0f); }

const int sampleRate = 44100;
const int frameSize = 1024;
const int hopSize = frameSize / 4;

static Gist<float> gist(frameSize, sampleRate);

struct FeatureVector {
  float rms{0}, zcr{0}, centroid{0};
};

struct Appp : App {
  // vector<FeatureVector> feature;
  unsigned index{0};

  void onCreate() override {
    //
    // feature.resize(width());
  }

  void onSound(AudioIOData& io) override {
    gist.processAudioFrame(io.inBuffer(0), frameSize);
    cout << gist.rootMeanSquare() << ',' << gist.zeroCrossingRate() << ','
         << gist.spectralCentroid() << '\n';

    while (io()) {
      float f = 0;
      io.out(0) = f;
      io.out(1) = f;
    }

    //    index++;
    //    if (index >= feature.size())  //
    //      index = 0;
  }

  void onDraw(Graphics& g) override {
    g.clear(1.0);

    //
  }
};

int main() {
  Appp app;
  app.audioDomain()->configure(44100, frameSize, 2, 2);
  app.start();
}
