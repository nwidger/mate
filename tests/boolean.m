Integer main() {
  Integer a, b, c;

  a = 1;
  b = 2;

  if (a == a && b == b && c == c) {
    out "yay!" + newline;
  } else {
    out "boo!" + newline;
  }

  if (a == b || b == b || c == a) {
    out "yay!" + newline;
  } else {
    out "boo!" + newline;    
  }

  if (a == a && (a == b || b == b)) {
    out "yay!" + newline;    
  } else {
    out "boo!" + newline;    
  }

  if (a.greaterThan(b) && b.greaterThan(a)) {
    out "boo!" + newline;
  } else {
    out "yay!" + newline;
  }
}
