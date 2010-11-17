// test 70% level of Phase 4

class A {
  A() { out 41.toString() + newline; }
  Integer meth() { return 42; }
  Integer meth2() { return 1; }
}

class B extends A {
  Integer meth() { return 44; }
  Integer meth2() { return 2; }
  Integer meth3() { return super.meth2(); }
}

class C extends B {
  C() { super(); out 45.toString() + newline; }
  Integer meth2() { return 3; }
}

class D {
  Integer meth1() { return this.meth2() + 1; }
  Integer meth2() { return meth3() + 1; }
  Integer meth3() { return 1066; }
}


Integer main()
{
  A a;

  a = new A();

  out a.meth().toString() + newline;

  a = new B();

  out a.meth().toString() + newline;

  a = new C();

  out a.meth().toString() + newline;

  C c;

  c = new C();

  c.meth3();

  D d;

  d = new D();

  out d.meth1().toString() + newline;
}

