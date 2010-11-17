class A {
    Integer i;
}

class B extends A {
    Integer i;
    Integer j;
}

Integer main() {
    A a;
    B b;

    a = new A();

    b = (B)a;

    return 0;
}
