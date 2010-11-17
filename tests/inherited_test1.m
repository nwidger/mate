class A {
  Integer a;
}
class B extends A {
  String a, b;
}
class C extends B {
  Integer a, b, c;
}

Integer main() {
  Object o;
  o = new C();

  ((C)o).a = 5;
  ((C)o).b = 4;
  ((C)o).c = 3;

  ((B)o).a = "2";
  ((B)o).b = "1";

  ((A)o).a = 0;

  out ((C)o).a;
  out ((C)o).b;
  out ((C)o).c;

  out ((B)o).a;
  out ((B)o).b;

  out ((A)o).a;

  out newline;
}
