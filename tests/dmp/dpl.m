class Clause {
	Object mutex;
	
	Integer size;
	Table literals;

	Integer removed;

	Clause() {
		this.removed = 0;
	}
	
	Clause(Integer size) {
		this.mutex = new Object();

		this.size = size;

		if (!this.size.equals(0))
			this.literals = new Table(this.size);

		this.removed = 0;
	}

	String toString() {
		Integer i;
		String str;
		
		if (this.removed) {
			str = "[ removed ]";
		} else {
			str = "[";
			
			for (i = 0; i < this.size; i = i + 1) {
				str = str + " " + ((Integer)this.literals.get(i)).toString() + " ";
			}
			
			str = str + "]";
		}

		return str;
	}
	
	Integer abs(Integer a) {
		if (a < 0) return -a;
		return a;
	}

	Clause assignLiteral(State state, Integer literal, Integer value) {
		Clause nc;
		Integer i, j, lit, size;

		// out "in assignLiteral, clause = " + this.toString() + newline;

		nc = this;

		for (i = 0; i < this.size; i = i + 1) {
			lit = (Integer)this.literals.get(i);

			if (!lit.equals(literal) && !lit.equals(-literal)) {
				continue;
			}

			if ((value.equals(1) && lit > 0) ||
			    (value.equals(0) && lit < 0)) {
				// remove clause
				nc = new Clause();
				nc.removed = 1;


				for (i = 0; i < this.size; i = i + 1) {
					state.decrementLiteral(abs((Integer)this.literals.get(i)));
				}
			} else {
				// remove literal
				state.decrementLiteral(abs(lit));

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

			break;
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

	String toString() {
		Integer i;
		String str;

		str = "[";

		str = str + "|" + "num_clauses=" + num_clauses.toString() + ",";
		str = str + "num_variables=" + num_variables.toString() + ",";
		str = str + "unit_literal=" + unit_literal.toString() + ",";
		str = str + "unit_value=" + unit_value.toString() + ",";
		str = str + "next_unassigned=" + next_unassigned.toString() + ",";
		str = str + "empty_clause=" + empty_clause.toString() + "| ";

		for (i = 0; i < this.literals_size; i = i + 1) {
			str = str + ((Integer)this.literals.get(i)).toString() + " ";
		}

		str = str + "]";

		return str;
	}
	
	Object readDimac() {
		String str;
		Clause clause;
		Integer i, n, p, literal, count;

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

				p = str.toInteger();
				clause.literals.put(0, p);
				p = abs(p);
				this.literals.put(p, ((Integer)this.literals.get(p)) + 1);
				
				for (n = 1; (str = in) != null && !str.equals("0"); n = n + 1) {
					p = str.toInteger();
					clause.literals.put(n, p);
					p = abs(p);
					this.literals.put(p, ((Integer)this.literals.get(p)) + 1);
				}

				clause.size = n;

				this.clauses.put(count, clause);
				count = count + 1;
			}
		}

		this.unit_literal = -1;
		this.unit_value = 0;
		this.empty_clause = 0;

		for (i = 0; i < this.clauses_size; i = i + 1) {
			clause = (Clause)this.clauses.get(i);

			if (clause.size.equals(0)) {
				this.empty_clause = 1;
			} else if (clause.size.equals(1)) {
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

		return null;
	}

	Integer abs(Integer a) {
		if (a < 0) return -a;
		return a;
	}
	
	Object decrementLiteral(Integer literal) {
		Integer l;

		l = (Integer)this.literals.get(literal);

		if ((l - 1) < 0) {
			out "ERROR: Literal must be non-negative!" + newline;
		}

		this.literals.put(literal, l - 1);

		
		return null;
	}

	State assignLiteral(Integer literal, Integer value) {
		Clause clause;
		Integer i, lit, size;
		
		this.unit_literal = -1;
		this.unit_value = 0;
		this.empty_clause = 0;

		for (i = 0; i < this.clauses_size; i = i + 1) {
			clause = (Clause)this.clauses.get(i);

			if (clause.removed || clause.size.equals(0)) {
				continue;
			}

			clause = clause.assignLiteral(this, literal, value);
			this.clauses.put(i, clause);

			if (clause.removed) {
				this.num_clauses = this.num_clauses - 1;
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
			lit = (Integer)this.literals.get(i);
			if (lit > 0) {
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

	Integer solved;

	Node() {
		this.solved = 0;
	}
	
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
		if (this.state != null) {
			this.state = this.state.assignLiteral(this.literal, this.value);
		}

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

class RunQueue extends Thread {
	Integer index;
	Integer size;
	DPL dpl;
	Integer stalled;
	Table levels;

	RunQueue(Integer index, Integer size, DPL dpl) {
		Integer i;

		this.index = index;
		this.size = size;
		this.dpl = dpl;
		this.stalled = 0;
		this.levels = new Table(this.size);

		for (i = 0; i < this.size; i = i + 1) {
			this.levels.put(i, new QueueList());
		}
	}

	Object print(String str) {
		out "[thread " + this.index.toString() + "] " + str + newline;
		return null;
	}
	
	Object run() {
		Node node;
		Integer i, thread;
		RunQueue queue;

		for (;;) {
			synchronized (this.dpl.print_mutex) {
				if (this.dpl.solved) {
					break;
				}
			}
			
			node = this.remove();

			if (node != null) {
				// print("no steal necessary");
			} else {
				for (;;) {
					this.stalled = 1;
					for (i = 0; i < this.dpl.num_threads; i = i + 1) {
						if (!(((RunQueue)this.dpl.runQueues.get(i)).stalled))
							break;
					}

					// print("check stalled, i = " + i.toString());

					if (i.equals(this.dpl.num_threads)) {
						synchronized (this.dpl.print_mutex) {
							print("no solution found");
							return null;
						}
					}

					i = i.rand().mod(this.dpl.num_threads);
					if (i.equals(this.index))
					    continue;

					if (!this.dpl.random_steal)
						node = ((RunQueue)this.dpl.runQueues.get(i)).steal();
					else
						node = ((RunQueue)this.dpl.runQueues.get(i)).stealRandom();

					if (node != null) {
						this.stalled = 0;
						print("stole from " + i.toString());
						break;
					}

					// this.sleep(10);
				}
			}

			if (node == null)
				continue;

			node = this.dpl.dpl(node.simplifyState(), this.index);
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

		i = 0;

		for (;;) {
			i = i.rand().mod(this.size);
			list = (QueueList)this.levels.get(i);
			if (!list.empty())
				return list.removeLast();
		}

		return null;
	}
}

class DPL {
	Node root;
	Integer solved;
	Integer num_threads;
	Integer random_steal;
	Integer parallel;
	Object print_mutex;
	Table runQueues;
	
	DPL(Integer num_threads) {
		this.root = null;
		this.solved = 0;
		this.random_steal = 1;
		this.parallel = 1;
		this.num_threads = num_threads;
		this.print_mutex = new Object();
	}

	Object printSolution(Node child) {
		Integer i;
		Integer value, num_variables, num_clauses;
		Table values;
		State state;

		synchronized (this.print_mutex) {
			if (this.solved)
				return null;

			this.solved = 1;

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
				if (((Integer)values.get(i)).equals(0)) {
					continue;
				}
				out "v " + ((Integer)values.get(i)).toString() + newline;
			}
		}
		
		return null;
	}
	
	Object initialize() {
		State state;
		Integer i, num_variables;

		state = new State();
		state.readDimac();

		num_variables = state.num_variables;

		this.root = new Node(null, 0, 0);

		this.root.state = state;

		this.parallel = 0;

		if (this.parallel) {
			this.runQueues = new Table(num_threads);

			for (i = 0; i < num_threads; i = i + 1) {
				this.runQueues.put(i, new RunQueue(i, num_variables+1, this));
			}
		}
		
		return null;
	}

	Node simplify(Node node, Integer literal, Integer value) {
		Node child;

		child = new Node(node, literal, value);

		return child.simplifyState();
	}

	Node dpl(Node node, Integer index) {
		Integer unit_literal, unit_value, next_unassigned,
			empty_clause, num_clauses, num_variables;
		Node retval, child1, child2;
		State state;

		// out "        in DPL(" + node.hashCode().toString() + ")" + newline;

		retval = null;
		state = node.state;

		num_clauses = state.num_clauses;
		num_variables = state.num_variables;
		unit_literal = state.unit_literal;
		unit_value = state.unit_value;
		next_unassigned = state.next_unassigned;
		empty_clause = state.empty_clause;

		if (num_clauses.equals(1)) {
			Clause c;
			Integer i;

			for (i = 0; i < state.clauses_size; i = i + 1) {
				c = (Clause)state.clauses.get(i);
				if (c.removed || c.size.equals(0)) continue;
			}
		}

		if (!empty_clause.equals(0)) {
			// empty clause, return false
			// out "        empty clause found" + newline;
		} else if (num_clauses.equals(0)) {
			// no clauses, return true
			// out "        num_clauses == 0" + newline;
			this.printSolution(node);
			retval = new Node();
			retval.solved = 1;
			return retval;
		} else if (!unit_literal.equals(-1)) {
			// unit clause, simplify
			// out "        unit literal found, l = " + unit_literal.toString() +
			// 	", v = " + unit_value.toString() + newline;
			child1 = this.simplify(node, unit_literal, unit_value);
			retval = this.dpl(child1, index);
		} else if (next_unassigned <= num_variables) {
			// assign next unassigned literal
			// out "        assigning next unassigned = " + next_unassigned.toString() + newline;
			child1 = new Node(node, next_unassigned, 0);
			child2 = new Node(node, next_unassigned, 1);

			if (this.parallel) {
				((RunQueue)this.runQueues.get(index)).enqueue(child1);
			}

			retval = this.dpl(child2.simplifyState(), index);

			if (!this.parallel) {
				if (retval == null)
					retval = this.dpl(child1.simplifyState(), index);
			}
		}

		return retval;
	}

	Integer _main() {
		Node node;
		Integer retval;

		retval = 0;
		initialize();

		if (!this.parallel) {
			node = this.dpl(this.root, 0);
		} else {
			State state;
			RunQueue queue;
			Integer i, unit_literal, unit_value, next_unassigned;
			
			state = root.state;

			unit_literal = state.unit_literal;
			unit_value = state.unit_value;
			next_unassigned = state.next_unassigned;

			queue = (RunQueue)this.runQueues.get(0);

			if (!unit_literal.equals(-1)) {
				queue.enqueue(this.simplify(root, unit_literal, unit_value));
			} else {
				queue.enqueue(this.simplify(root, next_unassigned, 0));
				queue.enqueue(this.simplify(root, next_unassigned, 1));
			}

			for (i = 0; i < this.num_threads; i = i + 1) {
				queue = (RunQueue)this.runQueues.get(i);
				queue.start();
			}

			for (i = 0; i < this.num_threads; i = i + 1) {
				queue = (RunQueue)this.runQueues.get(i);
				queue.join();
			}
		}

		if (node != null && !node.solved)
			out "no solution found" + newline;
		
		return retval;
	}
}

Integer main() {
	DPL dpl;

	dpl = new DPL(in.toInteger());
	
	return dpl._main();
}
