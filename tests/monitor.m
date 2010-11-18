class MonitorTest {
  Integer x, y;

  MonitorTest() {
    x = 0;
    y = 0;
  }

  Integer getX() {
    while (1) {
      synchronized (x) {
	synchronized (y) {
	  if (0) {
	    break;
	  }
	  
	  return x;
	}
      }
    }

    return x;
  }
}

Integer main() {
  Integer x;
  MonitorTest m;

  m = new MonitorTest();
  x = m.getX();
  
  return x;
}
