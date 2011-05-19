// RACEY: a program print a result which is very sensitive to the
// ordering between processors (races).
//
// It is important to "align" the short parallel executions in the
// simulated environment. First, a simple barrier is used to make sure
// thread on different processors are starting at roughly the same time.
// Second, each thread is bound to a physical cpu. Third, before the main
// loop starts, each thread use a tight loop to gain the long time slice
// from the OS scheduler.
//
// NOTE: This program need to be customized for your own OS/Simulator
// environment. See TODO places in the code.

// TODO: replace PHASE_MARKER with your own function that marks a
// program phase.  Example being a simic "magic" instruction, or
// VMware backdoor call. Printf should be avoided since it may cause
// app/OS interaction, even de-scheduling.

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

    // Thread Initialization:
    //
    // Bind the thread to a processor.  This will make sure that each of
    // threads are on a different processor.  ProcessorIds[threadId]
    // specifies the processor ID which the thread is binding to.

    // TODO:
    // Bind this thread to ProcessorIds[threadId]
    // use processor_bind(), for example on solaris.

    out "thread " + threadId.toString() + ": starting seize-loop" + newline;

    // seize the cpu, roughly 0.5-1 second on ironsides
    // for (i = 0; i < 134217727; i = i + 1);
    for (i = 0; i < 10500; i = i + 1);

    out "thread " + threadId.toString() + ": out of seize-loop" + newline;    

    // simple barrier, pass only once
    synchronized (threadLock) {
      maine.startCounter = maine.startCounter - 1;
      if (maine.startCounter.equals(0)) {
	// start of parallel phase
	// PHASE_MARKER;
      }
    }
    while (!maine.startCounter.equals(0));

    //
    // main loop:
    //
    // repeatedly using function "mix" to obtain two array indices, read two
    // array elements, mix and store into the 2nd
    //
    // If mix() is good, any race (except read-read, which can tell by software)
    // should change the final value of mix
    //
    for (i = 0; i < MAX_LOOP; i = i + 1) {
      Integer num, index1, index2;
      num = (Integer)sig.get(threadId);
      index1 = maine.mod(num, MAX_ELEM);
      
      num = maine.mix(num, (Integer)m.get(index1));
      index2 = maine.mod(num, MAX_ELEM);
      num = maine.mix(num, (Integer)m.get(index2));
      m.put(index2, num);
      sig.put(threadId, num);
      // Optionally, yield to other processors (Solaris use sched_yield())
      // pthread_yield();
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

    for (i = 0; i < 16; i = i + 1)
      sig.put(i, i);
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
    if ((str = in) == null) {
      usage();
      return 1;
    }

    NumProcs = str.toInteger();

    if (NumProcs < 0 || NumProcs > 16) {
      usage();
      return 1;
    }

    threadLock = new Object();

    // Initialize the mix array
    m = new Table(MAX_ELEM);

    for (i = 0; i < MAX_ELEM; i = i + 1)
      m.put(i, mix(i, i));

    // Initialize barrier counter
    startCounter = NumProcs;

    // Initialize array of thread structures
    threads = new Table(NumProcs);

    for (i = 0; i < NumProcs; i = i + 1) {
      thread = new RaceyThread(this, i);
      threads.put(i, thread);
    }

    for (i = 0; i < NumProcs; i = i + 1) {
      thread = (RaceyThread)threads.get(i);
      thread.start();
    }

    // Wait for each of the threads to terminate
    for (i = 0; i < NumProcs; i = i + 1) {
      thread = (RaceyThread)threads.get(i);
      thread.join();
    }

    // compute the result
    mix_sig = (Integer)sig.get(0);
    for (i = 1; i < NumProcs; i = i + 1)    
      mix_sig = mix((Integer)sig.get(i), mix_sig);

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
