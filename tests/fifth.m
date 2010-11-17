Integer main() {
  Integer x, y, z;
  String a, b;
  Table t;

  t = new Table(10);

  x = 5;
  y = 5;
  z = 6;

  a = "hello";
  b = "goodbye";

  t.put(x, a);

  out ((String)t.get(x)) + newline;

  t.put(z, b);

  out ((String)t.get(z)) + newline;

  out ((String)t.get(y)) + newline;

  return 0;
}
