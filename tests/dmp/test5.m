class SharedObj {
  Integer n;

  SharedObj() {
    this.n = 0;
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
    if (mod(this.idx, 2).equals(0)) {
      out idx.toString() + ": even, setting n to n - 1" + newline;
      this.shared.n = this.shared.n - 1;
    } else {
      out idx.toString() + ": odd, setting n to n + 1" + newline;      
      this.shared.n = this.shared.n + 1;
    }
  }
}

class Main {
  Main() {

  }

  Object usage() {
    out "usage: <numThreads>" + newline;
    out "numThreads must be a non-zero even number." + newline;
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

    if (NumThreads <= 0 || !mod(NumThreads, 2).equals(0)) {
      usage();
      return 1;
    }

    // Initialize array of thread structures
    threads = new Table(NumThreads);
    shared = new SharedObj();

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
      out "joining thread... ";
      thread.join();
      out "done!" + newline;
    }

    if (shared.n == null) {
      out "n = null" + newline;
      return 1;
    } else {
      out "n = " + shared.n.toString() + newline;
      return 0;
    }
  }
}

Integer main() {
  return new Main().go();
}
