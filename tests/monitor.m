class MonitorTest {
  Integer x;

  MonitorTest() {

  }

  Integer getX() {
    synchronized (x) {
      return x;
    }
  }
}

Integer main() {
  return 0;
}
