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
  SharedObj shared;

  TestThread(Integer idx, SharedObj shared) {
    super();

    this.idx = idx;
    this.shared = shared;
  }

  Object run() {
    Integer i;
    
    synchronized (this.shared) {
      this.shared.count = this.shared.count - 1;
    }

    while (!this.shared.count.equals(0));

    this.shared.x = idx;
    this.shared.y = idx;
    this.shared.z = idx;

    this.sleep(50);

    this.idx = this.shared.x + this.shared.y + this.shared.z;

    this.shared.x = this.shared.z * idx;
    this.shared.y = this.shared.x * idx;
    this.shared.z = this.shared.y * idx;

    this.sleep(50);

    this.idx = this.shared.x + this.shared.y + this.shared.z;    

    this.shared.x = this.shared.y * idx;
    this.shared.y = this.shared.z * idx;
    this.shared.z = this.shared.x * idx;
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
    SharedObj shared;
    TestThread thread;
    Integer i, NumThreads;

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

    // print results
    out shared.toString() + newline;

    return 0;
  }
}

Integer main() {
  return new Main().go();
}
