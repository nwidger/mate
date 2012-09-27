class Clause {
	Object mutex;
	
	Integer size;
	Table literals;

	Clause() {
		this(10);
	}
	
	Clause(Integer size) {
		this.mutex = new Object();

		this.size = size;
		this.literals = new Table(this.size);
	}

	Integer abs(Integer a) {
		if (a < 0) return -a;
		return a;
	}

	Clause assignLiteral(State state, Integer literal, Integer value) {
		Clause nc;
		Integer i, j, literal, size;

		nc = this;

		for (i = 0; i < this.size; i = i + 1) {
			literal = (Integer)this.literals.get(i);

			if (!literal.equals(1) && !literals.equals(-1))
				continue;

			if ((value.equals(1) && literal > 0) ||
			    (value.equals(0) && literal < 0)) {
				// remove clause
				nc = null;

				for (i = 0; i < this.size; i = i + 1) {
					state.decrementLiteral(abs((Integer)this.literals.get(i)));
				}
			} else {
				// remove literal
				state.decrementLiteral(abs(literal));

				size = this.size;
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

	}

	State(State state) {
		Integer i;
		
		this.num_clauses = state.num_clauses;
		this.num_variables = state.num_variables;
		this.unit_literal = state.unit_literal;
		this.unit_value = state.unit_value;
		this.next_unassigned = state.next_unassigned;
		this.empty_clause = state.empty_clause;

		this.clauses_size = state.clauses_size;
		this.clauses = new Table(this.clauses_size);

		for (i = 0; i < this.clauses_size; i = i + 1) {
			this.clauses.put(i, state.clauses.get(i));
		}

		this.literals_size = state.literals_size;
		this.literals = new Table(this.literals_size);

		for (i = 0; i < this.literals_size; i = i + 1) {
			this.literals.put(i, state.literals.get(i));
		}
	}

	Object readDimac() {
		String str;
		Clause clause;
		Integer i, n, literal, count;

		count = 0;

		while ((str = in) != null) {
			if (str.equals("c")) {
				// comment
			} else if (str.equals("p")) {
				// cnf
				str = in;
				if (str == null) return null;

				// nbvar
				str = in; if (str == null) return null;
				this.num_variables = str.toInteger();

				// nbclauses
				str = in; if (str == null) return null;
				this.num_clauses = str.toInteger();

				this.clauses_size = this.num_clauses;
				this.clauses = new Table(this.num_clauses);

				this.literals_size = this.num_variables + 1;
				this.literals = new Table(this.literals_size);

				for (i = 0; i < this.literals_size; i = i + 1) {
					this.literals.put(i, 0);
				}
			} else {
				clause = new Clause(this.num_variables);

				clause.literals.put(0, str.toInteger());
				
				for (n = 1; (str = in) != null && !str.equals("0"); n = n + 1) {
					clause.literals.put(n, str.toInteger());
				}

				clause.size = n;

				this.clauses.put(count, clause);
				count = count + 1;
			}
		}

		this.unit_literal = -1;
		this.unit_value = 0;
		this.empty_clause = 0;

		clause = (Clause)this.clauses.get(0);

		if (clause.size.equals(0)) {
			this.empty_clause = 1;
		} else if (clause.size.equals(1)) {
			this.unit_literal = (Integer)clause.literals.get(0);
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

		return null;
	}

	Integer abs(Integer a) {
		if (a < 0) return -a;
		return a;
	}

	Object incrementLiteral(Integer literal) {
		this.literals.put(literal, ((Integer)this.literals.get(literal)) + 1);
		return null;
	}
	
	Object decrementLiteral(Integer literal) {
		this.literals.put(literal, ((Integer)this.literals.get(literal)) - 1);
		return null;
	}

	State assignLiteral(Integer literal, Integer value) {
		Clause clause;
		Integer i, literal, size;
		
		this.unit_literal = -1;
		this.unit_value = 0;
		this.empty_clause = 0;

		for (i = 0; i < this.clauses_size; i = i + 1) {
			clause = (Clause)this.clauses.get(i);

			if (clause == null || clause.size.equals(0))
				continue;

			clause = clause.assignLiteral(this, literal, value);
			this.clauses.put(i, clause);

			if (clause == null) {
				this.num_clauses = this.num_clauses + 1;
				continue;
			}

			size = clause.size;

			if (size.equals(0)) {
				this.empty_clause = 1;
			} else if (size.equals(1)) {
				this.unit_literal = (Integer)clause.literals.get(0);
				if (this.unit_literal < 0) this.unit_value = 0;
				else this.unit_value = 1;
				this.unit_literal = abs(this.unit_literal);
			}
		}

		for (i = 1; i < this.literals_size; i = i + 1) {
			literal = (Integer)this.literals.get(i);
			if (literal > 0) {
				this.next_unassigned = i;
				break;
			}
		}

		return this;
	}
}

class Node {
	Object mutex;
	
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
		if (this.state != null)
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

	Integer empty() {
		return this.head == null;
	}

	Object addFirst(QueueRecord record) {
		synchronized(this.mutex) {
			if (this.head == null) {
				this.head = this.tail = record;
			} else {
				record.next = this.head;
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
				this.head = this.head.next;
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

class RunQueue {
	Integer size;
	Table levels;

	RunQueue(Integer size) {
		Integer i;
		
		this.size = size;
		this.levels = new Table(this.size);

		for (i = 0; i < this.size; i = i + 1) {
			this.levels.put(i, new QueueList());
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
	Node root;
	Object print_mutex;
	
	DPL() {
		root = null;
		print_mutex = new Object();
	}

	Object printSolution(Node child) {
		Integer i;
		Integer value, num_variables, num_clauses;
		Table values;
		State state;

		synchronized(this.print_mutex) {
			state = child.state;
			num_variables = state.num_variables;
			num_clauses = state.clauses_size;

			values = new Table(num_variables);

			for (i = 0; i < num_variables; i = i + 1) {
				values.put(i, 0);
			}

			out "s cnf 1 " + num_variables.toString() + " " + num_clauses.toString() + newline;
			out "t cnf 1 " + num_variables.toString() + " " + num_clauses.toString() + newline;

			i = 0;
			while (child.parent != null) {
				value = child.literal;
				if (child.value.equals(0))
					value = -value;
				values.put(i, value);
				i = i + 1;
				child = child.parent;
			}

			for (i = 0; i < num_variables; i = i + 1) {
				if (((Integer)values.get(i)).equals(0))
					values.put(i, i);
			}

			for (i = 0; i < num_variables; i = i + 1) {
				if (((Integer)values.get(i)).equals(0))
					continue;
				out "v " + ((Integer)values.get(i)).toString() + newline;
			}
		}
		
		return null;
	}
	
	Object initialize() {
		State state;
		Integer num_variables;

		state = new State();
		state.readDimac();

		num_variables = state.num_variables;

		this.root = new Node(null, 0, 0);

		this.root.state = state;
		
		return null;
	}

	Node simplify(Node node, Integer literal, Integer value) {
		Node child;

		child = new Node(node, literal, value);

		return child.simplifyState();
	}

	Node dpl(Node node) {
		Integer unit_literal, unit_value, next_unassigned,
			empty_clause, num_clauses, num_variables;
		Node retval, child1, child2;
		State state;

		retval = null;
		state = node.state;

		num_clauses = state.num_clauses;
		num_variables = state.num_variables;
		unit_literal = state.unit_literal;
		unit_value = state.unit_value;
		next_unassigned = state.next_unassigned;
		empty_clause = state.empty_clause;

		if (!empty_clause.equals(0)) {
			// empty clause, return false
		} else if (num_clauses.equals(0)) {
			// no clauses, return true
			this.printSolution(node);
			return null;
		} else if (!unit_literal.equals(-1)) {
			// unit clause, simplify
			child1 = this.simplify(node, unit_literal, unit_value);
			retval = this.dpl(child1);
		} else if (next_unassigned <= num_variables) {
			// assign next unassigned literal
			child1 = new Node(node, next_unassigned, 0);
			child2 = new Node(node, next_unassigned, 1);

			if (retval == null)
				retval = this.dpl(child1.simplifyState());
		}

		return retval;
	}

	Integer _main() {
		Integer retval;

		retval = 0;
		initialize();

		this.dpl(this.root);
		
		return retval;
	}
}

Integer main() {
	DPL dpl;

	dpl = new DPL();
	
	return dpl._main();
}
