class Object {
	native:0 Object();
	native:1 Integer equals(Object obj);
	native:2 Integer hashCode();
	native:3 String toString();
}

class Integer extends Object {
	native:4 Integer();
	native:5 Integer(Integer i);
	native:6 Integer add(Integer i);
	native:7 Integer subtract(Integer i);
	native:8 Integer multiply(Integer i);
	native:9 Integer divide(Integer i);
	native:10 Integer greaterThan(Integer i);
	native:11 Integer lessThan(Integer i);
	native:12 Integer not();
	native:13 Integer minus();
	native:14 Integer operator + (Integer i);
	native:15 Integer operator - (Integer i);
	native:16 Integer operator * (Integer i);
	native:17 Integer operator / (Integer i);
	native:18 Integer operator > (Integer i);
	native:19 Integer operator < (Integer i);
	native:20 Integer operator ! ();
	native:21 Integer operator - ();
	native:22 Integer equals(Object obj);
	native:23 Integer hashCode();
	native:24 String toString();
}

class String extends Object {
	native:25 String(String s);
	native:26 Integer length();
	native:27 String substr(Integer beg, Integer end);
	native:28 Integer toInteger();
	native:29 String concat(String s);
	native:30 String operator + (String s);
	native:31 Integer operator > (String s);
	native:32 Integer operator < (String s);
	native:33 Integer hashCode();
	native:34 Integer equals(Object obj);
	native:35 String toString();
}

class Table extends Object {
	native:36 Table();
	native:37 Table(Integer n);
	native:38 Object get(Object key);
	native:39 Object put(Object key, Object value);
	native:40 Object remove(Object key);
	native:41 Integer firstKey();
	native:42 Object nextKey();
}
