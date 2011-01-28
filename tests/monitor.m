class MonitorTest {
  Integer x, y, z;

  MonitorTest() {
    x = 0;
    y = 0;
    z = 0;
  }

  Integer getX() {
    synchronized (x) {
      while (0) {
	synchronized (y) {
	  synchronized (z) {
	    if (0) {
	      break;
	    }
	  
	    return x;
	  }
	}
      }

      return x;
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
