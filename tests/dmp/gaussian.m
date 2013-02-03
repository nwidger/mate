#include "IntegerTable.m"
#include "MDRealTable.m"
#include "Timer.m"
#include "Barrier.m"

// This maTe program completely solves a system of linear equations using
// (1) gaussian elimination with partial pivoting and (2) back substitution.
//
// The size of the matrix to be solved is expected as the single command-line
// argument. (If not given, the default size is 32.) A random matrix of that
// size is generated and then solved.

class GBarrier extends Barrier {
	Integer done;
	Table workers;

	Integer picked;
	Real tmp;

	GBarrier(Integer P, Table workers) {
		super(P);
		this.done = 0;
		this.workers = workers;
		this.picked = -1;
		this.tmp = 0.0;
	}
}

class Worker extends Thread {
	Integer id;
	Integer num_threads;
	GBarrier barrier;

	Integer N;
	MDRealTable a;

	IntegerTable marked;
	IntegerTable pivot;

	Real EPSILON;

	Worker(Integer id, Integer num_threads, GBarrier barrier, Integer N, MDRealTable a, IntegerTable marked, IntegerTable pivot, Real EPSILON) {
		this.id = id;
		this.num_threads = num_threads;
		this.barrier = barrier;

		this.N = N;
		this.a = a;

		this.marked = marked;
		this.pivot = pivot;

		this.EPSILON = EPSILON;
	}

	Real abs(Real x) {
		if (x < 0.0)
			return -x;
		else
			return x;
	}

	Object run() {
		Integer i, j, k, picked;
		Real tmp;

		picked = -1;

		for (i = 0; i < (N - 1); i = i + 1) {
			tmp = 0.0;
			if (id.equals(0)) {
				tmp = 0.0;
				for (j = 0; j < N; j = j + 1) {
					if (marked.get(j).equals(0) && abs(a.get(j, i)) > tmp) {
						tmp = abs(a.get(j, i));
						picked = j;
					}
				}

				this.barrier.tmp = tmp;
				this.barrier.picked = picked;

				marked.put(this.barrier.picked, 1); // Mark pivot row
				pivot.put(this.barrier.picked, i); // Remember permuted position

				if (abs(a.get(this.barrier.picked, i)) < EPSILON) {
					out "Exits on iteration " + i.toString() + newline;
					this.barrier.done = 1;
				}
			}

			barrier.await();

			if (this.barrier.done) {
				return null;
			}

			tmp = this.barrier.tmp;
			picked = this.barrier.picked;
			
			for (j = 0; j < N; j = j + 1) {
				if (id.equals(j.mod(num_threads))) {
					if (marked.get(j).equals(0)) {
						tmp = a.get(j, i) / a.get(picked, i);
						for (k = i; k < (N+1); k = k + 1) {
							Real olda, newa;
							olda = a.get(j, k);
							a.put(j, k, a.get(j, k) - (a.get(picked, k) * tmp));
							newa = a.get(j, k);
						}
					}
				}
			}
			
			barrier.await();

			if (id.equals(0)) {
				this.barrier.tmp = 0.0;
			}
		}

		for (i = 0; i < N; i = i + 1) {
			if (id.equals(i.mod(num_threads))) {
				if (marked.get(i).equals(0)) {
					pivot.put(i, N-1);
				}
			}
		}

		return null;
	}
}

class Gaussian {
	Integer parallel;
	Integer num_threads;

	Integer N;
	MDRealTable a;
	Timer timer;
	Real EPSILON;
	IntegerTable pivot, marked;

	Gaussian() {
		this(32);
	}

	Gaussian(Integer N) {
		this.parallel = 1;
		
		this.N = N;
		this.num_threads = N;

		EPSILON = 1.0e-20;
		timer = new Timer();

		a = new MDRealTable(N, N+1);
		pivot = new IntegerTable(N);
		marked = new IntegerTable(N);

		initialize_system();
	}

	Object doit() {
		start_timer();
		out "running gaussian_elimination" + newline;
		if (gaussian_elimination()) {
			out "running back_substitution" + newline;
			back_substitution();
			out "running stop_timer" + newline;
			stop_timer();
			out "running check_results" + newline;
			check_results();
		} else {
			out "No solution?" + newline;
		}

		return null;
	}

	Real abs(Real x) {
		if (x < 0.0)
			return -x;
		else
			return x;
	}

	// cook up a system where ans.get(i) will be i
	Object initialize_system() {
		Real d;
		Integer i, j, r;

		r = 0;

		for (i = 0; i < N; i = i + 1) {
			marked.put(i, 0);
			a.put(i, N, 0.0);
			for (j = 0; j < N; j = j + 1) {
				d = new Real(r.rand().mod(100)) / 100.0;
				a.put(i, j, d);
				a.put(i, N, a.get(i, N) + (new Real(j) * a.get(i, j)));
			}
		}

		return null;
	}

	Integer gaussian_elimination() {
		if (!this.parallel) {
			return serial_gaussian_elimination();
		} else {
			Integer i;
			Worker worker;
			Table workers;
			GBarrier barrier;

			barrier = new GBarrier(this.num_threads, workers);
			workers = new Table(this.num_threads);
			
			for (i = 0; i < this.num_threads; i = i + 1) {
				worker = new Worker(i, this.num_threads, barrier, N, a, marked, pivot, EPSILON);
				workers.put(i, worker);
				worker.start();
			}

			for (i = 0; i < this.num_threads; i = i + 1) {
				worker = (Worker)workers.get(i);
				worker.join();
			}

			return 1;
		}
	}
	
	// reduce the matrix using partial pivoting
	Integer serial_gaussian_elimination() {
		Integer i, j, k, picked;
		Real tmp;

		picked = -1;

		for (i = 0; i < (N - 1); i = i + 1) {
			tmp = 0.0;
			for (j = 0; j < N; j = j + 1) {
				if (marked.get(j).equals(0) && abs(a.get(j, i)) > tmp) {
					tmp = abs(a.get(j, i));
					picked = j;
				}
			}

			marked.put(picked, 1); // Mark pivot row
			pivot.put(picked, i);  // Remember permuted position

			if (abs(a.get(picked, i)) < EPSILON) {
				out "Exits on iteration " + i.toString() + newline;
				return 0;
			}

			for (j = 0; j < N; j = j + 1) {
				if (marked.get(j).equals(0)) {
					tmp = a.get(j, i) / a.get(picked, i);
					for (k = i; k < (N+1); k = k + 1) {
						Real olda, newa;
						olda = a.get(j, k);
						a.put(j, k, a.get(j, k) - (a.get(picked, k) * tmp));
						newa = a.get(j, k);
					}
				}
			}
		}

		for (i = 0; i < N; i = i + 1) {
			if (marked.get(i).equals(0)) {
				pivot.put(i, N-1);
			}
		}

		return 1;
	}

	// solve the (logical) upper triangular matrix
	Object back_substitution() {
		Real coeff;
		Integer i, j;

		for (i = N-1; i >= 0; i = i - 1) {
			for (j = 0; !pivot.get(j).equals(i); j = j + 1) { }
			coeff = a.get(j, N) / a.get(j, i);
			for (j = 0; j < N; j = j + 1) {
				if (pivot.get(j) < i)
					a.put(j, N, a.get(j, N) - (coeff * a.get(j, i)));
			}
		}

		return null;
	}

	// ans.get(i) should be (roughly) i
	// to get ans I still need to divide by the non-zero column's coefficient
	// might end up with -0.0 so I must special-case that
	Object check_results() {
		Integer i;

		for (i = 0; i < N; i = i + 1) {
			Real ans;

			ans = a.get(i, N) / a.get(i, pivot.get(i));
			if ((pivot.get(i).equals(0)) && (ans < 0.0)) ans = -ans;
			if (abs(ans - new Real(pivot.get(i))) > 0.01) {
				out "Error on iteration " + i.toString() +
					": " + new Real(pivot.get(i)).toString() + " " + ans.toString() + newline;
			}
		}

		out "Done." + newline;

		return null;
	}

	Object start_timer() {
		timer.start_timer();
	}

	Object stop_timer() {
		Real dtime;

		timer.stop_timer();
		dtime = new Real(timer.millisecs) / 1000.0;

		out "## Elapsed time: " + dtime.toString() + " seconds" + newline;
	}
}

Integer main() {
	String s;
	Gaussian g;
	Integer solution;

	if ((s = in) == null) {
		g = new Gaussian();
	} else {
		g = new Gaussian(s.toInteger());
	}

	out "Matrix size is " + g.N.toString() + newline;

	g.doit();

	return 0;
}
