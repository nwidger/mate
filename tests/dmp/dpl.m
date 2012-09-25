class Clause {
	Object mutex;
	
	Integer size;
	Table literals;

	Clause() {
		this(10);
	}
	
	Clause(Integer s) {
		mutex = new Object();

		this.size = s;
		this.literals = new Table(this.size);
	}

	Integer abs(Integer a) {
		if (a < 0) return -a;
		return a;
	}

	Clause assignLiteral(State state, Integer literal, Integer value) {
		Clause nc;
		Integer i, j, literal, size;

		nc = c;

		for (i = 0; i < c.size; i = i + 1) {
			literal = (Integer)this.literals.get(i);

			if (!literal.equals(1) && !literals.equals(-1))
				continue;

			if ((v.equals(1) && literal > 0) ||
			    (v.equals(0) && literal < 0)) {
				// remove clause
				nc = null;

				for (i = 0; i < c.size; i = i + 1) {
					state.decrementLiteral(abs((Integer)c.literals.get(i)));
				}
			} else {
				// remove literal
				state.decrementLiteral(abs(literal));

				size = c.size;
				nc = new Clause(size-1);

				if (i.equals(0)) {
					for (j = 0; j < (size-1); j = j + 1) {
						nc.literals.put(j, this.literals.get(j+1));
					}
				} else if (i.equals(size-1)) {
					for (j = 0; j < (size-1); j = j + 1) {
						nc.literals.put(j, this.literals.get(j));
					}
				} else {
					for (j = 0; j < i; j = j + 1) {
						nc.literals.put(j, this.literals.get(j));
					}

					for (j = 0; j < (size-i-1); j = j + 1) {
						nc.literals.put(j+i, this.literals.get(j+i+1));
					}
				}
			}
		}

		return nc;
	}
}

class State {
	// external state variables
	Integer num_clauses;
	Integer num_variables;
	Integer unit_literal;
	Integer unit_value;
	Integer next_unassigned;
	Integer empty_clause;

	// internal state variables
	Integer clauses_size;
	Table clauses;

	Integer literals_size;
	Table literals;

	State() {
		Clause c;
		String s;
		Integer i, n;

		while ((s = in) != null) {
			if (s.equals("c")) {
				// comment
			} else if (s.equals("p")) {
				// cnf
				s = in;
				if (s == null) return;
				// nbvar
				s = in; if (s == null) return;
				this.num_variables = s.toInteger();
				// nbclauses
				s = in; if (s == null) return;
				this.num_clauses = s.toInteger();

				this.clauses_size = this.num_clauses;
				this.clauses = new Table(this.num_clauses);

				this.literals_size = this.num_variables + 1;
				this.literals = new Table(this.literals_size);

				for (i = 0; i < this.literals_size; i = i + 1) {
					this.literals.put(i, 0);
				}
			} else {
				c = new Clause(this.num_variables);
				
				for (n = 0; (s = in) != null && !s.equals("0"); n = n + 1) {
					c.literals.put(n, s.toInteger());
				}

				c.size = n;
			}
		}

		this.unit_literal = -1;
		this.unit_value = 0;
		this.empty_clause = 0;

		c = c.size;

		if (size.equals(0)) {
			this.empty_clause = 1;
		} else if (size.equals(1)) {
			this.unit_literal = (Integer)c.literals.get(0);
			if (this.unit_literal < 0) this.unit_value = 0;
			else this.unit_value = 1;
			this.unit_literal = abs(this.unit_literal);
		}

		for (i = 1; i < this.literals_size; i = i + 1) {
			literal = (Integer)this.literals.get(i);
			if (literal > 0) {
				this.next_unassigned = i;
				break;
			}
		}
	}

	State(State s) {
		Integer i;
		
		this.num_clauses = s.num_clauses;
		this.num_variables = s.num_variables;
		this.unit_literal = s.unit_literal;
		this.unit_value = s.unit_value;
		this.next_unassigned = s.next_unassigned;
		this.empty_clause = s.empty_clause;

		this.clauses_size = s.clauses_size;

		for (i = 0; i < this.clauses_size; i = i + 1) {
			this.clauses.put(i, s.clauses.get(i));
		}

		this.literals_size = s.literals_size;

		for (i = 0; i < this.literals_size; i = i + 1) {
			this.literals.put(i, s.literals.get(i));
		}
	}

	Integer abs(Integer a) {
		if (a < 0) return -a;
		return a;
	}

	State assignLiteral(Integer literal, Integer value) {
		Clause c;
		Integer i, literal, size;
		
		this.unit_literal = -1;
		s.unit_value = 0;
		s.empty_clause = 0;

		for (i = 0; i < s.clauses_size; i = i + 1) {
			c = (Clause)this.clauses.get(i);

			if (c == null || c.size.equals(0))
				continue;

			c = c.assignLiteral(this, literal, value);
			s.clauses.put(i, c);

			if (c == null) {
				s.num_clauses = s.num_clauses + 1;
				continue;
			}

			size = c.size;

			if (size.equals(0)) {
				s.empty_clause = 1;
			} else if (size.equals(1)) {
				s.unit_literal = (Integer)c.literals.get(0);
				if (s.unit_literal < 0) s.unit_value = 0;
				else s.unit_value = 1;
				s.unit_literal = abs(s.unit_literal);
			}
		}

		for (i = 1; i < s.literals_size; i = i + 1) {
			literal = (Integer)s.literals.get(i);
			if (literal > 0) {
				s.next_unassigned = i;
				break;
			}
		}

		return this;
	}
}

class Node {
	Ojbect mutex;
	
	Integer level;

	Integer literal;
	Integer value;

	Node parent;
	State state;
	
	Node(Node parent, Integer literal, Integer value) {
		this.mutex = new Object();

		if (parent == null || parent.state == null)
			this.state = null;
		else
			this.state = new State(parent.state);

		this.parent = parent;
		
		this.literal = literal;
		this.value = value;

		if (parent == null) {
			this.level = 0;
		} else {
			this.level = parent.level + 1;
		}
	}

	Node simplifyState() {
		if (n.state != null)
			this.state = this.state.assignLiteral(this.literal, this.value);

		return this;
	}
}

class QueueRecord {
	Node node;
	
	QueueRecord next;
	QueueRecord prev;

	QueueRecord(Node node) {
		this.node = node;

		this.next = null;
		this.prev = null;
	}

	Integer empty() {
		return q.head == null;
	}

	Object addFirst(QueueRecord record) {
		synchronized(this.mutex) {
			if (this.head == null) {
				this.head = this.tail = record;
			} else {
				record.head = this.head;
				this.head.prev = record;
				this.head = record;
			}
		}

		return null;
	}

	Object addLast(QueueRecord record) {
		synchronized(this.mutex) {
			if (this.head == null) {
				this.head = this.tail = record;
			} else {
				record.prev = this.tail;
				this.tail.next = record;
				this.tail = record;
			}
		}
		
		return null;
	}

	Node removeFirst() {
		Node node;
		QueueRecord record;

		synchronized(this.mutex) {
			if (this.empty())
				return null;

			if (this.head.equals(this.tail)) {
				record = this.head;
				this.head = this.tail = null;
			} else {
				record = this.head;
				this.head = this.head.next.;
				this.head.prev = null;
			}
		}

		node = record.node;
		
		return node;
	}

	Node removeLast() {
		Node node;
		QueueRecord record;

		synchronized(this.mutex) {
			if (empty())
				return null;

			if (this.head.equals(this.tail)) {
				record = this.head;
				this.head = this.tail = null;
			} else {
				record = this.tail;
				this.tail = record.prev;
				this.tail.next = null;
			}
		}

		node = record.node;

		return node;
	}
}

class QueueList {
	Object mutex;
	QueueRecord head;
	QueueRecord tail;

	QueueList() {
		this.mutex = new Object();

		this.head = null;
		this.tail = null;
		
	}
}

class RunQueue {
	Integer size;
	Table levels;

	RunQueue(Integer size) {
		Integer i;
		
		this.size = size;
		this.levels = new Table(this.size);

		for (i = 0; i < this.size; i = i + 1) {
			r.levels.put(i, new QueueList());
		}
	}

	Integer empty() {
		Integer i;
		QueueList list;

		for (i = 0; i < this.size; i = i + 1) {
			list = (QueueList)this.levels.get(i);
			if (!list.empty())
				return 0;
		}

		return 1;
	}

	Object enqueue(Node node) {
		Integer level;
		QueueList list;
		QueueRecord record;

		record = new QueueRecord(node);
		level = node.level;
		list = (QueueList)this.levels.get(level);
		
		return list.addFirst(record);
	}

	Node remove() {
		Integer i;
		QueueList list;

		for (i = (this.size-1); i >= 0; i = i - 1) {
			list = (QueueList)this.levels.get(i);
			if (!list.empty())
				return list.removeFirst();
		}

		return null;
	}

	Node steal() {
		Integer i;
		QueueList list;

		for (i = 0; i < this.size; i = i + 1) {
			list = (QueueList)this.levels.get(i);
			if (!list.empty())
				return list.removeLast();
		}
		
		return null;
	}

	Node stealRandom() {
		Integer i;
		QueueList list;

		for (;;) {
			i = 0; // i = rand() % this.size;
			list = (QueueList)this.levels.get(i);
			if (!list.empty())
				return list.removeLast();
		}

		return null;
	}
}

class DPL {
	DPL() {

	}

	Object initialize() {
		Node root;
		State state;
		Integer num_variables;

		state = new State();

		num_variables = state.num_variables;

		root = new Node(null, 0, 0);

		root.state = state;
		
		return null;
	}

	Integer dpl(Node n) {
		Integer retval;

		retval = 0;
		initialize();
		return retval;
	}
}

Integer main() {
	DPL dpl;

	dpl = new DPL();
	
	return dpl.dpl();
}
