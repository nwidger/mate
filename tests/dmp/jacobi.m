class RealTable {
  Table t;
  
  RealTable() {
    t = new Table();
  }
  
  RealTable(Integer n) {
    t = new Table(n);
  }

  Real get(Integer key) {
    return this.get(new Real(key));
  }
  
  Real get(Real key) {
    Object value;

    value = t.get(key);

    if (value == null)
      return 0.0;

    return (Real)value;
  }

  Real put(Integer key, Real value) {
    return this.put(new Real(key), value);
  }

  Real put(Real key, Real value) {
    Object prev;

    prev = t.put(key, value);

    if (prev == null)
      return 0.0;

    return (Real)prev;
  }

  Real remove(Real key) {
    Object value;

    value = t.remove(key);

    if (value == null)
      return 0.0;

    return (Real)value;
  }

  Integer firstKey() {
    return t.firstKey();
  }

  Integer nextKey() {
    Object key;

    key = t.nextKey();

    if (key == null)
      return null;

    return (Integer)key;
  }
}

class Timer extends Thread {
  Integer started;
  Integer millisecs;
  Integer granularity;

  Timer() {
    this(50);
  }

  Timer(Integer g) {
    this.started = 0;
    this.millisecs = 0;
    this.granularity = g;
  }

  Object start_timer() {
    synchronized (this) {
      this.started = 1;
    }

    this.millisecs = 0;
    this.start();
  }

  Integer stop_timer() {
    Integer m;
    
    synchronized (this) {
      m = new Integer(this.millisecs);
      this.started = 0;
      
    }
    
    return m;
  }

  Object run() {
    for (;;) {
      synchronized (this) {
	if (!this.started) {
	  break;
	}
      }
      
      this.sleep(this.granularity);
      this.millisecs = this.millisecs + this.granularity;
    }
  }
}

class Jacobi {
  // Constants
  Integer PLATE_HEIGHT;
  Integer PLATE_WIDTH;
  Integer NUM_ITERS;
  Real TEMP;

  Jacobi() {
    // this.PLATE_HEIGHT = 2048;
    // this.PLATE_WIDTH = 256;
    // this.NUM_ITERS = 200;
    // this.TEMP = 50.0;

    this.PLATE_HEIGHT = 20;
    this.PLATE_WIDTH = 25;
    this.NUM_ITERS = 20;
    this.TEMP = 50.0;
  }

  Object doit() {
    Timer timer;
    Integer millisecs;

    Integer i, j, count;
    Integer start_row, start_col;
    Integer limit_row, limit_col;
    Real change, maxchange, tmp;
    Integer cool_cell;
    Integer this_row, row_above, row_below, new_row;
    RealTable newp;
    RealTable oldp;

    timer = new Timer();
    
    // Real newp[][] = new double[PLATE_HEIGHT][PLATE_WIDTH];
    // Real oldp[][] = new double[PLATE_HEIGHT][PLATE_WIDTH];
    newp = new RealTable(PLATE_HEIGHT*PLATE_WIDTH);
    oldp = new RealTable(PLATE_HEIGHT*PLATE_WIDTH);

    timer.start_timer();
    
    start_row = 1;
    limit_row = PLATE_HEIGHT-1;

    start_col = 1;
    limit_col = PLATE_WIDTH-1;


    // mwm Aug 98: the first dimension access of these arrays should be elided
    // for additional speed
    // double[] ad = oldp[0], ad2 = newp[0];

    for( j = 0;  j < PLATE_WIDTH;  j = j + 1 ) {
      oldp.put(j, 0.0);
      newp.put(j, 0.0);
    }

    Integer t;

    t = PLATE_HEIGHT*limit_row;
    oldp.put(t, 0.0);
    newp.put(t, 0.0);
    oldp.put(t+limit_col, 0.0);
    newp.put(t+limit_col, 0.0);

    // mwm Aug 98: the first dimension access of these arrays should be elided
    // for additional speed
    // ad = oldp[limit_row];
    // ad2 = newp[limit_row];
    // for( j = start_col;  j < limit_col;  j = j + 1 ) {
    //   oldp[limit_row][j] = (double)100.0;
    //   newp[limit_row][j] = (double)100.0;
    // }

    for( i = start_row;  i < limit_row;  i = i + 1 ) {
      // this_row = oldp[i];
      // new_row = newp[i];
      // this_row[0] = 0.0;
      // new_row[0] = 0.0;
      // this_row[limit_col] = 0.0;
      // new_row[limit_col] = 0.0;

      this_row = PLATE_HEIGHT*i;
      new_row = PLATE_HEIGHT*i;

      oldp.put(this_row, 0.0);
      newp.put(new_row, 0.0);
      oldp.put(this_row+limit_col, 0.0);
      newp.put(new_row+limit_col, 0.0);

      for( j = start_col;  j < limit_col;  j = j + 1 ) {
	// new_row[j] = 50.0;
	newp.put(new_row+j, 50.0);
      }
    }

    // now iterate over the matrix
    count = 0;
    while(count < NUM_ITERS) {
      // first save all newly computed values in oldp cells
      for( i = start_row;  i < limit_row;  i = i + 1 ) {
	// mwm Aug 98: we can do this more efficiently by swapping arrays
	// (a feature we don't have in C)
	// this_row = oldp[i];
	// new_row = newp[i];
	// for( j = start_col;  j < limit_col;  j = j + 1 )
	//   this_row[j] = new_row[j];

	this_row = PLATE_HEIGHT*i;
	new_row = PLATE_HEIGHT*i;

	for( j = start_col;  j < limit_col;  j = j + 1 )
	  oldp.put(this_row+j, newp.get(new_row+j));

	// ad = oldp[i];
	// ol dp[i] = newp[i];
	// newp[i] = ad;
      }

      maxchange = 0.0;
      for( i = start_row;  i < limit_row;  i = i + 1 ) {
	// this_row = oldp[i];
	// row_above = oldp[i-1];
	// row_below = oldp[i+1];
	// new_row = newp[i];

	this_row = PLATE_HEIGHT*i;
	row_above = PLATE_HEIGHT*(i-1);
	row_below = PLATE_HEIGHT*(i+1);
	new_row = PLATE_HEIGHT*i;

	for( j = start_col;  j < limit_col;  j = j + 1 ) {
	  // mwm Aug 98: store the new_row value in a temp and use that later
	  // instead of re-accessing the array - unfortunately, this doesn't
	  // seem to help.
	  // tmp = new_row[j] = ( this_row[j-1] +
	  // 		       this_row[j+1] +
	  // 		       row_above[j] +
	  // 		       row_below[j] ) / 4.0;

	  newp.put(new_row+j, (oldp.get(this_row+(j-1)) +
					oldp.get(this_row+(j+1)) +
					oldp.get(row_above+j) +
					oldp.get(row_below+j)) / 4.0);

	  tmp = newp.get(new_row+j);

	  // mwm Aug 98: a method call prevents the java2c optimizer from
	  // converting object references into pointers inside loops.  The
	  // reason is because the method may contain a synchronization which
	  // could invalidate those pointers by flushing remote objects.  A
	  // temporary fix is to replace the Math.abs() call with explicit
	  // code to do the same thing.
	  //          change = Math.abs(this_row[j] - new_row[j]);

	  // change = this_row[j] - tmp;

	  change = oldp.get(this_row+j) - tmp;
	  if (change < 0.0)
	    change = -change;
	  if (change > maxchange)
	    maxchange = change;
	}
      }

      // IO.println("max_diff = " + maxchange);
      count = count + 1;
    }

    cool_cell = 0;
    for( i = start_row;  i < limit_row;  i = i + 1 ) {
      // new_row = newp[i];
      new_row = PLATE_HEIGHT*i;
      
      for( j = 0;  j < PLATE_WIDTH;  j = j + 1 ) {
	// if( new_row[j] < TEMP ) {
	//   cool_cell++;
	// }

	if (newp.get(new_row+j) < TEMP) {
	  cool_cell = cool_cell + 1;
	}
      }
    }

    millisecs = timer.stop_timer();
    out "Time (millisecs): " + millisecs.toString() + newline;
    out count.toString() + " iterations, maxchange = " + maxchange.toString() + newline;
    out "Cells cooler than 50 degrees: " + cool_cell.toString() + newline;

    return null;
  }
}

Integer main() {
  Jacobi jacobi;

  jacobi = new Jacobi();
  jacobi.doit();
  
  return 0;
}
