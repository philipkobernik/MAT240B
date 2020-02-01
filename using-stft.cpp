#include "Gamma/DFT.h"  // gam::STFT
#include "Gamma/Oscillator.h"
#include "Gamma/SamplePlayer.h"
#include "al/app/al_App.hpp"
using namespace al;

#include <limits>

#include "functions.h"

gam::STFT stft(4096, 4096 / 4, 16384, gam::HAMMING);  //, gam::MAG_FREQ, 3);
gam::SamplePlayer<float, gam::ipl::Cubic, gam::phsInc::Loop> player;

// unsigned winSize
// unsigned hopSize
// unsigned padSize
// WindowType winType,
// SpectralType specType
// unsigned numAuxA???

struct MyApp : App {
  float hz;
  gam::Sine<> osc;

  Mesh spectrum{Mesh::LINE_STRIP};
  Mesh line{Mesh::LINES};

  float minimum{std::numeric_limits<float>::max()};
  float maximum{-std::numeric_limits<float>::max()};

  void onCreate() override {
    gam::sampleRate(audioIO().framesPerSecond());
    player.load("../BBQ.wav");

    for (int i = 0; i < stft.numBins(); i++) {
      spectrum.vertex(2.0 * i / stft.numBins() - 1);
    }

    line.vertex(0, 1);
    line.vertex(0, -1);
  }

  void onSound(AudioIOData& io) override {
    while (io()) {
      float f = player();

      if (stft(f)) {
        //
        // search for the "hottest" bin
        int hottest = -1;
        float _maximum = -std::numeric_limits<float>::max();

        int N = stft.numBins();
        for (int i = 0; i < N; i++) {
          //
          // calculate statistics on the magnitude of the bin
          float m = log(stft.bin(i).mag());
          if (m > maximum) maximum = m;
          if (m < minimum) minimum = m;
          spectrum.vertices()[i].y = diy::map(m, minimum, maximum, -1, 1);

          // search for the "hottest" bin
          if (m > _maximum) {
            _maximum = m;
            hottest = i;
          }
        }

        // if a hottest bin was found on this frame
        if (hottest > -1) {
          hz = audioIO().framesPerSecond() / 2.0f * hottest / N;
          // set the frequency of the oscillator
          osc.freq(hz);
        }
      }

      io.out(0) = io.out(1) = osc();
    }
  }

  void onAnimate(double dt) override {
    //
    //
    //
    // printf("%f\n", hz);
  }

  void onDraw(Graphics& g) override {
    g.clear(0.3);
    g.camera(Viewpoint::IDENTITY);  // Ortho [-1:1] x [-1:1]
    g.color(0, 1, 1);
    g.draw(spectrum);
    g.color(1, 0, 0);
    g.translate(diy::map(hz, 0, audioIO().framesPerSecond() / 2, -1, 1), 0, 0);
    g.draw(line);
  }
};

int main() {
  MyApp app;
  //  app.configureAudio(44100, 512, 2, 2);
  app.start();
}
