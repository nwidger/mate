Integer main() {
  Real r, s;

  r = 4.5;

  out r.toString() + newline;

  s = 5.5;

  out s.toString() + newline;

  if (r < s) {
    out r.toString() + " is less than " + s.toString() + newline;
  } else {
    out "error!" + newline;
  }

  r = r + s;

  out r.toString() + newline;

  return 0;
}
