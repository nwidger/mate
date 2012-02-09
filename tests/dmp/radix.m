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
    
      while (b >= 0) {
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
    Integer i, k, v;
    IntegerTable newbits;

    newbits = new IntegerTable(32);

    bits.firstKey();

    while ((k = bits.nextKey()) != null) {
      v = bits.get(k);
      newbits.put(k, v);
    }

    for (i = 31; i > (nbits-1) && i >= 0; i = i - 1) {
      newbits.put(i, 0);
    }

    return new BinaryInteger(newbits);
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
  
  String toString() {
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

class Tuple {
  Integer value;
  Integer masked;

  Tuple() {
    value = 0;
    masked = 0;
  }

  Tuple(Integer v, Integer m) {
    value = v;
    masked = m;
  }
}

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
      return null;

    return (Integer)value;
  }

  Integer put(Integer key, Integer value) {
    Object prev;

    prev = t.put(key, value);

    if (prev == null)
      return null;

    return (Integer)prev;
  }

  Integer remove(Integer key) {
    Object value;

    value = t.remove(key);

    if (value == null)
      return null;

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

class RadixSort {
  Object doSort(IntegerTable ary, Integer n, Integer nbits) {
    Integer i;

    for (i = 8; i <= nbits; i = i * 2) {
      if (doCountingSort(ary, n, i) == 1) {
	return 1;
      }
    }

    return 0;
  }

  Integer mask(Integer v, Integer nbits) {
    return new BinaryInteger(v).mask(nbits).toInteger();
  }

  Integer doCountingSort(IntegerTable ary, Integer n, Integer nbits) {
    Tuple t;
    IntegerTable C;
    TupleTable tary;
    Integer i, max, temp1, temp2, temp3;

    if (nbits > 20)
      return 1;

    tary = new TupleTable(n);

    for (i = 0; i < n; i = i + 1) {
      tary.put(i, new Tuple());
    }

    max = 0;
	
    for (i = 0; i < n; i = i + 1) {
      tary.get(i).value = ary.get(i);
      tary.get(i).masked = mask(ary.get(i), nbits);

      if (tary.get(i).masked > max)
	max = tary.get(i).masked;
    }

    max = max + 1;

    C = new IntegerTable(max);
    for (i = 0; i < max; i = i + 1) {
      C.put(i, 0);
    }

    for (i = 0; i < n; i = i + 1) {
      // C[tary[i].masked]++;
      // 
      // C.put(tary.get(i).masked, C.get(tary.get(i).masked)+1);

      temp1 = tary.get(i).masked;
      temp2 = C.get(temp1);
      C.put(temp1, temp2 + 1);
    }

    for (i = 1; i < max; i = i + 1) {
      // C[i] += C[i-1];
      // 
      // C.put(i, C.get(i) + C.get(i-1));

      temp1 = C.get(i);
      temp2 = C.get(i-1);
      C.put(i, temp1 + temp2);
    }

    for (i = n - 1; i >= 0; i = i - 1) {
      // ary[C[tary[i].masked]-1] = tary[i].value;
      // C[tary[i].masked-1]--;
      // 
      // ary.put(C.get(tary.get(i).masked)-1, tary.get(i).value);
      // C.put(tary.get(i).masked-1, C.get(tary.get(i).masked-1)-1);

      t = tary.get(i);
      
      temp1 = t.masked;
      temp2 = C.get(temp1) - 1;
      ary.put(temp2, t.value);
      
      temp1 = t.masked - 1;
      temp2 = C.get(temp1);
      C.put(temp1, temp2 - 1);
    }

    return 0;
  }
}

Integer main() {
  RadixSort rs;
  IntegerTable ary;
  String sn, snbits;
  Integer i, n, nbits, temp;

  sn = in;
  snbits = in;

  if (sn == null || snbits == null) {
    out "Usage: n nbits [NUMBERS...]" + newline;
    return 1;
  }

  n = sn.toInteger();
  nbits = snbits.toInteger();

  if (n <= 0 || nbits <= 0 || nbits > 32) {
    out "Bad arguments" + newline;
    return 1;
  }

  ary = new IntegerTable(n);

  for (i = 0; i < n; i = i + 1) {
    temp = in.toInteger();

    if (temp < 0) {
      out "Numbers must be non-negative" + newline;
      return 1;
    }
    
    ary.put(i, temp);
  }

  // PRE-SORT
  
  for (i = 0; i < n; i = i + 1) {
    out ary.get(i).toString();
    out " ";
  }

  out newline;

  rs = new RadixSort();

  if (rs.doSort(ary, n, nbits).equals(1)) {
    out "error!" + newline;
    return 1;
  }

  // POST-SORT

  for (i = 0; i < n; i = i + 1) {
    out ary.get(i).toString();
    out " ";
  }

  out newline;
  
  return 0;
}
