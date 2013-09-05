#include "BinaryInteger.m"
#include "BinaryIntegerTable.m"
#include "IntegerTable.m"
// #include "Timer.m"
#include "Tuple.m"
#include "TupleTable.m"

class Maxer extends Thread {
  Integer i;
  Integer n;
  Integer nbits;
  BinaryIntegerTable ary;
  TupleTable tary;
  Integer max;

  Maxer(Integer i, Integer n, Integer nbits, BinaryIntegerTable ary, TupleTable tary) {
    this.i = i;
    this.n = n;
    this.nbits = nbits;
    this.ary = ary;
    this.tary = tary;
    this.max = 0;
  }

  Object run() {
    Integer j, b2i;
    BinaryInteger b1, b2;
    
    for (j = i; j < n; j = j + 1) {
      b1 = ary.get(j);
      b2 = b1.mask(nbits);
      tary.put(j, new Tuple(b1, b2));

      b2i = b2.toInteger();
      if (b2i > max)
	max = b2i;
    }

    return null;
  }
}

class Counter extends Thread {
  Integer i;
  Integer n;
  IntegerTable C;
  TupleTable tary;

  Counter(Integer i, Integer n, IntegerTable C, TupleTable tary) {
    this.i = i;
    this.n = n;
    this.C = C;
    this.tary = tary;
  }

  Object run() {
    Integer j, temp1, temp2;
    
    for (j = i; j < n; j = j + 1) {
      temp1 = tary.get(j).masked.toInteger();
      temp2 = C.get(temp1);
      C.put(temp1, temp2 + 1);
    }

    return null;
  }
}

class Sort {
  Integer numCounters;

  Sort() {
    this.numCounters = 2;
  }
  
  Sort(Integer numCounters) {
    this.numCounters = numCounters;
  }
  
  Object dout(String msg) {
	  if (1) {
		  out msg + newline;
	  }

	  return null;
  }
	
  Object doRadixSort(BinaryIntegerTable ary, Integer n, Integer nbits) {
    Integer i;

    for (i = 8; i <= nbits; i = i * 2) {
      if (doCountingSort(ary, n, i).equals(1)) {
	return 1;
      }

      Integer j;

      // dout("--------------------------------------------------------------------------------" + newline);
      // dout("result" + newline);
      // for (j = 0; j < n; j = j + 1) {
      // 	      dout("nbits = " + i.toString() + " ary[" + j.toString() + "] = " +
      // 	      	      ary.get(j).toString() + " ary[" + j.toString() + "].mask(" + i.toString() + ") = " +
      // 	      	      ary.get(j).mask(i).toString() + newline);
      // }
    }

    return 0;
  }

  Integer doCountingSort(BinaryIntegerTable ary, Integer n, Integer nbits) {
    Tuple t;
    Integer rem, beg, end, b2i;
    IntegerTable C, C2;
    TupleTable tary;
    BinaryInteger b1, b2;
    Integer i, max, temp1, temp2, temp3, ii, xx, x, sum;

    if (nbits > 32)
      return 1;

    tary = new TupleTable(n);

    // dout("determining max");
    max = 0;

    if (1) {			// parallel
      Maxer m;
      Table maxers;
      Integer numMaxers;

      maxers = new Table(numCounters);

      // dout("dolling out numbers");
      i = 0;
      beg = 0;
      end = 0;
      rem = n;

      while (rem > 0) {
	      beg = end;

	      if (rem >= (n/numCounters))
		      end = end + (n/numCounters);
	      else
		      end = end + rem;
	      
	      rem = rem - (end - beg);

	      // dout("maxer " + i.toString() + " beg=" + beg.toString() + " end=" + end.toString());
	      m = new Maxer(beg, end, nbits, ary, tary);
	      maxers.put(i, m);

	      i = i + 1;
      }

      numMaxers = i;
      // dout("done dolling out numbers");

      for (i = 0; i < numMaxers; i = i + 1) {
	      // dout("starting maxer " + i.toString());
	      m = (Maxer)maxers.get(i);
	      m.start();
      }

      for (i = 0; i < numMaxers; i = i + 1) {
	      // dout("joining maxer " + i.toString());
	      m = (Maxer)maxers.get(i);
	      m.join();

	      if (m.max > max)
		      max = m.max;
      }
    } else {
	    for (i = 0; i < n; i = i + 1) {
		    b1 = ary.get(i);
		    b2 = b1.mask(nbits);

		    tary.put(i, new Tuple(b1, b2));

		    b2i = b2.toInteger();
		    if (b2i > max)
		      max = b2i;
	    }
    }

    max = max + 1;
    // dout("done determining max");

    C = new IntegerTable(max);

    if (1) {			// parallel
      Counter c;
      Table counters;

      // dout("dolling out numbers");
      counters = new Table(numCounters);

      i = 0;
      beg = 0;
      end = 0;
      rem = n;

      while (rem > 0) {
	      beg = end;

	      if (rem >= (n/numCounters))
		      end = end + (n/numCounters);
	      else
		      end = end + rem;
	      
	      rem = rem - (end - beg);
	      
	      c = new Counter(beg, end, new IntegerTable(max), tary);
	      counters.put(i, c);

	      i = i + 1;
      }

      numCounters = i;
      // dout("done dolling out numbers");

      for (i = 0; i < numCounters; i = i + 1) {
	c = (Counter)counters.get(i);
	// dout("starting thread " + (i+1).toString());
	c.start();
      }

      for (i = 0; i < numCounters; i = i + 1) {
	c = (Counter)counters.get(i);

	c.join();
	// dout("joined thread " + (i+1).toString());

	// dout("aggregating totals");

	ii = c.C.firstKey();
	for (ii = c.C.nextKey(); ii != null; ii = c.C.nextKey()) {
	  temp1 = ii;
	  temp2 = c.C.get(temp1);
	  temp3 = C.get(temp1);

	  C.put(temp1, temp2 + temp3);
	}

	// dout("done aggregating");
      }
    } else {			// serial
      for (i = 0; i < n; i = i + 1) {
	temp1 = tary.get(i).masked.toInteger();
	temp2 = C.get(temp1);
	C.put(temp1, temp2 + 1);
      }
    }

    // dout("doing rest");

    sum = 0;
    C2 = new IntegerTable(max);
    
    x = C.firstKey();
    for (x = C.nextKey(); x != null; x = C.nextKey()) {
	    xx = C.get(x);
	    C2.put(x, sum+xx);
	    sum = sum + xx;
    }

    C = C2;

    // dout("--------------------------------------------------------------------------------" + newline);
    // dout("dumping C" + newline);

    // x = C.firstKey();
    // for (x = C.nextKey(); x != null; x = C.nextKey()) {
    // 	    xx = C.get(x);
    // 	    dout("C[" + x.toString() + ", " + xx.toString() + "]" + newline);
    // }

    // dout("--------------------------------------------------------------------------------" + newline);

    for (i = n - 1; i >= 0; i = i - 1) {
      t = tary.get(i);

      temp1 = t.masked.toInteger();
      temp2 = C.get(temp1);
      C.put(temp1, temp2 - 1);

      temp2 = temp2 - 1;

      // dout("tary(" + i.toString() + ") = " + t.masked.toString() + ", index = " + temp2.toString() + newline);

      ary.put(temp2, t.value);
    }

    // dout("done doing rest");

    return 0;
  }
}

////////////////////////////////////////////////////////////////////////////////

Integer main() {
  Sort s;
  String sn, snbits, snc;
  BinaryInteger temp;
  BinaryIntegerTable ary;
  Integer i, n, nbits, numCounters;

  snc = in;

  if (snc == null) {
    out "Usage: nthreads n nbits [NUMBERS...]" + newline;
    return 1;
  }

  sn = in;
  snbits = in;

  if (sn == null || snbits == null) {
    out "Usage: nthreads n nbits [NUMBERS...]" + newline;
    return 1;
  }

  n = sn.toInteger();
  nbits = snbits.toInteger();

  if (n <= 0 || nbits <= 0 || nbits > 32) {
    out "n and nbits must be non-negative, nbits must be <= 32" + newline;
    return 1;
  }

  numCounters = snc.toInteger();

  if (numCounters > n) {
    out "nthreads must not be greater than n!" + newline;
    return 1;
  }

  ary = new BinaryIntegerTable(n);

  for (i = 0; i < n; i = i + 1) {
    temp = new BinaryInteger(in.toInteger());

    if (temp.toInteger() < 0) {
      out "Numbers must be non-negative" + newline;
      return 1;
    }
    
    ary.put(i, temp);
  }

  s = new Sort(numCounters);

  // Timer timer;

  // timer = new Timer();

  // timer.start_timer();

  // if (s.doCountingSort(ary, n, nbits).equals(1)) {
  if (s.doRadixSort(ary, n, nbits).equals(1)) {
    out "error!" + newline;
    return 1;
  }

  // timer.stop_timer();

  // for (i = 0; i < n; i = i + 1) {
  //   out ary.get(i).toString() + newline;
  // }

  // out "Total time: " + (new Real(timer.millisecs) / 1000.0).toString() + " sec" + newline;

  return 0;
}
