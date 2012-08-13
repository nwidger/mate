class IntegerTable {
  Table t;
  
  IntegerTable() {
    t = new Table();
  }
  
  IntegerTable(Integer n) {
    t = new Table(n);
  }

  Integer get(Integer key) {
    Object value;

    value = t.get(key);

    if (value == null)
      return 0;

    return (Integer)value;
  }

  Integer put(Integer key, Integer value) {
    Object prev;

    prev = t.put(key, value);

    if (prev == null)
      return 0;

    return (Integer)prev;
  }

  Integer remove(Integer key) {
    Object value;

    value = t.remove(key);

    if (value == null)
      return 0;

    return (Integer)value;
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

class Barrier {
  Integer P;
  Integer Q;
  Integer go;
  Object lock;
  
  Barrier(Integer P) {
    this.P = P;
    this.Q = P;
    this.go = 0;
    this.lock = new Object();
  }

  Object await() {
    synchronized(this.lock) {
      this.Q = this.Q - 1;

      if (this.Q.equals(0)) {
	this.Q = this.P;
	this.go = 1;
	return null;
      }

      this.go = 0;
    }

    for (;;) {
      synchronized(this.lock) {
	if (this.go) {
	  break;
	}
      }
    }

    return null;
  }
}

class GlobalMemory {
  Real MAXRAND;
  Integer n;
  Integer P;
  Integer block_size;
  Integer nblocks;
  Integer dostats;
  Integer num_rows;
  Integer num_cols;
  Integer doprint;
  RealTable a;
  RealTable rhs;
  IntegerTable proc_bytes;
  RealTable t_in_fac;
  RealTable t_in_solve;
  RealTable t_in_mod;
  RealTable t_in_bar;
  RealTable completion;
  Integer starttime;
  Integer rf;
  Integer rs;
  Integer done;
  Integer id;
  Barrier start;
  Object idlock;
  Table slaves;
  
  GlobalMemory(Integer P) {
    this.P = P;
    
    t_in_fac = new RealTable(P);
    t_in_mod = new RealTable(P);
    t_in_solve = new RealTable(P);
    t_in_bar = new RealTable(P);
    completion = new RealTable(P);

    start = new Barrier(P);

    idlock = new Object();
    id = 0;

    slaves = new Table(P);
  }

  Integer min(Integer a, Integer b) {
    if (a < b)
      return a;
    else
      return b;
  }

  Object InitA() {
    Integer i, j;

    // srand48(1);
    for (j = 0; j < this.n; j = j + 1) {
      for (i = 0; i < this.n; i = i + 1) {
	a.put(i+j*n, 3.14159);
	if (i.equals(j)) {
	  a.put(i+j*n, a.get(i+j*n) * 10.0);
	}
      }
    }

    for (j = 0; j < n; j = j + 1) {
      this.rhs.put(j, 0.0);
    }
    for (j = 0; j < n; j = j + 1) {
      for (i = 0; i < this.n; i = i + 1) {
	this.rhs.put(i, this.rhs.get(i) + a.get(i+j*n));
      }
    }
    
    return null;
  }

  Object PrintA()
  {
    Integer i, j;

    for (i = 0; i < this.n; i = i + 1) {
      for (j = 0; j < this.n; j = j + 1) {
	out a.get(i+j*n).toString() + " ";
      }
      out newline;
    }

    return null;
  }

  Integer BlockOwner(Integer I, Integer J) {
    return ((I.mod(num_cols)) + (J.mod(num_rows))*num_cols);
  }

  Real TouchA(Integer MyNum) {
    Integer i, j, I, J, bs;
    Real tot;

    bs = block_size;
    tot = 0.0;

    // for (J = 0; (J*bs) < n; J = J + 1) {
    //   for (I = 0; (I*bs) < n; I = I + 1) {
    // 	if (BlockOwner(I, J).equals(MyNum)) {
    // 	  for (j = (J*bs); j < ((J+1)*bs) && j < n; j = j + 1) {
    // 	    for (i = (I*bs); i < ((I+1)*bs) && i < n; i = i + 1) {
    // 	      tot = tot + a.get(i+j*n);
    // 	    }
    // 	  }
    // 	}
    //   }
    // }

    return tot;
  }
}

class LocalCopies {
  Real t_in_fac;
  Real t_in_solve;
  Real t_in_mod;
  Real t_in_bar;

  LocalCopies() {
    t_in_fac = 0.0;
    t_in_solve = 0.0;
    t_in_mod = 0.0;
    t_in_bar = 0.0;
  }
}

class Slave extends Thread {
  Integer MyNum;
  LocalCopies lc;
  GlobalMemory global;
  
  Slave(GlobalMemory global) {
    this.global = global;

    lc = new LocalCopies();
  }

  Object run() {
    Integer i, j, cluster, max_block;

    synchronized(global.idlock) {
      this.MyNum = global.id;
      global.id = global.id + 1;
    }

    this.OneSolve();
    
    return null;
  }

  Object OneSolve() {
    Integer i, myrs, myrf, mydone;

    myrs = myrf = mydone = 0;

    global.start.await();

    global.TouchA(MyNum);

    global.start.await();

    if (MyNum.equals(0) || global.dostats) {
      // CLOCK(myrs);
    }

    // lu();

    if (MyNum.equals(0) || global.dostats) {
      // CLOCK(mydone);
    }

    global.start.await();

    if (MyNum.equals(0) || global.dostats) {
      // CLOCK(myrf);

      global.t_in_fac.put(MyNum, lc.t_in_fac);
      global.t_in_solve.put(MyNum, lc.t_in_solve);
      global.t_in_mod.put(MyNum, lc.t_in_mod);
      global.t_in_bar.put(MyNum, lc.t_in_bar);
      global.completion.put(MyNum, new Real(mydone - myrs));
    }

    if (MyNum.equals(0)) {
      global.rs = myrs;
      global.done = mydone;
      global.rf = myrf;
    }
    
    return null;
  }
}

Integer main() {
  String opt, optarg;
  GlobalMemory Global;
  Integer n, P, block_size, nblocks, dostats, test_result, doprint,
    i, j, last_page, proc_num, edge, size, start, MyNum;
  Real mint, maxt, avgt,
    min_fac, min_solve, min_mod, min_bar,
    max_fac, max_solve, max_mod, max_bar,
    avg_fac, avg_solve, avg_mod, avg_bar;

  n = 10;
  P = 4;
  block_size = 16;
  doprint = 0;
  dostats = 0;
  
  MyNum = 0;

  while ((opt = in) != null) {
    if (opt.equals("-n")) {
      if ((optarg = in) == null) { out "Need value for " + opt.toString() + newline; }
      n = optarg.toInteger();
    } else if (opt.equals("-p")) {
      if ((optarg = in) == null) { out "Need value for " + opt.toString() + newline; }
      P = optarg.toInteger();
    } else if (opt.equals("-b")) {
      if ((optarg = in) == null) { out "Need value for " + opt.toString() + newline; }
      block_size = optarg.toInteger();
    } else if (opt.equals("-s")) {
      if ((optarg = in) == null) { out "Need value for " + opt.toString() + newline; }
      dostats = optarg.toInteger();
    } else if (opt.equals("-t")) {
      test_result = !test_result;
    } else if (opt.equals("-o")) {
      doprint = !doprint;
    } else if (opt.equals("-h")) {
      out "usage: LU <options>" + newline + newline;
      out "options:\n" + newline;
      out "  -nN : Decompose NxN matrix." + newline;
      out "  -pP : P = number of processors." + newline;
      out "  -bB : Use a block size of B. BxB elements should fit in cache for" + newline;
      out "        good performance. Small block sizes (B=8, B=16) work well." + newline;
      out "  -c  : Copy non-locally allocated blocks to local memory before use." + newline;
      out "  -s  : Print individual processor timing statistics." + newline;
      out "  -t  : Test output." + newline;
      out "  -o  : Print out matrix values." + newline;
      out "  -h  : Print out command line options." + newline + newline;
      out "Default: LU -n" + n.toString() + " -p" + P.toString() + " -b" + block_size.toString() + newline;
      return 1;
    } else {
      out "Invalid argument " + opt.toString() + newline;
    }
  }

  out newline;
  out "Blocked Dense LU Factorization" + newline;
  out "     " + n.toString() + " by " + n.toString() + " Matrix" + newline;
  out "     " + P.toString() + " Processors" + newline;
  out "     " + block_size.toString() + " by " + block_size.toString() + " Element Blocks" + newline;
  out newline;
  out newline;

  Global = new GlobalMemory(P);

  Global.MAXRAND = 32767.0;
  Global.n = n;
  Global.P = P;
  Global.block_size = block_size;
  Global.nblocks = nblocks;
  Global.dostats = dostats;
  Global.doprint = doprint;

  Global.num_rows = new Integer(new Real(Global.P).squareRoot());
  for (;;) {
    Global.num_cols = Global.P/Global.num_rows;
    if ((Global.num_rows*Global.num_cols).equals(Global.P))
      break;
    Global.num_rows = Global.num_rows - 1;
  }
  Global.nblocks = Global.n/Global.block_size;
  if (!(Global.block_size * Global.nblocks).equals(Global.n)) {
    Global.nblocks = Global.nblocks + 1;
  }

  Global.a = new RealTable(Global.nblocks*Global.nblocks);
  Global.rhs = new RealTable(Global.n);

  Global.InitA();
  if (doprint) {
    out "Matrix before decomposition:" + newline;
    Global.PrintA();
  }

  for (i = 0; i < P; i = i + 1) {
    Global.slaves.put(i, new Slave(Global));
  }

  for (i = 0; i < P; i = i + 1) {
    ((Slave)Global.slaves.get(i)).start();
  }

  for (i = 0; i < P; i = i + 1) {
    ((Slave)Global.slaves.get(i)).join();
  }

  if (doprint) {
    out "Matrix after decomposition:" + newline;
    Global.PrintA();
  }

  return 0;
}
