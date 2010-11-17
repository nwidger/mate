// Allen Hubbe
// cs812 Spring 2010

// This shows an issue I'm having with the maTe Table.


// ---- Main function with test cases ------------------- //

// Table is being repurposed as a set.  The keys make up
// the set, and members of the set map to the object, yes.

Integer main()
{
  Table table;
  Object yes;

  Object w;

  table = new Table();
  yes = new Object();

// Add "foo" to the set
// (no problem)

  w = "foo";
      out "w.toString() : " + w.toString() + newline;
      out "w.hashCode() : " + w.hashCode().toString() + newline;
      out "calling table.put(w, yes) ..." + newline;
  table.put(w, yes);
      out "... table.put(w, yes) is ok ..." + newline + newline;

// Add an A to the set
// (mvm: object has not been initialized!)

  w = new A("A");
      out "w.toString() : " + w.toString() + newline;
      out "w.hashCode() : " + w.hashCode().toString() + newline;
      out "calling table.put(w, yes) ..." + newline;
  table.put(w, yes);
      out "... table.put(w, yes) is ok ..." + newline + newline;

// Add a B to the set
// (mvm: cannot invoke method on null reference!)

  w = new B("B");
      out "w.toString() : " + w.toString() + newline;
      out "w.hashCode() : " + w.hashCode().toString() + newline;
      out "calling table.put(w, yes) ..." + newline;
  table.put(w, yes);
      out "... table.put(w, yes) is ok ..." + newline + newline;

// Add a C to the set
// (mvm: object has not been initialized!)

  w = new C("C");
      out "w.toString() : " + w.toString() + newline;
      out "w.hashCode() : " + w.hashCode().toString() + newline;
      out "calling table.put(w, yes) ..." + newline;
  table.put(w, yes);
      out "... table.put(w, yes) is ok ..." + newline + newline;

  return 0;
}

// ---- Dummy classes which resemble Grammar hierarchy -- //

// A <---+---- B
//       |
//       +---- C
//
// A : in my solution is GrammarSymbol
// B : in my solution is NonterminalSymbol
// C : in my solution is TerminalSymbol


// The super class
class A
{
    String name;
 
    A(String n) { name = n; }

    // Reference equality for the dummy hierarchy
    Integer equals(Object obj) { return this == obj; }

    // Delegate toString to name
    String toString() { return name; }

    // Delegate hashCode to name
    Integer hashCode() { return name.hashCode(); }
}

// B explicity delegates hashCode to super.hashCode()
class B extends A
{
    B(String n) { super(n); }

    Integer hashCode()
    {
      out "In B.hashCode()"+newline;
      out "  > super.toString(): " + newline;
      out "    : " + super.toString() + newline;
      out "  > super.hashCode(): " + newline;
      out "    : " + super.hashCode().toString() + newline;

      return super.hashCode();
    }
}

// C inherits hashCode from A
class C extends A
{
    C(String n) { super(n); }
}

