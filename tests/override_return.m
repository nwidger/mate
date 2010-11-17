class A extends Integer {
A(Integer a) {super(a);}
Integer toString() { return 999; }
String realString() { return super.toString(); }
}

Integer main() {
A a;
Integer i;
String s;

a = new A(1);
i = a;

// Add 2 Integers, out converts to String
out (a.toString() + 2);
out newline;

// Use the superclass toString()
out a.realString();
out newline;

// fool the compiler
out ((A)i).toString();
out newline;

// The compiler allows this although runtime fails
s = i.toString();

// same as above
// this fails at runtime because out converts "i"
// via toString which is still an Integer
out i;
out newline;
}
