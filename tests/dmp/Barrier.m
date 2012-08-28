class Barrier {
  Integer P;
  Integer Q;
  Integer go;
  Object lock;
  
  Barrier(Integer P) {
    this.P = P;
    this.Q = P;
    this.go = 0;
    this.lock = new Object();
  }

  Object await() {
    synchronized(this.lock) {
      this.Q = this.Q - 1;

      if (this.Q.equals(0)) {
	this.Q = this.P;
	this.go = 1;
	return null;
      }

      this.go = 0;
    }

    for (;;) {
      synchronized(this.lock) {
	if (this.go) {
	  break;
	}
      }
    }

    return null;
  }
}
