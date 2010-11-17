class A {
  Integer f;
  A(Integer x) {
    f = x;
  }
}

Integer main() {
  Integer x;
  A a;
  a = new A(5);

  x = a.f = 99;

  return 0;
}
