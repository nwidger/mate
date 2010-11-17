class A {
  // Returns sum of 0..n, right?
  Integer f(Integer n) {
    if(n < 1)
      return n;
    else
      return n + f(n - 1);
  }

  // Hint: What is this?
  Object hint(Integer n) {
    out "Hint A: this is a " + this.toString() + newline;
  }

  String toString() {
    return "A";
  }
}

class B extends A {
  // Returns ___ ?
  Integer f(Integer n) {
    if(n < 1)
      return super.f(n);
    else
      return super.f(n) + f(n - 1);
  }

  // Hint: what does super.f(n) return?
  Object hint(Integer n) {
    super.hint(n);
    out "Hint B: super.f(" + n.toString() + ") is "
      + super.f(n).toString() + newline;
  }

  String toString() {
    return "B";
  }
}

class C extends B {
  // Returns ___ ?
  Integer f(Integer n) {
    if(n < 1)
      return super.f(n);
    else
      return super.f(n) + f(n - 1);
  }

  // Hint: what does super.f(n) return?
  Object hint(Integer n) {
    super.hint(n);
    out "Hint C: super.f(" + n.toString() + ") is "
      + super.f(n).toString() + newline;
  }

  String toString() {
    return "C";
  }
}

Integer main() {
  
  Integer hints;
  hints = 0;

  Integer n;
  A a;

  n = 5;

  a = new A();
  if(hints) {
    a.hint(n);
  }
  out "A.f(" + n.toString() + ") is "
    + a.f(n).toString() + newline;
  
  a = new B();
  if(hints) {
    out newline;
    a.hint(n);
  }
  out "B.f(" + n.toString() + ") is "
    + a.f(n).toString() + newline;

  a = new C();
  if(hints) {
    out newline;
    a.hint(n);
  }
  out "C.f(" + n.toString() + ") is "
    + a.f(n).toString() + newline;
}
