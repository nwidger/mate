class MyThread extends Thread {
  MyThread() {

  }
  
  Object run() {
    out "thread:acquiring monitor..." + newline;
    
    synchronized (this) {
      out "thread:done." + newline;

      out "thread:notifying..." + newline;
      this.notify();
      out "thread:done." + newline;
      
      out "thread:releasing monitor..." + newline;    
    }

    out "thread:done." + newline;

    return null;
  }
}

Integer main() {
  MyThread t;

  t = new MyThread();

  out "main:acquiring monitor..." + newline;
  
  synchronized (t) {
    out "main:done." + newline;
    
    out "main:starting thread..." + newline;
    t.start();
    out "main:done." + newline;

    out "main:waiting on thread..." + newline;
    t.wait();
    out "main:done." + newline;

    out "main:releasing monitor..." + newline;
  }

  out "main:done." + newline;
  
  return 0;
}
