class ReplicatedString extends String {

  Integer count;

  ReplicatedString(String s, Integer count)
  {
    super(s);
    this.count = count;
  }

  Integer getCount()
  {
    return count;
  }

  Integer print()
  {
    Integer i;
    i = 0;
    while (i < count)
    {
      out this;
      out newline;
      i = i + 1;
    }
  }
}

Integer main()
{
  ReplicatedString x;
  x = new ReplicatedString("abc", 3);
  out x.getCount();
  out newline;
  x.print();
  return 0;
}

