class A {
}

class B extends A {
}

Integer main() {
    A a, aa;
    B b;

    a = new A();

    aa = a;

    if (aa == a)
       out 1.toString() + newline;
    else
       out 0.toString() + newline;

    return 0;
}
