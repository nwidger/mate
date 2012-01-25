class SharedObj {
  Integer threadCount;

  Integer n;
  Integer done;

  SharedObj(Integer threadCount) {
    this.threadCount = threadCount;

    this.n = 0;
    this.done = 0;
  }
}

class TestThread extends Thread {
  Integer idx;
  SharedObj shared;

  TestThread(Integer idx, SharedObj shared) {
    super();

    this.idx = idx;
    this.shared = shared;
  }

  Integer mod(Integer a, Integer n) {
    if (a < 0) a = -a;
    if (n < 0) n = -n;
    return a - (n * (a/n));
  }

  Object run() {
    synchronized (this.shared) {
      this.shared.threadCount = this.shared.threadCount - 1;
    }

    while (!this.shared.threadCount.equals(0));

    if (this.idx.equals(1)) {
      out "n = " + newline;

      while (!this.shared.done) {
	sleep(500);
	out this.shared.n.toString() + " ";
	this.shared.n = 0;
      }

      out newline;
    } else {
      while (!this.shared.done) {
	this.shared.n = this.idx;

	if (this.shared.n.equals(0))
	  this.shared.done = 1;
      }
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

  Integer mod(Integer a, Integer n) {
    if (a < 0) a = -a;
    if (n < 0) n = -n;
    return a - (n * (a/n));
  }

  Integer go() {
    String str;
    Table threads;
    SharedObj shared;
    TestThread thread;
    Integer i, NumThreads;

    // Parse arguments
    if ((str = in) == null) {
      usage();
      return 1;
    }

    NumThreads = str.toInteger();

    if (NumThreads <= 0) {
      usage();
      return 1;
    }

    // Initialize array of thread structures
    threads = new Table(NumThreads);
    shared = new SharedObj(NumThreads);

    for (i = 0; i < NumThreads; i = i + 1) {
      thread = new TestThread(i+1, shared);
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

    return 0;
  }
}

Integer main() {
  return new Main().go();
}
