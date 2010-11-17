// test the 90% level of Phase 4

class A {
  Integer f(Integer i) { return 1; }
  Integer f(A a) { return 2; }
} 
  
class B extends A {
  Integer f(Integer i, Integer j) { return i + j; }
}

Integer main()
{ 
  A a;
  B b;

  a = new A();
  b = new B();

  out (b.f(19) + 41).toString() + newline;
  out (b.f(a) + 40).toString() + newline;
  out (b.f(b) + 40).toString() + newline;
  out (b.f(7, 8) + 27).toString() + newline;
}

