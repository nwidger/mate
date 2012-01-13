class TestThread extends Thread {
  Integer idx;
  Table x;

  TestThread(Integer idx, Table x) {
    super();

    this.idx = idx;
    this.x = x;
  }

  Object run() {
    this.x.put(0, this.idx);
    sleep(500);
    this.x.put(0, this.idx);
    sleep(500);
    this.x.put(0, this.idx);
    sleep(500);
    this.x.put(0, this.idx);
    return null;
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
    TestThread thread;
    Table threads, retval;
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
    retval = new Table(NumThreads);

    for (i = 0; i < NumThreads; i = i + 1) {
      thread = new TestThread(i, retval);
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
    out "x.get(0): " + ((Integer)retval.get(0)).toString() + newline;

    return 0;
  }
}

Integer main() {
  return new Main().go();
}
