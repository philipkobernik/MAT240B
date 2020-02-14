#include "Gamma/DFT.h"  // gam::STFT
#include "Gamma/Filter.h"
#include "Gamma/Oscillator.h"
#include "Gamma/SamplePlayer.h"
#include "al/app/al_App.hpp"
#include "al/ui/al_ControlGUI.hpp"  // gui.draw(g)
using namespace al;

#include <algorithm>
#include <limits>
#include <vector>

#include "functions.h"

gam::STFT stft(4096, 4096 / 4, 16384, gam::HAMMING);
gam::SamplePlayer<float, gam::ipl::Cubic, gam::phsInc::Loop> player;

// https://ccrma.stanford.edu/~jos/pasp/Fundamental_Frequency_Estimation.html
//

std::vector<float> bottom;
//
float f0(float threshold, int N, const float* data, int size,
         float sampleRate) {
  // find loudest maxima over the threshold
  //
  struct Peak {
    float db, hz;
  };
  std::vector<Peak> peak;
  for (int i = 1; i < size - 1; i++) {
    if (data[i] < (threshold + bottom[i])) continue;

    // make sure it is a local maxima
    if (data[i] < data[i - 1]) continue;
    if (data[i] < data[i + 1]) continue;
    peak.push_back({data[i], i * sampleRate / (2 * size)});
  }
  std::sort(peak.begin(), peak.end(),
            [](const Peak& a, const Peak& b) { return a.db > b.db; });

  if (peak.size() < N) {
    return 0.0f / 0.0f;
  }

  // collect the differences
  //
  std::vector<float> difference;
  for (int i = 0; i < N; i++)
    for (int j = 1 + i; j < N; j++)
      difference.push_back(abs(peak[i].hz - peak[j].hz));
  // because we need minimum and maximum...
  std::sort(difference.begin(), difference.end(),
            [](float a, float b) { return a < b; });

  // build a histogram of the differences
  //
  struct Bin {
    int index, bin;
  };
  std::vector<Bin> histogram;
  for (int index = 0; index < N; index++) {
    int bin = difference.size() * (difference[index] - difference[0]) /
              (difference.back() - difference[0]);
    if (bin >= difference.size()) {
      printf("%d is beyond the bin range %lu\n", bin, difference.size());
      fflush(stdout);
      exit(10);
    }
    histogram.push_back({index, bin});
  }

  // count the "frequency" (number of occurances) of bins
  //
  std::vector<int> count;
  count.resize(difference.size());
  for (int i = 0; i < histogram.size(); i++) {
    count[histogram[i].bin]++;
  }

  // find the "most common" differences
  //
  int maxValue = 0;
  int mostCommon = -1;
  for (int bin = 0; bin < count.size(); bin++) {
    if (count[bin] > maxValue) {
      maxValue = count[bin];
      mostCommon = bin;
    }
  }

  if (mostCommon == -1) {
    printf("most common difference not found\n");
    return 0.0f / 0.0f;
  }

  float sum = 0;
  for (int i = 0; i < N; i++) {
    if (histogram[i].bin == mostCommon) {
      float value = difference[histogram[i].index];
      sum += value;
    }
  }
  return sum / count[mostCommon];
}

struct MyApp : App {
  float fundamental;
  ParameterInt peakCount{"/peakCount", "", 7, "", 5, 15};
  Parameter threshold{"/threshold", "", 11.0, "", 1.0, 24.0};
  Parameter highLimit{"/highLimit", "", 1.0, "", 0.0, 1.0};
  Parameter estimate{"/estimate", "", 0, "", 0.0, 127.0};
  ControlGUI gui;

  Mesh spectrum{Mesh::LINE_STRIP};
  Mesh lower{Mesh::LINE_STRIP};
  Mesh vertical{Mesh::LINES};
  Mesh horizontal{Mesh::LINES};

  float minimum{std::numeric_limits<float>::max()};
  float maximum{-std::numeric_limits<float>::max()};

  float t{0};

  void onCreate() override {
    gui << peakCount << threshold << highLimit << estimate;
    gui.init();
    navControl().useMouse(false);

    bottom.resize(stft.numBins());
    for (int i = 0; i < bottom.size(); i++) {
      bottom[i] = 0;
    }

    gam::sampleRate(audioIO().framesPerSecond());
    player.load("../Guitar-Tuner-Example.wav");

    for (int i = 0; i < stft.numBins(); i++) {
      spectrum.vertex(2.0 * i / stft.numBins() - 1);
      lower.vertex(2.0 * i / stft.numBins() - 1);
    }

    vertical.vertex(0, 1);
    vertical.vertex(0, -1);
    horizontal.vertex(-1, 0);
    horizontal.vertex(1, 0);
  }

  void onAnimate(double dt) override { estimate = diy::ftom(fundamental); }

  void onSound(AudioIOData& io) override {
    while (io()) {
      float f = player();

      if (stft(f)) {
        std::vector<float> data;
        t = -threshold.get();

        int N = stft.numBins();
        for (int i = 0; i < N; i++) {
          //
          // calculate statistics on the magnitude of the bin
          float m = log(stft.bin(i).mag());
          data.push_back(m);
          if (m > maximum) maximum = m;
          if (m < minimum) minimum = m;
          spectrum.vertices()[i].y = diy::map(m, minimum, maximum, -1, 1);

          if (m < bottom[i]) {
            bottom[i] = m;
            lower.vertices()[i].y = diy::map(m, minimum, maximum, -1, 1);
          }
        }

        fundamental = f0(t, peakCount.get(), data.data(), data.size(),
                         audioIO().framesPerSecond());
      }

      io.out(0) = io.out(1) = f;
    }
  }

  bool onKeyDown(const Keyboard& k) override {
    minimum = std::numeric_limits<float>::max();
    maximum = -std::numeric_limits<float>::max();
  }

  void onDraw(Graphics& g) override {
    g.clear(0.3);
    g.camera(Viewpoint::IDENTITY);  // Ortho [-1:1] x [-1:1]
    g.color(0, 1, 1);
    g.draw(spectrum);
    g.color(0, 0, 0);
    g.draw(lower);

    g.pushMatrix();
    g.color(1, 0, 0);
    g.translate(
        diy::map(fundamental, 0, audioIO().framesPerSecond() / 2, -1, 1), 0, 0);
    g.draw(vertical);
    g.popMatrix();

    g.pushMatrix();
    g.color(1, 1, 0);
    g.translate(0, diy::map(t, minimum, maximum, -1, 1), 0);
    g.draw(horizontal);
    g.popMatrix();

    gui.draw(g);
  }
};

int main() {
  MyApp app;
  app.start();
}
