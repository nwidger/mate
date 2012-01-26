class SharedObj {
  Integer threadCount;

  Table table;
  Integer size;

  Integer value;

  SharedObj(Integer threadCount) {
    this.threadCount = threadCount;

    this.table = new Table();
    this.size = 0;
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
    Integer v, nulls;
    
    synchronized (this.shared) {
      this.shared.threadCount = this.shared.threadCount - 1;
    }

    while (!this.shared.threadCount.equals(0));
    
    if (this.idx.equals(0)) {
      // consumer
      sleep(3000);

      nulls = 0;

      while (nulls < 3) {
	synchronized (this.shared.table) {
	  if (!this.shared.size.equals(0)) {
	    v = (Integer)this.shared.table.remove(this.shared.size-1);

	    if (v != null) {
	      out "v = " + v.toString() + newline;	      
	    } else {
	      out "v = null" + newline;
	      nulls = nulls + 1;
	    }
	  }

	  this.shared.size = this.shared.size - 1;
	}

	sleep(500);
      }
    } else {
      // producers
      synchronized (this.shared.table) {
	this.shared.table.put(this.shared.size, this.idx);
      }

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
      thread = new TestThread(i, shared);
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
