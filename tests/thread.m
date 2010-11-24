class MyThread extends Thread {
  Integer run() {
    synchronized (this) {
      out "1 omg it works!" + newline;
      out "2 omg it works!" + newline;
      out "3 omg it works!" + newline;
      out "4 omg it works!" + newline;
      out "5 omg it works!" + newline;
      out "6 omg it works!" + newline;      
    }

    return 0;
  }

  Integer blah() {
    synchronized (this) {
      out "1 in blah!" + newline;
      out "2 in blah!" + newline;
      out "3 in blah!" + newline;
      out "4 in blah!" + newline;
      out "5 in blah!" + newline;
      out "6 in blah!" + newline;
    }

    return 0;
  }
}

Integer main() {
  MyThread t;
  
  out "creating new MyThread..." + newline;
  t = new MyThread();
  out "done." + newline;

  out "starting thread..." + newline;
  t.start();
  out "done." + newline;

  out "calling blah..." + newline;
  t.blah();
  out "done." + newline;

  out "joining..." + newline;
  t.join();
  out "done." + newline;

  out "terminating..." + newline;
  return 0;
}
