class MDRealTable {
  Table t;
  Integer c, r;
  
  MDRealTable() {
    this(10, 10);
  }
  
  MDRealTable(Integer c, Integer r) {
    this.c = c;
    this.r = r;
    t = new Table(c*r);
  }

  Integer makeKey(Integer k1, Integer k2) {
    if (k1 < 0 || k1 >= c ||
	k2 < 0 || k2 >= r) {
      out "Bad index [" + k1.toString() + "][" + k2.toString() + "]" + newline;
    }
    
    return (k1 * 1000) + k2;
  }

  Real get(Integer k1, Integer k2) {
    Integer key;
    Object value;

    key = makeKey(k1, k2);

    synchronized(t) {
      value = t.get(key);
    }

    if (value == null)
      return 0.0;

    return (Real)value;
  }

  Real put(Integer k1, Integer k2, Real value) {
    Object prev;
    Integer key;

    key = makeKey(k1, k2);

    synchronized(t) {
      prev = t.put(key, value);
    }

    if (prev == null)
      return 0.0;

    return (Real)prev;
  }

  Real remove(Integer k1, Integer k2) {
    Integer key;
    Object value;

    key = makeKey(k1, k2);

    synchronized(t) {
      value = t.remove(key);
    }

    if (value == null)
      return 0.0;

    return (Real)value;
  }

  Integer firstKey() {
    Integer key;

    synchronized(t) {
      key = t.firstKey();
    }

    return key;
  }

  Integer nextKey() {
    Object key;

    synchronized(t) {
      key = t.nextKey();
    }

    if (key == null)
      return null;

    return (Integer)key;
  }
}
