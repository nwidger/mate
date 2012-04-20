Integer main() {
  Real r, s;
  Integer i, j;

  i = 4;
  r = new Real(i);

  out "r == " + r.toString() + newline;

  j = new Integer(r);

  out "j == " + i.toString() + newline;

  s = r.squareRoot();

  out "s == " + s.toString() + newline;

  return 0;
}
  
  
