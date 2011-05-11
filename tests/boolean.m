Integer main() {
  Integer a, b, c;

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
}
