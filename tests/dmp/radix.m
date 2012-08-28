#include "BinaryInteger.m"
#include "BinaryIntegerTable.m"
#include "IntegerTable.m"
#include "Tuple.m"
#include "TupleTable.m"

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
  Object dout(String msg) {
	  if (0) {
		  out msg;
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
    IntegerTable C, C2;
    TupleTable tary;
    BinaryInteger b1, b2;
    Integer i, max, temp1, temp2, temp3, ii, xx, x, sum;

    if (nbits > 32)
      return 1;

    tary = new TupleTable(n);

    max = 0;

    for (i = 0; i < n; i = i + 1) {
      b1 = ary.get(i);
      b2 = b1.mask(nbits);

      // dout("ary.get(" + i.toString() + ") = " + b1.toString() + ", ary.get().mask(" + nbits.toString() + ") = " + b2.toString() + newline);

      tary.put(i, new Tuple(b1, b2));

      if (b2.toInteger() > max)
	max = b2.toInteger();
    }

    max = max + 1;

    C = new IntegerTable(max);

    if (1) {			// parallel
      Counter c;
      Table counters;
      Integer numCounters, rem, beg, end;

      numCounters = 16;

      counters = new Table(numCounters);

      if (numCounters > n) {
	      out "numCounters must not be greater than n!" + newline;
	      return 1;
      }

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

	      // dout("counter " + i.toString() + " beg=" + beg.toString() + ", end=" + end.toString() + " rem=" + rem.toString() + newline);

	      i = i + 1;
      }

      numCounters = i;

      for (i = 0; i < numCounters; i = i + 1) {
	c = (Counter)counters.get(i);
	c.start();
      }

      for (i = 0; i < numCounters; i = i + 1) {
	c = (Counter)counters.get(i);

	c.join();

	ii = c.C.firstKey();
	for (ii = c.C.nextKey(); ii != null; ii = c.C.nextKey()) {
	  temp1 = ii;
	  temp2 = c.C.get(temp1);
	  temp3 = C.get(temp1);
		// dout("i = " + i.toString() + " temp1 = " + temp1.toString() + " temp2 = " + temp2.toString() + " temp3 = " + temp3.toString() + newline);
	  C.put(temp1, temp2 + temp3);
	}
      }
    } else {			// serial
      for (i = 0; i < n; i = i + 1) {
	temp1 = tary.get(i).masked.toInteger();
	temp2 = C.get(temp1);
	C.put(temp1, temp2 + 1);
      }
    }

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

    return 0;
  }
}

////////////////////////////////////////////////////////////////////////////////

Integer main() {
  Sort s;
  String sn, snbits;
  BinaryInteger temp;
  Integer i, n, nbits;
  BinaryIntegerTable ary;
  
  sn = in;
  snbits = in;

  if (sn == null || snbits == null) {
    out "Usage: n nbits [NUMBERS...]" + newline;
    return 1;
  }

  n = sn.toInteger();
  nbits = snbits.toInteger();

  if (n <= 0 || nbits <= 0 || nbits > 32) {
    out "n and nbits must be non-negative, nbits must be <= 32" + newline;
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

  s = new Sort();

  // if (s.doCountingSort(ary, n, nbits).equals(1)) {
  if (s.doRadixSort(ary, n, nbits).equals(1)) {
    out "error!" + newline;
    return 1;
  }

  for (i = 0; i < n; i = i + 1) {
    out ary.get(i).toString() + newline;
  }

  return 0;
}
