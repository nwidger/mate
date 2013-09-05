#include "RealTable.m"
// #include "Timer.m"

class Worker extends Thread {
  Real change;
  RealTable oldp, newp;
  Integer i, limit_col, start_col, PLATE_HEIGHT, PLATE_WIDTH;

  Worker(Integer i, RealTable oldp, RealTable newp, Integer limit_col, Integer start_col,
	 Integer PLATE_HEIGHT, Integer PLATE_WIDTH) {
    this.i = i;
    this.oldp = oldp;
    this.newp = newp;

    this.limit_col = limit_col;
    this.start_col = start_col;

    this.PLATE_HEIGHT = PLATE_HEIGHT;
    this.PLATE_WIDTH = PLATE_WIDTH;
    
    this.change = 0.0;
  }

  Object run() {
    Real tmp, o;
    Integer j, this_row, row_above, row_below, new_row;

    this_row = PLATE_HEIGHT*i;
    row_above = PLATE_HEIGHT*(i-1);
    row_below = PLATE_HEIGHT*(i+1);
    new_row = PLATE_HEIGHT*i;

    for (j = start_col;  j < limit_col;  j = j + 1) {
      synchronized (oldp) {
	o = oldp.get(this_row+(j-1)) +
	  oldp.get(this_row+(j+1)) +
	  oldp.get(row_above+j) +
	  oldp.get(row_below+j);
      }

      synchronized (newp) {
	newp.put(new_row+j, o / 4.0);
	tmp = newp.get(new_row+j);
      }

      synchronized (oldp) {
	change = oldp.get(this_row+j) - tmp;
      }

      if (change < 0.0)
	change = -change;
    }
    
    return null;
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
    // Timer timer;
    Integer millisecs;

    Integer i, j, count;
    Integer start_row, start_col;
    Integer limit_row, limit_col;
    Real change, maxchange, tmp;
    Integer cool_cell;
    Integer this_row, row_above, row_below, new_row;
    RealTable newp;
    RealTable oldp;

    // timer = new Timer();
    
    newp = new RealTable(PLATE_HEIGHT*PLATE_WIDTH);
    oldp = new RealTable(PLATE_HEIGHT*PLATE_WIDTH);

    // timer.start_timer();
    
    start_row = 1;
    limit_row = PLATE_HEIGHT-1;

    start_col = 1;
    limit_col = PLATE_WIDTH-1;

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

    t = PLATE_HEIGHT*limit_row;
    for( j = start_col;  j < limit_col;  j = j + 1 ) {
      oldp.put(t+j, 100.0);
      newp.put(t+j, 100.0);
    }

    for( i = start_row;  i < limit_row;  i = i + 1 ) {
      this_row = PLATE_HEIGHT*i;
      new_row = PLATE_HEIGHT*i;

      oldp.put(this_row, 0.0);
      newp.put(new_row, 0.0);
      oldp.put(this_row+limit_col, 0.0);
      newp.put(new_row+limit_col, 0.0);

      for( j = start_col;  j < limit_col;  j = j + 1 ) {
	newp.put(new_row+j, 50.0);
      }
    }

    Worker w;
    Table workers;
    workers = new Table(limit_col);

    // now iterate over the matrix
    count = 0;
    while(count < NUM_ITERS) {
      // first save all newly computed values in oldp cells
      for( i = start_row;  i < limit_row;  i = i + 1 ) {
	this_row = PLATE_HEIGHT*i;
	new_row = PLATE_HEIGHT*i;

	for( j = start_col;  j < limit_col;  j = j + 1 )
	  oldp.put(this_row+j, newp.get(new_row+j));
      }

      maxchange = 0.0;
      for( i = start_row;  i < limit_row;  i = i + 1 ) {
	w = new Worker(i, oldp, newp, limit_col, start_col, PLATE_HEIGHT, PLATE_WIDTH);
	workers.put(i, w);
	w.start();
      }

      for( i = start_row;  i < limit_row;  i = i + 1 ) {
	w = (Worker)workers.get(i);
	w.join();

	change = w.change;

	if (change > maxchange)
	  maxchange = change;
      }

      out "count = " + count.toString() + " maxchange = " + maxchange.toString() + newline;

      count = count + 1;
    }

    cool_cell = 0;
    for( i = start_row;  i < limit_row;  i = i + 1 ) {
      new_row = PLATE_HEIGHT*i;
      
      for( j = 0;  j < PLATE_WIDTH;  j = j + 1 ) {
	if (newp.get(new_row+j) < TEMP) {
	  cool_cell = cool_cell + 1;
	}
      }
    }

    // millisecs = timer.stop_timer();
    // out "Time (millisecs): " + millisecs.toString() + newline;
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
