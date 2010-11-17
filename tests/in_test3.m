Integer main() {
  String s;

  while (1) {
    s = in;
    if (s == null) {
      out "breaking" + newline;
      break;
    }
    out s + newline;
  }

  return 0;
}
