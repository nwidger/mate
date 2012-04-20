class Object {
	native:0 Object();
	native:1 Integer equals(Object obj);
	native:2 Integer hashCode();
	native:3 String toString();
        native:4 Object notify();
        native:5 Object notifyAll();
        native:6 Object wait();
        native:7 Object wait(Integer timeout);	
}

class Integer extends Object {
	native:8 Integer();
	native:9 Integer(Integer i);
	native:10 Integer(Real r);
	native:11 Integer add(Integer i);
	native:12 Integer subtract(Integer i);
	native:13 Integer multiply(Integer i);
	native:14 Integer divide(Integer i);
	native:15 Integer greaterThan(Integer i);
	native:16 Integer lessThan(Integer i);
	native:17 Integer greaterThanEqual(Integer i);
	native:18 Integer lessThanEqual(Integer i);
	native:19 Integer not();
	native:20 Integer minus();
	native:21 Integer operator + (Integer i);
	native:22 Integer operator - (Integer i);
	native:23 Integer operator * (Integer i);
	native:24 Integer operator / (Integer i);
	native:25 Integer operator > (Integer i);
	native:26 Integer operator < (Integer i);
	native:27 Integer operator >= (Integer i);  
	native:28 Integer operator <= (Integer i);  
	native:29 Integer operator ! ();
	native:30 Integer operator - ();
	native:31 Integer equals(Object obj);
	native:32 Integer hashCode();
	native:33 String toString();
}

class String extends Object {
	native:34 String(String s);
	native:35 Integer length();
	native:36 String substr(Integer beg, Integer end);
	native:37 Integer toInteger();
	native:38 String concat(String s);
	native:39 String operator + (String s);
	native:40 Integer operator > (String s);
	native:41 Integer operator < (String s);
	native:42 Integer hashCode();
	native:43 Integer equals(Object obj);
	native:44 String toString();
}

class Table extends Object {
	native:45 Table();
	native:46 Table(Integer n);
	native:47 Object get(Object key);
	native:48 Object put(Object key, Object value);
	native:49 Object remove(Object key);
	native:50 Integer firstKey();
	native:51 Object nextKey();
}

class Thread extends Object {
        native:52 Thread();
        native:53 Object start();
        native:54 Object run();
        native:55 Object join();
        native:56 Object sleep(Integer millisec);  
}

class Real extends Object {
	native:57 Real();
	native:58 Real(Real r);
	native:59 Real(Integer i);
	native:60 Real add(Real r);
	native:61 Real subtract(Real r);
	native:62 Real multiply(Real r);
	native:63 Real divide(Real r);
	native:64 Real greaterThan(Real r);
	native:65 Real lessThan(Real r);
	native:66 Real greaterThanEqual(Real r);
	native:67 Real lessThanEqual(Real r);
	native:68 Integer not();
	native:69 Real minus();
	native:70 Real operator + (Real r);
	native:71 Real operator - (Real r);
	native:72 Real operator * (Real r);
	native:73 Real operator / (Real r);
	native:74 Integer operator > (Real r);
	native:75 Integer operator < (Real r);
	native:76 Integer operator >= (Real r);  
	native:77 Integer operator <= (Real r);  
	native:78 Integer operator ! ();
	native:79 Real operator - ();
	native:80 Integer equals(Object obj);
	native:81 Integer hashCode();
	native:82 String toString();
	native:83 Real squareRoot();
}
