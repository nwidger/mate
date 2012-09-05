#include "IntegerTable.m"
#include "MDRealTable.m"
#include "Timer.m"

// This maTe program completely solves a system of linear equations using
// (1) gaussian elimination with partial pivoting and (2) back substitution.
//
// The size of the matrix to be solved is expected as the single command-line
// argument. (If not given, the default size is 32.) A random matrix of that
// size is generated and then solved.

class Gaussian {
  Integer N;
  MDRealTable a;
  Timer timer;
  Real EPSILON;
  IntegerTable pivot, marked;

  Gaussian() {
    this(32);
  }

  Gaussian(Integer N) {
    this.N = N;
    EPSILON = 1.0e-20;
    timer = new Timer();

    a = new MDRealTable(N, N+1);
    pivot = new IntegerTable(N);
    marked = new IntegerTable(N);

    initialize_system();
  }

  Object doit() {
    start_timer();
    if (gaussian_elimination()) {
      back_substitution();
      stop_timer();
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
    Integer i, j, iN;

    for (i = 0; i < N; i = i + 1) {
      iN = i*N;
      marked.put(i, 0);
      a.put(i, N, 0.0);
      for (j = 0; j < N; j = j + 1) {
	a.put(i, j, in.toReal());
	a.put(i, N, a.get(i, N) + (new Real(j) * a.get(i, j)));
      }
    }

    return null;
  }

  // reduce the matrix using partial pivoting
  Integer gaussian_elimination() {
    Integer i, j, k, picked, pickedN, jN;
    Real tmp;

    picked = -1;

    for (i = 0; i < (N - 1); i = i + 1) {
      tmp = 0.0;
      for (j = 0; j < N; j = j + 1) {
	jN = j*N;
	if (marked.get(j).equals(0) && abs(a.get(j, i)) > tmp) {
	  tmp = abs(a.get(j, i));
	  picked = j;
	}
      }

      pickedN = picked*N;

      marked.put(picked, 1); // Mark pivot row
      pivot.put(picked, i);  // Remember permuted position

      if (abs(a.get(picked, i)) < EPSILON) {
	out "Exits on iteration " + i.toString() + newline;
	return 0;
      }

      for (j = 0; j < N; j = j + 1) {
	if (marked.get(j).equals(0)) {
	  jN = j*N;
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
    Integer i, j, jN;

    for (i = N-1; i >= 0; i = i - 1) {
      for (j = 0; !pivot.get(j).equals(i); j = j + 1) { }
      jN = j*N;
      coeff = a.get(j, N) / a.get(j, i);
      for (j = 0; j < N; j = j + 1) {
	jN = j*N;
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
    timer.join();

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
