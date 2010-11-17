Integer main()
{
    B b;
    b = new B();
}

class A
{
    A()
    {
        Integer a;
        out "The MVM complains about the local variable declared above when creating a B in main block";
        out newline;
    }
}

class B extends A
{
}
