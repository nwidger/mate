class TableEntry {
	Object key;
	Object value;
	TableEntry next;

	TableEntry() {
		this.key = null;
		this.value = null;
		this.next = null;
	}
}

class Bucket {
	TableEntry entry;
	Bucket next;

	Bucket() {
		this.entry = null;
		this.next = null;
	}
}

class OtherTable {
        Object lock;
        Integer num_entries;
        Real load_factor;
        Integer current_capacity;
        Integer iterator_is_running;
        Integer iterator_bucket;
        TableEntry iterator_entry;
        Bucket buckets;

	OtherTable() {
		this(16);
	}

	OtherTable(Integer n) {
		Integer i;
		Bucket prev, bucket;

		this.lock = new Object();
		this.num_entries = 0;
		this.load_factor = 0.75;
		this.current_capacity = n;
		this.iterator_is_running = 0;
		this.iterator_bucket = 0;
		this.iterator_entry = null;
		this.buckets = new Bucket();

		prev = null;
		for (i = 0; i < n; i = i + 1) {
			bucket = new Bucket();
			if (prev != null) bucket.next = prev;
			prev = bucket;
		}

		this.buckets = bucket;
	}

	Bucket acquireBucket(Integer n) {
		Integer i;
		Bucket bucket;

		i = 0;
		
		for (bucket = this.buckets; bucket != null; bucket = bucket.next) {
			if (i.equals(n))
				return bucket;

			i = i + 1;
		}

		return null;
	}

	Object get(Object key) {
		Object value;
		Integer n, hash;
		TableEntry entry;

		value = null;
		hash = key.hashCode();
		if (hash < 0) hash = -hash;

		n = hash.mod(this.current_capacity);

		synchronized (this.lock) {
			for (entry = this.acquireBucket(n).entry; entry != null; entry = entry.next) {
				if (key.equals(entry.key)) {
					value = entry.value;
					break;
				}
			}
		}

		return value;
	}

	Integer exceedsLoadFactor() {
		return (new Real(this.num_entries) * this.load_factor) > new Real(this.current_capacity);
	}

	Object put(Object key, Object value) {
		Integer hash, n;
		Object old_value;
		TableEntry head, curr, newh;
		
		synchronized (this.lock) {
			if (this.iterator_is_running) {
				out "cannot put, Table's iterator is running!" + newline;
				return null;
			}

			hash = key.hashCode();
			if (hash < 0) hash = -hash;

			if (this.current_capacity <= 0) {
				this.resize(16);
			}

			n = hash.mod(this.current_capacity);

			old_value = null;
			head = this.acquireBucket(n).entry;

			for (curr = head; curr != null; curr = curr.next) {
				if (key.equals(curr.key))
					break;
			}

			if (curr != null) {
				old_value = curr.value;
				curr.key = key;
				curr.value = value;
			} else {
				newh = new TableEntry();

				newh.key = key;
				newh.value = value;
				newh.next = head;

				this.acquireBucket(n).entry = newh;
				this.num_entries = this.num_entries + 1;
			}

			if (this.exceedsLoadFactor()) {
				this.resize(this.current_capacity*2);
			}
		}

		return old_value;
	}
	
	Object remove(Object key) {
		Integer hash, n;
		Object old_value;
		TableEntry head, curr, prev, newh;
		
		synchronized (this.lock) {
			if (this.iterator_is_running) {
				out "cannot remove, Table's iterator is running!" + newline;
				return null;
			}

			hash = key.hashCode();
			if (hash < 0) hash = -hash;

			n = hash.mod(this.current_capacity);

			old_value = null;
			prev = null;
			
			head = this.acquireBucket(n).entry;

			curr = head;
			while (curr != null) {
				if (key.equals(curr.key)) {
					old_value = curr.value;
					break;
				}

				prev = curr;
				curr = curr.next;
			}

			if (curr != null) {
				if (curr == head) {
					this.acquireBucket(n).entry = curr.next;
				} else {
					prev.next = curr.next;
				}
			}

			this.num_entries = this.num_entries - 1;
		}

		return old_value;
	}
	
	Integer firstKey() {
		TableEntry entry;
		Integer value, i;

		synchronized (this.lock) {
			for (i = 0; i < this.current_capacity; i = i + 1) {
				if (this.acquireBucket(i).entry != null)
					break;
			}

			if (i >= this.current_capacity) {
				value = 1;
				this.iterator_is_running = 0;
			} else {
				value = 0;
				this.iterator_is_running = 1;
				this.iterator_bucket = i;
				this.iterator_entry = entry;
			}
		}

		return value;
	}
	
	Object nextKey() {
		Integer n;
		Object prev;
		TableEntry entry;

		synchronized (this.lock) {
			if (!this.iterator_is_running)
				return 0;

			entry = this.iterator_entry;
			prev = entry.key;

			entry = entry.next;
			this.iterator_entry = entry;

			if (entry != null)
				return prev;

			for (n = this.iterator_bucket + 1; n < this.current_capacity; n = n + 1) {
				entry = this.acquireBucket(n).entry;

				if (entry != null) {
					this.iterator_bucket = n;
					this.iterator_entry = entry;
					break;
				}
			}

			if (n >= this.current_capacity)
				this.iterator_is_running = 0;
		}

		return prev;
	}

	Object resize(Integer n) {
		Integer i;
		TableEntry entry;
		OtherTable new_table;
		
		synchronized (this.lock) {
			new_table = new OtherTable(n);

			for (i = 0; i < this.current_capacity; i = i + 1) {
				entry = this.acquireBucket(i).entry;
				new_table.put(entry.key, entry.value);
			}

			this.num_entries = new_table.num_entries;
			this.load_factor = new_table.load_factor;
			this.current_capacity = new_table.current_capacity;
			this.iterator_is_running = new_table.iterator_is_running;
			this.iterator_bucket = new_table.iterator_bucket;
			this.iterator_entry = new_table.iterator_entry;
			this.buckets = new_table.buckets;
		}

		return null;
	}
}
