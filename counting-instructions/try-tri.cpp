float tri(float phase) {
  float f = 2 * phase - 1;
  f = (f < -0.5) ? -1 - f : (f > 0.5 ? 1 - f : f);
  return 2 * f;
}
