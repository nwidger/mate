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
