class RealTable {
  Table t;
  
  RealTable() {
    t = new Table();
  }
  
  RealTable(Integer n) {
    t = new Table(n);
  }

  Real get(Integer key) {
    return this.get(new Real(key));
  }
  
  Real get(Real key) {
    Object value;

    value = t.get(key);

    if (value == null)
      return 0.0;

    return (Real)value;
  }

  Real put(Integer key, Real value) {
    return this.put(new Real(key), value);
  }

  Real put(Real key, Real value) {
    Object prev;

    prev = t.put(key, value);

    if (prev == null)
      return 0.0;

    return (Real)prev;
  }

  Real remove(Real key) {
    Object value;

    value = t.remove(key);

    if (value == null)
      return 0.0;

    return (Real)value;
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
