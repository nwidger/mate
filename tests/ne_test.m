class Util {
  Util() {
  }

  Object pass() {
    out "pass" + newline;
  }

  Object fail() {
    out "fail" + newline;
  }
}

Integer main() {
  Util u;
  Integer a, b;

  u = new Util();

  a = 1;
  b = 2;

  if (a == a)
    u.pass();
  else
    u.fail();

  if (a != a)
    u.fail();
  else
    u.pass();

  if (a == b)
    u.fail();
  else
    u.pass();

  if (a != b)
    u.pass();
  else
    u.fail();
}
