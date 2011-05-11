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
	native:10 Integer add(Integer i);
	native:11 Integer subtract(Integer i);
	native:12 Integer multiply(Integer i);
	native:13 Integer divide(Integer i);
	native:14 Integer greaterThan(Integer i);
	native:15 Integer lessThan(Integer i);
	native:16 Integer greaterThanEqual(Integer i);
	native:17 Integer lessThanEqual(Integer i);
	native:18 Integer not();
	native:19 Integer minus();
	native:20 Integer operator + (Integer i);
	native:21 Integer operator - (Integer i);
	native:22 Integer operator * (Integer i);
	native:23 Integer operator / (Integer i);
	native:24 Integer operator > (Integer i);
	native:25 Integer operator < (Integer i);
	native:26 Integer operator >= (Integer i);  
	native:27 Integer operator <= (Integer i);  
	native:28 Integer operator ! ();
	native:29 Integer operator - ();
	native:30 Integer equals(Object obj);
	native:31 Integer hashCode();
	native:32 String toString();
}

class String extends Object {
	native:33 String(String s);
	native:34 Integer length();
	native:35 String substr(Integer beg, Integer end);
	native:36 Integer toInteger();
	native:37 String concat(String s);
	native:38 String operator + (String s);
	native:39 Integer operator > (String s);
	native:40 Integer operator < (String s);
	native:41 Integer hashCode();
	native:42 Integer equals(Object obj);
	native:43 String toString();
}

class Table extends Object {
	native:44 Table();
	native:45 Table(Integer n);
	native:46 Object get(Object key);
	native:47 Object put(Object key, Object value);
	native:48 Object remove(Object key);
	native:49 Integer firstKey();
	native:50 Object nextKey();
}

class Thread extends Object {
        native:51 Thread();
        native:52 Object start();
        native:53 Object run();
        native:54 Object join();
        native:55 Object sleep(Integer millisec);  
}

class Real extends Object {
	native:56 Real();
	native:57 Real(Real r);
	native:58 Real add(Real r);
	native:59 Real subtract(Real r);
	native:60 Real multiply(Real r);
	native:61 Real divide(Real r);
	native:62 Real greaterThan(Real r);
	native:63 Real lessThan(Real r);
	native:64 Real greaterThanEqual(Real r);
	native:65 Real lessThanEqual(Real r);
	native:66 Integer not();
	native:67 Real minus();
	native:68 Real operator + (Real r);
	native:69 Real operator - (Real r);
	native:70 Real operator * (Real r);
	native:71 Real operator / (Real r);
	native:72 Integer operator > (Real r);
	native:73 Integer operator < (Real r);
	native:74 Integer operator >= (Real r);  
	native:75 Integer operator <= (Real r);  
	native:76 Integer operator ! ();
	native:77 Real operator - ();
	native:78 Integer equals(Object obj);
	native:79 Integer hashCode();
	native:80 String toString();
}
