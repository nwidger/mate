Integer main() {
  Object o, o2;
  String s, s2, s3;
  Table t;

  t = new Table(10);

  o = new Object();
  s = "hello";
  s3 = "foobar";

  o2 = new Object();
  s2 = "goodbye";

  t.put(o, s);
  t.put(o2, s2);

  out (String)(t.get(o)) + newline;
  out (String)(t.get(o2)) + newline;

  t.put(o, s3);

  out (String)(t.get(o)) + newline;

  return 0;
}
