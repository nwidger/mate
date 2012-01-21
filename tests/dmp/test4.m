class SharedObj {
  Integer threadCount;
  
  Table table;
  Integer size;

  SharedObj(Integer threadCount) {
    this.threadCount = threadCount;
    
    this.table = new Table();
    size = 0;
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
      this.shared.threadCount = this.shared.threadCount - 1;      
    }

    while (!this.shared.threadCount.equals(0));    

    for (i = 0; i < 10; i = i + 1) {
      this.shared.table.put(this.idx, this.idx+i);
      this.shared.size = this.shared.size + 1;
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

    out "size = " + shared.size.toString() + newline;

    return 0;
  }
}

Integer main() {
  return new Main().go();
}
