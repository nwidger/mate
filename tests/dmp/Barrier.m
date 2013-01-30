class Barrier {
  Integer P;
  Integer Q;
  Integer G;
  Object lock;
  
  Barrier(Integer P) {
    this.P = P;
    this.Q = P;
    this.G = 0;
    this.lock = new Object();
  }

  Object await() {
    synchronized(this.lock) {
      this.Q = this.Q - 1;

      if (this.Q.equals(0)) {
	this.Q = this.P;
	this.G = this.G + 1;
	// out "inc G to " + this.G.toString() + newline;
	this.lock.notifyAll();
	return null;
      }

      Integer g;
	
      g = this.G;
	
      while (g.equals(this.G)) {
	this.lock.wait();
	// out "woke up, g = " + g.toString() + " G = " + this.G.toString() + newline;
      }
    }

    return null;
  }
}
