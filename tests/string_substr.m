Integer main() {
  String a;

  a = "0123456789";

  out "expect 0123456789" + newline;
  out a.substr(0, 9) + newline;

  out "expect 0" + newline;
  out a.substr(0, 0) + newline;

  out "expect 01" + newline;
  out a.substr(0, 1) + newline;

  out "expect 345" + newline;
  out a.substr(3, 5) + newline;

  out "expect 9" + newline;
  out a.substr(9, 9) + newline;

  out "expect 89" + newline;
  out a.substr(8, 9) + newline;
}
