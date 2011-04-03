class Point {
  Integer x, y;

  Point(Integer x, Integer y) {
    this.x = x;
    this.y = y;
  }

  Point operator+(Point p) {
    return new Point(x + p.x, y + p.y);
  }

  Integer equals(Object obj) {
    if (obj == null)
      return 0;
    if (obj instanceof Point) {
      Point p;

      p = (Point)obj;

      if (x.equals(p.x)) {
	if (y.equals(p.y)) {
	  return 1;
	}
      }
    }

    return 0;
  }

  String toString() {
    return "(" + x.toString() + ", " + y.toString() + ")";
  }
}

Integer main() {
  Point p, q, r;

  p = new Point(10, 10);
  q = new Point(5, 5);
  r = p + q;

  out "p = " + p.toString() + newline;
  out "q = " + q.toString() + newline;
  out "r = " + r.toString() + newline;

  return 0;
}
