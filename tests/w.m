
class IndexTable extends Table
{
  // constructor: must provide the length
  IndexTable(Integer size)
  {
    super(size);
  }

  Integer get(Integer index)
  {
    return (Integer) ((Table) this).get((Object) index);
  }

  Integer put(Integer index, Integer value)
  {
    return (Integer) ((Table) this).put((Object) index, (Object) value);
  }
    
}

// Sieve of Eratosthenes is implemented in main block
Integer main()
{
  // constants
  Integer N;
  Integer SIZE;
  Integer TRUE;
  Integer FALSE;

  // local variables
  Integer count;
  Integer i;
  Integer index;
  Integer value;

  // an "array" that represents only odd numbers
  //   position 0 represents 1
  //   position 1 represents 3
  //   position i represents (i * 2) + 1
  // value at position i is TRUE (1) if (i * 2) + 1 is prime
  // value at position i is FALSE (0) if (i * 2) + 1 is not prime
  IndexTable prime;

  out "Enter a number: ";
  N = in.toInteger();
  out newline;
 
  // initialize the constants 
  SIZE = N / 2;
  TRUE = 1;
  FALSE = 0;

  // create the array to track which odd numbers are prime
  prime = new IndexTable(SIZE);

  // initialize the array
  //   1 is assumed to not be a prime
  //   all other odd numbers are assumed initially to be prime
  prime.put(0, FALSE);
  i = 1;
  while (i < SIZE)
  {
    prime.put(i, TRUE);
    i = i + 1;
  }

  // now run the sieve on the array
  //   at each iteration take the next smallest number marked as prime
  //     and mark all multiples of that number as not prime
  //   continue until the smallest number marked as prime is greater
  //     than sqrt(N)
  index = 1;
  while (TRUE)
  {
    // get value represented by position of index
    value = (2 * index) + 1;

    // check terminating condition
    if (value * value > N) break;
    if (value * value == N) break;

    // now execute the sieve
    i = index + value;
    while (i < SIZE)
    {
      prime.put(i, FALSE);
      i = i + value;
    }

    // find next prime to use to sieve
    index = index + 1;
    while (prime.get(index).equals(FALSE))
    {
      index = index + 1;
    }
  }

  // count the number of primes
  count = 1;
  i = 1;
  while (i < SIZE)
  {
    if (prime.get(i).equals(TRUE))
    {
      out ((i * 2) + 1).toString() + newline;
      count = count + 1;
    }
    i = i + 1;
  }

  // print the count
  out "There are " + count.toString() + " primes less than " + N.toString() +
    newline;
}

