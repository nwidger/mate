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
    if (s.length().equals(0)) {
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
