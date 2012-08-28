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
