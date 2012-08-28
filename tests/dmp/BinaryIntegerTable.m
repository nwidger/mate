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
