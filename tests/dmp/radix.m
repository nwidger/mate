////////////////////////////////////////////////////////////////////////////////

class BinaryInteger {
  String str;
  Integer value;
  IntegerTable bits;

  BinaryInteger(Integer v) {
    if (v < 0) {
      out "Error: BinaryInteger passed negative number, using zero instead" + newline;
      v = 0;
    }
    
    str = null;
    value = v;
    regenerate();
  }

  BinaryInteger(String s) {
    if (s.length() == 0) {
      out "Error: BinaryInteger passed zero-length string, using zero instead" + newline;
      s = "0";
    }

    str = s;
    value = null;
    regenerate();
  }

  BinaryInteger(IntegerTable b) {
    str = null;
    value = null;
    bits = b;
  }
  
  Object regenerate() {
    String s;
    Integer i, j, v, b;
    
    bits = new IntegerTable(32);

    for (i = 0; i < 32; i = i + 1) {
      bits.put(i, 0);
    }

    if (str != null) {		// use str
      i = str.length() - 1;
      j = 0;

      while (i >= 0 && j < 32) {
	v = str.substr(i, i).toInteger();
	bits.put(j, v);
	
	i = i - 1;
	j = j + 1;
      }
    } else {			// use value
      v = value;

      b = 30;
      i = 1073741824;		// 2^30
    
      while (v > 0 && b >= 0) {
	if (v >= i) {
	  v = v - i;
	  bits.put(b, 1);
	}

	i = i / 2;
	b = b - 1;
      }
    }

    return null;
  }

  BinaryInteger mask(Integer nbits) {
    Integer i;
    IntegerTable newbits;

    newbits = new IntegerTable(32);

    for (i = 31; i > (nbits-1) && i >= 0; i = i - 1) {
      newbits.put(i, 0);
    }

    while (i >= 0) {
      newbits.put(i, bits.get(i));
      i = i - 1;
    }

    return new BinaryInteger(newbits);
  }

  BinaryInteger and(BinaryInteger bi) {
    Integer i, v1, v2, b;
    IntegerTable newbits;

    newbits = new IntegerTable(32);

    for (i = 0; i < 32; i = i + 1) {
      v1 = this.bits.get(i);
      v2 = bi.bits.get(i);

      if (v1.equals(1) && v2.equals(1))
	b = 1;
      else
	b = 0;

      newbits.put(i, b);
    }

    return new BinaryInteger(newbits);
  }

  BinaryInteger or(BinaryInteger bi) {
    Integer i, v1, v2, b;
    IntegerTable newbits;

    newbits = new IntegerTable(32);

    for (i = 0; i < 32; i = i + 1) {
      v1 = this.bits.get(i);
      v2 = bi.bits.get(i);

      if (v1.equals(1) || v2.equals(1))
	b = 1;
      else
	b = 0;

      newbits.put(i, b);
    }

    return new BinaryInteger(newbits);
  }

  BinaryInteger xor(BinaryInteger bi) {
    Integer i, v1, v2, b;
    IntegerTable newbits;

    newbits = new IntegerTable(32);

    for (i = 0; i < 32; i = i + 1) {
      v1 = this.bits.get(i);
      v2 = bi.bits.get(i);

      if ((v1.equals(1) && v2.equals(0)) ||
	  (v2.equals(1) && v1.equals(0))) {
	b = 1;
      } else {
	b = 0;
      }

      newbits.put(i, b);
    }

    return new BinaryInteger(newbits);
  }

  BinaryInteger shiftLeft(Integer n) {
    Integer v, i;

    if (n < 0) {
      out "Error: BinaryInteger passed negative shift, returning zero" + newline;
      return new BinaryInteger(0);
    }
    
    if (n.equals(0))
      return this;

    if (n >= 32)
      return new BinaryInteger(0);

    v = toInteger();

    for (i = 0; !v.equals(0) && i < n; i = i + 1) {
      v = v * 2;
    }

    return new BinaryInteger(v);
  }

  BinaryInteger shiftRight(Integer n) {
    Integer v, i;

    if (n < 0) {
      out "Error: BinaryInteger passed negative shift, returning zero" + newline;
      return new BinaryInteger(0);
    }
    
    if (n.equals(0))
      return this;

    if (n >= 32)
      return new BinaryInteger(0);

    v = toInteger();

    for (i = 0; !v.equals(0) && i < n; i = i + 1) {
      v = v / 2;
    }

    return new BinaryInteger(v);
  }
  
  Integer toInteger() {
    Integer i, b, v;
    
    if (value == null) {
      v = 0;

      b = 30;
      i = 1073741824;		// 2^30
    
      while (b >= 0) {
	if (bits.get(b).equals(1)) {
	  v = v + i;
	}

	i = i / 2;
	b = b - 1;
      }

      value = v;
    }
    
    return value;
  }

  Integer hashCode() {
    return toInteger();
  }
  
  String toString() {
    return toInteger().toString();
  }

  String toBinaryString() {
    String s;
    Integer i;
    
    if (str == null) {
      s = "";
      for (i = 0; i < 32; i = i + 1) {
	s = bits.get(i).toString() + s;
      }

      str = s;
    }

    return str;
  }
}

////////////////////////////////////////////////////////////////////////////////

class Tuple {
  BinaryInteger value;
  BinaryInteger masked;

  Tuple() {
    value = new BinaryInteger(0);
    masked = new BinaryInteger(0);
  }

  Tuple(BinaryInteger v, BinaryInteger m) {
    value = v;
    masked = m;
  }
}

////////////////////////////////////////////////////////////////////////////////

class TupleTable {
  Table t;
  
  TupleTable() {
    t = new Table();
  }
  
  TupleTable(Integer n) {
    t = new Table(n);
  }

  Tuple get(Integer key) {
    Object value;

    value = t.get(key);

    if (value == null)
      return null;

    return (Tuple)value;
  }

  Tuple put(Integer key, Tuple value) {
    Object prev;

    prev = t.put(key, value);

    if (prev == null)
      return null;

    return (Tuple)prev;
  }

  Tuple remove(Integer key) {
    Object value;

    value = t.remove(key);

    if (value == null)
      return null;

    return (Tuple)value;
  }

  Integer firstKey() {
    return t.firstKey();
  }
  
  Integer nextKey() {
    Object key;

    key = t.nextKey();

    if (key == null)
      return null;

    return (Integer)key;
  }
}

////////////////////////////////////////////////////////////////////////////////

class IntegerTable {
  Table t;
  
  IntegerTable() {
    t = new Table();
  }
  
  IntegerTable(Integer n) {
    t = new Table(n);
  }

  Integer get(Integer key) {
    Object value;

    value = t.get(key);

    if (value == null)
      return 0;

    return (Integer)value;
  }

  Integer put(Integer key, Integer value) {
    Object prev;

    prev = t.put(key, value);

    if (prev == null)
      return 0;

    return (Integer)prev;
  }

  Integer remove(Integer key) {
    Object value;

    value = t.remove(key);

    if (value == null)
      return 0;

    return (Integer)value;
  }

  Integer firstKey() {
    return t.firstKey();
  }

  Integer nextKey() {
    Object key;

    key = t.nextKey();

    if (key == null)
      return null;

    return (Integer)key;
  }
}

////////////////////////////////////////////////////////////////////////////////

class BinaryIntegerTable {
  Table t;
  
  BinaryIntegerTable() {
    t = new Table();
  }
  
  BinaryIntegerTable(Integer n) {
    t = new Table(n);
  }

  BinaryInteger get(Integer key) {
    Object value;

    value = t.get(key);

    if (value == null)
      return null;

    return (BinaryInteger)value;
  }

  BinaryInteger put(Integer key, BinaryInteger value) {
    Object prev;

    prev = t.put(key, value);

    if (prev == null)
      return null;

    return (BinaryInteger)prev;
  }

  BinaryInteger remove(Integer key) {
    Object value;

    value = t.remove(key);

    if (value == null)
      return null;

    return (BinaryInteger)value;
  }

  Integer firstKey() {
    return t.firstKey();
  }

  Integer nextKey() {
    Object key;

    key = t.nextKey();

    if (key == null)
      return null;

    return (Integer)key;
  }
}

////////////////////////////////////////////////////////////////////////////////

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
    return go();
  }

  Object go() {
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
  Object doRadixSort(BinaryIntegerTable ary, Integer n, Integer nbits) {
    Integer i;

    for (i = 8; i <= nbits; i = i * 2) {
      if (doCountingSort(ary, n, i) == 1) {
	return 1;
      }
    }

    return 0;
  }

  Integer getIndex(Integer masked, IntegerTable C) {
    Integer k, retval, sum;

    sum = 1;
    retval = 1;
    
    C.firstKey();
    for (k = C.nextKey(); k != null; k = C.nextKey()) {
      if (k <= masked) {
	retval = sum;
      }
      
      sum = sum + C.get(k);
    }

    return retval;
  }
  
  Integer doCountingSort(BinaryIntegerTable ary, Integer n, Integer nbits) {
    Tuple t;
    IntegerTable C;
    TupleTable tary;
    BinaryInteger b1, b2;
    Integer i, max, temp1, temp2, temp3, ii;

    if (nbits > 32)
      return 1;

    tary = new TupleTable(n);

    max = 0;

    for (i = 0; i < n; i = i + 1) {
      b1 = ary.get(i);
      b2 = b1.mask(nbits);
      tary.put(i, new Tuple(b1, b2));

      if (b2.toInteger() > max)
	max = b2.toInteger();
    }

    max = max + 1;

    C = new IntegerTable(max);

    if (1) {
      Counter c;
      Table counters;
      
      counters = new Table(10);

      for (i = 0; i < n; i = i + 1) {
	c = new Counter(i, i + 1, new IntegerTable(max), tary);
	counters.put(i, c);
      }

      for (i = 0; i < n; i = i + 1) {
	c = (Counter)counters.get(i);
	c.start();
      }

      for (i = 0; i < n; i = i + 1) {
	c = (Counter)counters.get(i);

	c.join();

	for (ii = c.i; ii < c.n; ii = ii + 1) {
	  temp1 = tary.get(ii).masked.toInteger();
	  temp2 = c.C.get(temp1);
	  temp3 = C.get(temp1);
	  C.put(temp1, temp2 + temp3);
	}
      }
    } else {
      for (i = 0; i < n; i = i + 1) {
	temp1 = tary.get(i).masked.toInteger();
	temp2 = C.get(temp1);
	C.put(temp1, temp2 + 1);
      }
    }

    for (i = n - 1; i >= 0; i = i - 1) {
      t = tary.get(i);

      temp1 = t.masked.toInteger();
      temp2 = getIndex(temp1, C);

      ary.put(temp2 - 1, t.value);

      temp1 = temp2;
      temp2 = C.get(temp1);

      if (temp2.equals(0))
      	C.remove(temp1);
      else
      	C.put(temp1, temp2 - 1);
    }

    // for (i = 1; i < max; i = i + 1) {
    //   temp1 = C.get(i);
    //   temp2 = C.get(i-1);
    //   C.put(i, temp1 + temp2);
    // }

    // for (i = n - 1; i >= 0; i = i - 1) {
    //   t = tary.get(i);
      
    //   temp1 = t.masked.toInteger();
    //   temp2 = C.get(temp1);
    //   ary.put(temp2 - 1, t.value);
      
    //   temp1 = t.masked.toInteger();
    //   temp2 = C.get(temp1);
    //   C.put(temp1, temp2 - 1);
    // }

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

  if (s.doCountingSort(ary, n, nbits).equals(1)) {
    out "error!" + newline;
    return 1;
  }

  for (i = 0; i < n; i = i + 1) {
    out ary.get(i).toString() + newline;
  }

  return 0;
}
