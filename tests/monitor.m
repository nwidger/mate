class MonitorTest {
  Integer x, y;

  MonitorTest() {

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
  return 0;
}
