class Real extends Float {
}

class Int {
    Integer var;

    Int() {
        var = 6;
    }

    Int(Float a) {
        var = 5;
    }

    Float func(Float a, Real b) {
        return c;
    }

    Float func(Real a, Float b) {
        return new Float();
    }

    Float func(Float a, Float b) {
    	return new Float();
    }
} 

class Float extends Int {
    Integer var2;

    Float() {
    }

    Float(Integer a) {
        var2 = a;
    }
    
    Float func(Float a, Real b) {
        return new Float();
    }

    Float func(Real a, Float b) {
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
        return a + b + c + d;
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

    f = new Float(999);;

    out (f.var2).toString() + newline;

    abc = f.test(1,2,3);

    abc = f.test(1);

    abc = f.test(1,2);

    abc = f.test(1,2,3,4,5);

    return abc;
}
