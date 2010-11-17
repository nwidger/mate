// test 80% level of Phase 4

class AA {
  Integer f(Integer a, Integer b, Integer c)
  {
    out a.toString() + newline;
    out b.toString() + newline;
    out c.toString() + newline;
    return 0;
  }
  Integer f1() { out 1.toString() + newline; return 1; }
  Integer f2() { out 2.toString() + newline; return 2; }
  Integer f3() { out 3.toString() + newline; return 3; }
}

class A {
  A(Integer x) { out x.toString() + newline; }
} 

class B {
  B(Integer x, Integer y) { out (x * 10 + y).toString() + newline; }
}   
      
class C {
  C(Integer x, Integer y, Integer z) { out (x * 5 + y * 2 + z).toString() + newline; }
} 
      
Integer main()
{
  AA aa;

  aa = new AA();

  aa.f(aa.f1(), aa.f2(), aa.f3());

  A a;
  B b;
  C c;

  a = new A(42);

  b = new B(4, 2);

  c = new C(5, 8, 1);
}

