
//
// "Synthesis of Quasi-Bandlimited Analog Waveforms Using Frequency Modulation"
//   by Peter Schoffhauzer
// (http://scp.web.elte.hu/papers/synthesis1.pdf)
//
class QuasiBandlimitedAnalogSynth {
  float a0 = 2.5;   // precalculated coeffs
  float a1 = -1.5;  // for HF compensation

  // variables
  float osc;      // output of the saw oscillator
  float osc2;     // output of the saw oscillator 2
  float phase;    // phase accumulator
  float w;        // normalized frequency
  float scaling;  // scaling amount
  float DC;       // DC compensation
  float norm;     // normalization amount
  float last;     // delay for the HF filter

  float frequency, filter, pulseWidth;

  QuasiBandlimitedAnalogSynth() {
    reset();
    frequency = 1.0;
    filter = 1.0;
    pulseWidth = 0.5;
    recalculate();
  }

  void reset() {
    // zero oscillator and phase
    osc = 0.0;
    osc2 = 0.0;
    phase = 0.0;
  }

  void recalculate() {
    w = frequency / width;  // normalized frequency
    float n = 0.5 - w;
    scaling = filter * 13.0f * pow(n, 5); // calculate scaling
    DC = 0.376 - w * 0.752;         // calculate DC compensation
    norm = 1.0 - 2.0 * w;           // calculate normalization
  }

  void frequency(float f) {
    frequency = f;
    recalculate();
  }

  void filter(float f) {
    filter = f;
    recalculate();
  }
  
  void pulseWidth(float w) {
    pulseWidth = w;
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
    osc = (osc + sin(2 * PI * (phase + osc * scaling))) * 0.5;
    // compensate HF rolloff
    float out = a0 * osc + a1 * last;
    last = osc;
    out = out + DC; // compensate DC offset
    return (out * norm - 0.37217414) / 1.52;
    //return out * norm /  1.89;
    //return (out * norm - 0.4) / 1.3;
    //return (out * norm - 0.19449006) / 1.46599684;
    // saw: min:-0.8336497 max:1.6937943 mean:0.37217414 power:0.7965761
  }


  float pulse() {
    step();

    // calculate saw1
    osc = (osc + sin(2 * PI * (phase + osc * scaling))) * 0.5;
    // calculate saw2
    osc2 = (osc2 + sin(2 * PI * (phase + osc2 * scaling + pulseWidth))) * 0.5;
    float out = osc - osc2;  // subtract two saw waves
    // compensate HF rolloff
    out = a0 * out + a1 * last;
    last = osc;
    return out * norm / 3.702;
    //return out * norm / 3.5;
    //return (out * norm - 0.68514013) / 2.343291820105159;
    // pulse: min:-3.4910474 max:3.4910223 mean:1.383575E-4 power:2.4606218
  }
}