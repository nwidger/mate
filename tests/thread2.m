class Worker extends Thread {
	String token;
	String str;

	Worker(String token, String str) {
		this.token = token;
		this.str = str;
	}

	Object run() {
		String c;
		Integer i, length;

		i = 0;
		length = str.length();

		while (i < length) {
			c = str.substr(i, i);

			if (c.equals(token)) {
				out "string " + str + " contains " + token + " at index " +
					i.toString() + "!" + newline;
				return null;
			}

			i = i + 1;
		}
	
		out "string " + str + " does not contain " + token + newline;
		return null;
	}
}

Integer main() {
	Table t;
	Worker w;
	Integer i, key;
	String token, str;

	i = 0;
	t = new Table();

	token = in;

	if (token == null) {
		out "error: please specify token" + newline;
		return 1;
	}

	while (!((str = in) == null)) {
		w = new Worker(token, str);
		t.put(i, w);
    
		w.start();
    
		i = i + 1;
	}

	if (t.firstKey().equals(0)) {
		key = (Integer)t.nextKey();
		i = 0;

		while (!(key == null)) {
			w = (Worker)t.get(key);
	    
			w.join();

			key = (Integer)t.nextKey();
			i = i + 1;
		}
	}

	return 0;
}
