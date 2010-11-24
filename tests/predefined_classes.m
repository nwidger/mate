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
	native:16 Integer not();
	native:17 Integer minus();
	native:18 Integer operator + (Integer i);
	native:19 Integer operator - (Integer i);
	native:20 Integer operator * (Integer i);
	native:21 Integer operator / (Integer i);
	native:22 Integer operator > (Integer i);
	native:23 Integer operator < (Integer i);
	native:24 Integer operator ! ();
	native:25 Integer operator - ();
	native:26 Integer equals(Object obj);
	native:27 Integer hashCode();
	native:28 String toString();
}

class String extends Object {
	native:29 String(String s);
	native:30 Integer length();
	native:31 String substr(Integer beg, Integer end);
	native:32 Integer toInteger();
	native:33 String concat(String s);
	native:34 String operator + (String s);
	native:35 Integer operator > (String s);
	native:36 Integer operator < (String s);
	native:37 Integer hashCode();
	native:38 Integer equals(Object obj);
	native:39 String toString();
}

class Table extends Object {
	native:40 Table();
	native:41 Table(Integer n);
	native:42 Object get(Object key);
	native:43 Object put(Object key, Object value);
	native:44 Object remove(Object key);
	native:45 Integer firstKey();
	native:46 Object nextKey();
}

class Thread extends Object {
        native:47 Thread();
        native:48 Object start();
        native:49 Object run();
        native:50 Object join();
}
