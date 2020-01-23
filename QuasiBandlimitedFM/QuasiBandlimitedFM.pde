import controlP5.*;
ControlP5 cp5;

QuasiBandlimitedAnalogSynth saw, pulse;

final float N = 2.1;

MinMaxMeanPower a, b;

void setup() {
  //size(800, 450);
  fullScreen();
  colorMode(HSB, 100);


  a = new MinMaxMeanPower("pulse:");
  b = new MinMaxMeanPower("saw:");
  cp5 = new ControlP5(this);
  saw = new QuasiBandlimitedAnalogSynth();
  pulse = new QuasiBandlimitedAnalogSynth();
  frameRate(20);

  cp5.addSlider("saw Hz")
    .setPosition(50, height - 150)
    .setSize(10, 100)
    .setRange(0, 2)
    .setValue(1.0)
    .plugTo(saw, "frequency");

  cp5.addSlider("pulse Hz")
    .setPosition(100, height - 150)
    .setSize(10, 100)
    .setRange(0, 2)
    .setValue(1.0)
    .plugTo(pulse, "frequency");

  cp5.addSlider("pulse %")
    .setPosition(150, height - 150)
    .setSize(10, 100)
    .setRange(0, 1)
    .setValue(0.5)
    .plugTo(pulse, "pulseWidth");

  cp5.addSlider("saw F")
    .setPosition(200, height - 150)
    .setSize(10, 100)
    .setRange(-1.2, 1.2)
    .setValue(1.0)
    .plugTo(saw, "filter");

  cp5.addSlider("pulse F")
    .setPosition(250, height - 150)
    .setSize(10, 100)
    .setRange(-1.2, 1.2)
    .setValue(1.0)
    .plugTo(pulse, "filter");
}

float mtof(float m) { 
  return pow(2.0f, m / 12.0f);
}

void draw() {
  if (!keyPressed) {
    saw.reset();
    pulse.reset();
  }

  //a.frequency(mtof(map(mouseY, height, 0.0, 0.0, 32.0)), map(mouseX, 0.0, width, -1.0, 1.0));

  /////////////////////////////////////////
  //// Draw (below) / Simulate (above) ////
  /////////////////////////////////////////

  background(63);

  stroke(0, 100, 75);
  {
    float _y = pulse.pulse();
    a.take(_y);
    //_y -= a.mean;
    _y = map(_y, -4, 4, height, 0);
    for (int x = 1; x < width; ++x) {
      float y = pulse.pulse();
      a.take(y);
      //y -= a.mean;
      y = map(y, -4, 4, height, 0);
      line(x - 1, _y, x, y);
      _y = y;
    }
    float mean = map(a.mean, -N, N, height, 0);
    line(0, mean, width, mean);
    
    //text("pulse :: min:" + a.min + " max:" + a.max, 300, 400);
  }

  stroke(66, 100, 75);
  {
    float _y = saw.saw();
    b.take(_y);
    //_y -= b.mean;
    _y = map(_y, -N, N, height, 0);
    for (int x = 1; x < width; ++x) {
      float y = saw.saw();
      b.take(y);
      //y -= b.mean;
      y = map(y, -N, N, height, 0);
      line(x - 1, _y, x, y);
      _y = y;
    }
    float mean = map(b.mean, -N, N, height, 0);
    line(0, mean, width, mean);
    //text("saw :: min:" + b.min + " max:" + b.max, 300, 300);
  }


  stroke(33, 50, 50);
  float high = map(1.0, -N, N, height, 0);
  float low = map(-1.0, -N, N, height, 0);
  line(0, high, width, high);
  line(0, low, width, low);
}