class SharedObj {
  Integer x;
  Integer y;
  Integer z;
  Integer count;

  SharedObj(Integer count) {
    this.count = count;

    this.x = 0;
    this.y = 0;
    this.z = 0;
  }

  String toString() {
    return "[ " +
      "x = " + x.toString() + ", " +
      "y = " + y.toString() + ", " +
      "z = " + z.toString() +
      " ]";
  }
}

class TestThread extends Thread {
  Integer idx;
  SharedObj shared1, shared2;

  TestThread(Integer idx, SharedObj shared1, SharedObj shared2) {
    super();

    this.idx = idx;
    this.shared1 = shared1;
    this.shared2 = shared2;
  }

  Integer mod(Integer a, Integer n) {
    if (a < 0) a = -a;
    if (n < 0) n = -n;
    return a - (n * (a/n));
  }

  Object run() {
    Integer i;

    if (mod(idx, 2).equals(0)) {
      out "*** even thread, acquiring shared1" + newline;
      synchronized (this.shared1) {
	out "*** even thread, acquired shared1" + newline;
	out "*** even thread, acquiring shared2" + newline;
	synchronized (this.shared2) {
	  out "*** even thread, acquired shared2" + newline;
	  this.shared1.count = this.shared1.count - 1;
	  out "*** even thread, releasing shared2" + newline;
	}
	out "*** even thread, released shared2" + newline;
	out "*** even thread, releasing shared1" + newline;
      }
      out "*** even thread, released shared1" + newline;
    } else {
      out "=== odd thread, acquiring shared2" + newline;
      synchronized (this.shared2) {
	out "=== odd thread, acquired shared2" + newline;
	out "=== odd thread, acquiring shared1" + newline;
	synchronized (this.shared1) {
	  out "=== odd thread, acquired shared1" + newline;
	  this.shared1.count = this.shared1.count - 1;
	  out "=== odd thread, releasing shared1" + newline;
	}
	out "=== odd thread, released shared1" + newline;
	out "=== odd thread, releasing shared2" + newline;
      }
      out "=== odd thread, released shared2" + newline;
    }
  }
}

class Main {
  Main() {

  }

  Object usage() {
    out "usage: <numThreads>" + newline;
    return null;
  }

  Integer go() {
    String str;
    Table threads;
    TestThread thread;
    Integer i, NumThreads;
    SharedObj shared1, shared2;

    // Parse arguments
    if ((str = in) == null) {
      usage();
      return 1;
    }

    NumThreads = str.toInteger();

    if (NumThreads < 0) {
      usage();
      return 1;
    }

    // Initialize array of thread structures
    threads = new Table(NumThreads);
    shared1 = new SharedObj(NumThreads);
    shared2 = new SharedObj(NumThreads);

    for (i = 0; i < NumThreads; i = i + 1) {
      thread = new TestThread(i+1, shared1, shared2);
      threads.put(i, thread);
    }

    for (i = 0; i < NumThreads; i = i + 1) {
      thread = (TestThread)threads.get(i);
      thread.start();
    }

    // Wait for each of the threads to terminate
    for (i = 0; i < NumThreads; i = i + 1) {
      thread = (TestThread)threads.get(i);
      thread.join();
    }

    out "done!" + newline;

    return 0;
  }
}

Integer main() {
  return new Main().go();
}
