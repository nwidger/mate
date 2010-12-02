class Worker extends Thread {
	Main m;
	String token;
	String str;

	Worker(Main m, String token, String str) {
		this.m = m;
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
				m.println("string " + str + " contains " + token + " at index " +
					  i.toString() + "!");
				return null;
			}

			i = i + 1;
		}

		m.println("string " + str + " does not contain " + token);
		return null;
	}
}

class Main extends Thread {
	Main() {

	}

	Object println(String str) {
		synchronized (this) {
			synchronized (this) {
				out str + newline;
				return null;
			}
		}
	}
	
	Object run() {
		Table t;
		Worker w;
		Integer i, key;
		String token, str;

		token = in;

		if (token == null) {
			out "error: please specify token" + newline;
			return 1;
		}

		i = 0;
		t = new Table();

		while (!((str = in) == null)) {
			w = new Worker(this, token, str);
			t.put(i, w);
			w.start();
			i = i + 1;
		}

		if (t.firstKey().equals(0)) {
			key = (Integer)t.nextKey();

			while (!(key == null)) {
				w = (Worker)t.get(key);
				w.join();
				key = (Integer)t.nextKey();
			}
		}

		return null;
	}
}

Integer main() {
	Main m;

	m = new Main();

	m.start();
	m.join();

	return 0;
}
