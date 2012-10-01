Integer main() {
  Integer i;

  for (i = 0; i < 10; i = i + 1) {
    out "i = " + i.toString() + newline;
    if (1)
      continue;
  }

  out "finished!" + newline;
  
  return 0;
}
