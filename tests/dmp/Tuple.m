class Tuple {
  BinaryInteger value;
  BinaryInteger masked;

  Tuple() {
    value = new BinaryInteger(0);
    masked = new BinaryInteger(0);
  }

  Tuple(BinaryInteger v, BinaryInteger m) {
    value = v;
    masked = m;
  }
}
