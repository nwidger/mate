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
  // Util u;
  // Integer x, a, b, c, d, e;

  // u = new Util();

  // a = 1;
  // b = 2;
  // c = 3;
  // d = 4;
  // e = 5;

  // x = 3;

  // if (x >= a)			// x = 3, a = 1
  //   u.pass();
  // else
  //   u.fail();

  // if (x <= a)			// x = 3, a = 1
  //   u.fail();
  // else
  //   u.pass();

  // if (x <= x)			// x = 3, x= 3
  //   u.pass();
  // else
  //   u.fail();

  // if (x >= x)			// x = 3, x = 3
  //   u.pass();
  // else
  //   u.fail();

  // if (x <= e)			// x = 3, e = 5;
  //   u.pass();
  // else
  //   u.fail();

  // if (x >= e)			// x = 3, e = 5
  //   u.fail();
  // else
  //   u.pass();

  Util u;
  Real x, a, b, c, d, e;

  u = new Util();

  a = 1.0;
  b = 2.0;
  c = 3.0;
  d = 4.0;
  e = 5.0;

  x = 3.0;

  if (x >= a)			// x = 3, a = 1
    u.pass();
  else
    u.fail();

  if (x <= a)			// x = 3, a = 1
    u.fail();
  else
    u.pass();

  if (x <= x)			// x = 3, x = 3
    u.pass();
  else
    u.fail();

  if (x >= x)			// x = 3, x = 3
    u.pass();
  else
    u.fail();

  if (x <= e)			// x = 3, e = 5;
    u.pass();
  else
    u.fail();

  if (x >= e)			// x = 3, e = 5
    u.fail();
  else
    u.pass();  
}
