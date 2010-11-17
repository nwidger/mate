class Real extends Float {
}


class Float extends Int {
    Integer var2;

    Float() {
        super.var2 = 42;
	var2 = 24;
    }

    Float(Integer a) {
        var2 = a;
    }

    Float func(Float a, Real b) {
    	func(a,b);
        return new Float();
    }

    Float func(Real a, Float b) {
    	out (b.var2).toString() + newline;
        return new Float();
    }

    Integer test(Integer a, Integer b, Integer c) {
    	out a.toString() + newline;
	out b.toString() + newline;
	out c.toString() + newline;
	return a + b + c;
    }

    Integer test(Integer a, Integer b, Integer c, Integer d) {
    	out a.toString() + newline;
	out b.toString() + newline;
	out c.toString() + newline;
	out d.toString() + newline;
	super.test(b);
	return new Integer(b);
    }

    Integer test(Integer a) {
    	out a.toString() + newline;
	return a;
    }

    Integer test(Integer a, Integer b) {
    	out a.toString() + newline;
	out b.toString() + newline;
	return a + b;
    }
}

Integer main() {
    Integer abc;
    Integer def;
    Float f;
    Real r;

    f = new Float(999);

    out f.var2.toString() + newline;

    abc = f.test(1,2,3);

    abc = f.test(1);

    abc = f.test(1,2);

    def = 2;

    f.test(def,2,3,4);

    if (def.equals(def))
       out 1.toString() + newline;
    else
       out 0.toString() + newline;

    f.var2 = 919;
    f.func(r,f);

    return abc;
}

class Int {
    Integer var;
    Integer var2;

    Int() {
    	super();
        var = 6;
    }

    Int(Float a) {
        var = 5;
    }

    Integer func(Float a, Real b) {
    	out 2.toString() + newline;
        return 2;
    }

    Float func(Real a, Float b) {
        return new Float();
    }

    Float func(Float a, Float b) {
    	return new Float();
    }

    Integer test(Integer a) {
        out 4242.toString();
        return a;
    }
} 
