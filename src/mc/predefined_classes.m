class Object {
	native Object();
	native Integer equals(Object obj);
	native Integer hashCode();
	native String toString();
        native Object notify();
        native Object notifyAll();
        native Object wait();
        native Object wait(Integer timeout);	
}

class Integer extends Object {
	native Integer();
	native Integer(Integer i);
	native Integer(Real r);
	native Integer add(Integer i);
	native Integer subtract(Integer i);
	native Integer multiply(Integer i);
	native Integer divide(Integer i);
	native Integer greaterThan(Integer i);
	native Integer lessThan(Integer i);
	native Integer greaterThanEqual(Integer i);
	native Integer lessThanEqual(Integer i);
	native Integer not();
	native Integer minus();
	native Integer operator + (Integer i);
	native Integer operator - (Integer i);
	native Integer operator * (Integer i);
	native Integer operator / (Integer i);
	native Integer operator > (Integer i);
	native Integer operator < (Integer i);
	native Integer operator >= (Integer i);  
	native Integer operator <= (Integer i);  
	native Integer operator ! ();
	native Integer operator - ();
	native Integer equals(Object obj);
	native Integer hashCode();
	native String toString();
	native Integer mod(Integer i);
	native Integer rand();
}

class String extends Object {
	native String(String s);
	native Integer length();
	native String substr(Integer beg, Integer end);
	native Integer toInteger();
	native Real toReal();
	native String concat(String s);
	native String operator + (String s);
	native Integer operator > (String s);
	native Integer operator < (String s);
	native Integer hashCode();
	native Integer equals(Object obj);
	native String toString();
}

class Table extends Object {
        Object lock;
        Integer num_entries;
        Real load_factor;
        Integer current_capacity;
        Integer iterator_is_running;
        Integer iterator_bucket;
        Integer iterator_entry;
        Object buckets;

	native Table();
	native Table(Integer n);
	native Object get(Object key);
	native Object put(Object key, Object value);
	native Object remove(Object key);
	native Integer firstKey();
	native Object nextKey();
}

class Thread extends Object {
        native Thread();
        native Object start();
        native Object run();
        native Object join();
        native Object sleep(Integer millisec);  
}

class Real extends Object {
	native Real();
	native Real(Real r);
	native Real(Integer i);
	native Real add(Real r);
	native Real subtract(Real r);
	native Real multiply(Real r);
	native Real divide(Real r);
	native Real greaterThan(Real r);
	native Real lessThan(Real r);
	native Real greaterThanEqual(Real r);
	native Real lessThanEqual(Real r);
	native Integer not();
	native Real minus();
	native Real operator + (Real r);
	native Real operator - (Real r);
	native Real operator * (Real r);
	native Real operator / (Real r);
	native Integer operator > (Real r);
	native Integer operator < (Real r);
	native Integer operator >= (Real r);  
	native Integer operator <= (Real r);  
	native Integer operator ! ();
	native Real operator - ();
	native Integer equals(Object obj);
	native Integer hashCode();
	native String toString();
	native Real squareRoot();
}
