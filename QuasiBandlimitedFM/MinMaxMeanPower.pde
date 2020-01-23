class MinMaxMeanPower {
  static final int N = 100000;
  float min = 999999.9f, max = -999999.9f, sum = 0.0f;
  int index = 0;
  float[] data;
  String name;
  float squareSum = 0.0f;
  
  float mean = 0, power = 0;
  MinMaxMeanPower(String name) {
    this.name = name;
    data = new float[N];
  }
  void take(float v) {
    if (v < min) min = v;
    if (v > max) max = v;
    squareSum -= data[index] * data[index];
    sum -= data[index];
    data[index] = v;
    sum += data[index];
    squareSum += data[index] * data[index];
    index++;
    if (index == N) {
      index = 0;
      mean = sum / N;
      power = sqrt(squareSum / N);
      println(name + " min:" + min + " max:" + max + " mean:" + mean + " power:" + power);
    }
  }
}