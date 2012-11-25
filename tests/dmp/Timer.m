class Timer extends Thread {
  Integer started;
  Integer millisecs;
  Integer granularity;

  Timer() {
    this(5);
  }

  Timer(Integer g) {
    this.started = 0;
    this.millisecs = 0;
    this.granularity = g;
  }

  Object start_timer() {
    synchronized (this) {
      this.started = 1;
    }

    this.millisecs = 0;
    this.start();
  }

  Integer stop_timer() {
    Integer m;
    
    synchronized (this) {
      m = new Integer(this.millisecs);
      this.started = 0;
      
    }

    this.join();
    
    return m;
  }

  Object run() {
    for (;;) {
      synchronized (this) {
	if (!this.started) {
	  break;
	}
      }
      
      this.sleep(this.granularity);
      this.millisecs = this.millisecs + this.granularity;
    }
  }
}
