class XYZ
{
 Integer doCalc(Integer x, Integer y, Integer z)
 {
   return x * y + z;
 }
}

Integer main()
{
  XYZ dummy;

  dummy = new XYZ();

  out (dummy.doCalc(2, 5, 9)).toString() + newline;
}
