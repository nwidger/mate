class RaceyThread extends Thread {
  Main maine;
  Integer threadId;

  // taken out of main
  Integer MAX_LOOP;
  Integer MAX_ELEM;
  Integer PRIME1;
  Integer PRIME2;
  Object threadLock;
  Table m;
  Table sig;

  RaceyThread(Main maine, Integer tid) {
    super();
    this.maine = maine;
    this.threadId = tid;

    this.MAX_LOOP = maine.MAX_LOOP;
    this.MAX_ELEM = maine.MAX_ELEM;
    this.PRIME1 = maine.PRIME1;
    this.PRIME2 = maine.PRIME2;
    this.threadLock = maine.threadLock;
    this.m = maine.m;
    this.sig = maine.sig;
  }

  // The function which is called once the thread is created
  Object run() {
    Integer i;

    // seize the cpu, roughly 0.5-1 second on ironsides
    // i = 0;
    // while (i < 134217727)
    //   i = i + 1;

    // simple barrier, pass only once
    synchronized (threadLock) {
      maine.startCounter = maine.startCounter - 1;
      if (maine.startCounter == 0) {
	// start of parallel phase
	// PHASE_MARKER;
      }
    }
    while (!(maine.startCounter == 0));

    //
    // main loop:
    //
    // repeatedly using function "mux" to obtain two array indices, read two
    // array elements, mix and store into the 2nd
    //
    // If mix() is good, any race (except read-read, which can tell by software)
    // should change the final value of mix
    //
    i = 0;
    while (i < MAX_LOOP) {
      Integer num, index1, index2;

      num = (Integer)sig.get(threadId);
      index2 = maine.mod(num, MAX_ELEM);
      num = maine.mix(num, (Integer)m.get(index2));
      m.put(index2, num);
      sig.put(threadId, num);

      // Optionally, yield to other processors (Solaris use sched_yield())
      // pthread_yield();

      i = i + 1;
    }

    return null;
  }
}

class Main {
  Integer MAX_LOOP;
  Integer MAX_ELEM;
  Integer PRIME1;
  Integer PRIME2;

  Integer NumProcs;
  Integer startCounter;
  Object threadLock;

  Table sig;
  Table m;

  Table threads;

  Main() {
    Integer i;

    MAX_LOOP = 1000;
    MAX_ELEM = 64;

    PRIME1 = 103072243;
    PRIME2 = 103995407;

    sig = new Table(16);

    i = 0;
    while (i < 16) {
      sig.put(i, i);
      i = i + 1;
    }
  }

  // the mix function
  Integer mix(Integer i, Integer j) {
    if (i < 0) i = -i;
    if (j < 0) j = -j;
    return mod((i + j * PRIME2), PRIME1);
  }

  Integer mod(Integer a, Integer n) {
    if (a < 0) a = -a;
    if (n < 0) n = -n;
    return a - (n * (a/n));
  }

  Object usage() {
    out "usage: <numProcessors>" + newline;
    return null;
  }

  Integer go() {
    String str;
    RaceyThread thread;
    Integer i, mix_sig;

    // Parse arguments
    if (!((str = in) == null)) {
      NumProcs = str.toInteger();

      if (NumProcs < 0)
	usage();
      if (NumProcs > 16)
	usage();
    } else {
      usage();
      return 1;
    }

    threadLock = new Object();

    // Initialize the mix array
    m = new Table(MAX_ELEM);

    i = 0;
    while (i < MAX_ELEM) {
      m.put(i, mix(i, i));
      i = i + 1;
    }

    // Initialize barrier counter
    startCounter = NumProcs;

    // Initialize array of thread structures
    threads = new Table(NumProcs);

    i = 0;
    while (i < NumProcs) {
      thread = new RaceyThread(this, i);
      threads.put(i, thread);

      i = i + 1;
    }

    i = 0;
    while (i < NumProcs) {
      thread = (RaceyThread)threads.get(i);
      thread.start();
      i = i + 1;
    }

    // Wait for each of the threads to terminate
    i = 0;
    while (i < NumProcs) {
      ((RaceyThread)(threads.get(i))).join();
      i = i + 1;
    }

    // compute the result
    mix_sig = (Integer)sig.get(0);
    i = 1;
    while (i < NumProcs) {
      mix_sig = mix((Integer)sig.get(i), mix_sig);
      i = i + 1;
    }

    // end of parallel phase
    // PHASE_MARKER;

    // print results
    out "Short signature: " + mix_sig.toString() + newline;

    // PHASE_MARKER;

    return 0;
  }
}

Integer main() {
  return new Main().go();
}
