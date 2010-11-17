class A {
  Integer a;
}
class B extends A {
  String a, b;
}
class C extends B {
  Integer a, b, c;
}
class D extends C {
  String a, b, c, d;
}

Integer main() {
  Object o;
  o = new D();

  ((D)o).a = "9";
  ((D)o).b = "8";
  ((D)o).c = "7";
  ((D)o).d = "6";

  ((C)o).a = 5;
  ((C)o).b = 4;
  ((C)o).c = 3;

  ((B)o).a = "2";
  ((B)o).b = "1";

  ((A)o).a = 0;

  out ((D)o).a;
  out ((D)o).b;
  out ((D)o).c;
  out ((D)o).d;

  out ((C)o).a;
  out ((C)o).b;
  out ((C)o).c;

  out ((B)o).a;
  out ((B)o).b;

  out ((A)o).a;

  out newline;
}
