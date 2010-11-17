// Allen Hubbe
// CS 812 Fall 2010

// Homework 2

// ---- Utility Classes --------------------------------- //

// An immutable singly-linked list-node
// (null is the empty list)
class List
{
    // The element at the head of the list
    Object head;
    // The rest of the list
    List tail;

    // Construct a ListNode
    List(Object hd, List tl)
    {
      head = hd;
      tail = tl;
    }

    // Get the head of the list
    Object hd()
    {
      return head;
    }

    // Get the tail of the list
    List tl()
    {
      return tail;
    }
}

// ---- Grammar Symbols --------------------------------- //

// Represents a grammar symbol
class GrammarSymbol
{
    // The name of the symbol
    String name;

    // Construct a grammar symbol with name
    GrammarSymbol(String n)
    {
      name = n;
    }

    // Object Methods
    Integer equals(Object obj)
    {
      if(this == obj) {
        return 1;
      }
      else
      {
        if(obj instanceof GrammarSymbol)
        {
          return name.equals(obj.toString());
        }
        else
        {
          return 0;
        }
      }
    }
    Integer hashCode()
    {
      return name.hashCode();
    }
    String toString()
    {
      return name;
    }
}

// A nonterminal grammar symbol
class NonterminalSymbol extends GrammarSymbol
{
    // Construct a nonterminal symbol with name
    NonterminalSymbol(String n)
    {
      super(n);
    }

    // Object Methods
    Integer equals(Object obj)
    {
      if(this == obj) {
        return 1;
      }
      else
      {
        if(obj instanceof NonterminalSymbol)
        {
          return super.equals(obj);
        }
        else
        {
          return 0;
        }
      }
    }
}

// A terminal grammar symbol
class TerminalSymbol extends GrammarSymbol
{
    // Construct a terminal symbol with name
    TerminalSymbol(String n)
    {
      super(n);
    }

    // Object Methods
    Integer equals(Object obj)
    {
      if(this == obj) {
        return 1;
      }
      else
      {
        if(obj instanceof TerminalSymbol)
        {
          return super.equals(obj);
        }
        else
        {
          return 0;
        }
      }
    }
}

// ---- Productions ------------------------------------- //

// Represents a production
class Production
{
    // Nonterminal symbol on the left hand side
    NonterminalSymbol left;
    // List of grammar symbols on the right hand side
    List rights;

    // Construct a production
    Production(NonterminalSymbol lhs, List rhs)
    {
      left = lhs;
      rights = rhs;
    }

    // Get the left hand side
    NonterminalSymbol lhs()
    {
      return left;
    }

    // Get the right hand side
    List rhs()
    {
      return rights;
    }

    // Object Methods
    String toString()
    {
      String s;
      List rs;
      
      s = left.toString();
      rs = rights;

      while(!(rs == null))
      {
        s = s + " " + rs.hd().toString();
        rs = rs.tl();
      }
      return s;
    }
}

// ---- Grammar ----------------------------------------- //

// Represents a grammar
class Grammar
{
    // Nonterminal symbol that is the start symbol
    NonterminalSymbol start;
    // Set of nonterminal symbols
    Table nonterminals;
    // Set of terminal symbols
    Table terminals;
    // Table of token -> symbol mappings
    Table symbols;
    // List of productions
    List productions;
    // set of null-deriving nonterminal symbols
    Table nullderiving;

    // Construct an empty, uninitialized grammar
    Grammar()
    {
      start = null;
      nonterminals = new Table();
      terminals = new Table();
      symbols = new Table();
      productions = null;
      nullderiving = new Table();
    }

    // Initialize a Grammar by reading a context-free
    // grammar from the input.  Each call to initialize
    // reads one line.  This is recursive; the production
    // list is built when returning from the recursion.
    // (always returns null)
    Object initialize()
    {
      List statements;

      statements = read_statement_list();
      start = init_nonterminal((String)((List)statements.hd()).hd());
      productions = init_productions(statements);
      init_nullderiving();

      return null;
    }

    // Output a Grammar:
    // 1) the start symbol
    //    on one line, followed by a blank line
    // 2) the set of nonterminal symbols, separated by spaces
    //    on one line, followed by a blank line
    // 3) the set of terminal symbols, separated by spaces
    //    on one line, followed by a blank line
    // 4) the list of productions
    //    left hand symbol first, all symbols separated by spaces
    //    one production per line
    //    followed by a blank line
    // 5) the set of null-deriving non-terminals
    //    on one line
    // (always returns null)
    Object output()
    {
      out start.toString();
      out newline + newline;
      output_set(nonterminals, " ");
      out newline + newline;
      output_set(terminals, " ");
      out newline + newline;
      output_list(productions, newline);
      out newline + newline;
      output_set(nullderiving, " ");
      out newline;
      return null;
    }

    // PRIVATE
    // read tokens as a list until ";" (and omit it)
    List read_statement()
    {
      String token;
      token = in;

      if(!(token == null))
      {
        if(!(token.equals(";")))
        {
          return new List(token, read_statement());
        }
      }
      return null;
    }

    // PRIVATE
    // read statements until the input is exhausted
    List read_statement_list()
    {
      String token;
      token = in;

      if(!(token == null))
      {
        return new List(new List(token, read_statement()),
                        read_statement_list());
      }
      return null;
    }

    // PRIVATE
    // transform statements into a production list
    // 1) registers all nonterminals going into recursion
    // 2) registers all remaining symbols coming out of recursion
    //    and constructs the productions.
    List init_productions(List statements)
    {
      if(statements == null)
      {
        return null;
      }
      else
      {
        NonterminalSymbol lhs;
        List rhs;
        List tail;

        lhs = init_nonterminal((String)((List)statements.hd()).hd());
        tail = init_productions(statements.tl());
        rhs = init_symbol_list(((List)statements.hd()).tl());
        
        return new List(new Production(lhs, rhs), tail);
      }
    }

    // PRIVATE
    // register the token as a nonterminal symbol
    // (returns the instance of the symbol)
    NonterminalSymbol init_nonterminal(String token)
    {
      NonterminalSymbol symbol, tmpsymbol;

      tmpsymbol = new NonterminalSymbol(token);
      symbol = (NonterminalSymbol)nonterminals.get(tmpsymbol);
      if(symbol == null)
      {
        symbol = tmpsymbol;
        nonterminals.put(symbol, symbol);
      }
      symbols.put(token, symbol);

      return symbol;
    }
      
    // PRIVATE
    // register the token as a terminal symbol if it is not
    // already registered as any symbol type.
    GrammarSymbol init_symbol(String token)
    {
      GrammarSymbol symbol;

      symbol = (GrammarSymbol)symbols.get(token);
      if(symbol == null)
      {
        symbol = new TerminalSymbol(token);
        terminals.put(symbol, symbol);
        symbols.put(token, symbol);
      }

      return symbol;
    }

    // PRIVATE
    // transform a list of tokens to symbols using init_symbol
    List init_symbol_list(List tokens)
    {
      if(tokens == null)
      {
        return null;
      }
      else
      {
        return new List(init_symbol((String)tokens.hd()),
                        init_symbol_list(tokens.tl()));
      }
    }

    // PRIVATE
    // compute the set of null-deriving nonterminals
    // (always returns null)
    Object init_nullderiving()
    {
      List p_current;
      List g_current;
      Production prod;
      Object repeat;
      Object reject;

      Object true;
      Object false;
      true = new Object();
      false = new Object();

      repeat = true;
      while(repeat == true)
      {
        repeat = false;

        p_current = productions;
        while(!(p_current == null))
        {
          prod = (Production)p_current.hd();

          if(nullderiving.get(prod.lhs()) == null)
          {

            g_current = prod.rhs();
            reject = false;
            while(!(g_current == null))
            {
              if(nullderiving.get(g_current.hd()) == null)
              {
                reject = true;
                break;
              }
              g_current = g_current.tl();
            }

            if(reject == false)
            {
              nullderiving.put(prod.lhs(), prod.lhs());
              repeat = true;
            }

          }

          p_current = p_current.tl();
        }
      }

      return null;
    }

    // PRIVATE
    // output a set using toString and a separator
    // (always returns null)
    Object output_set(Table set, String separator)
    {
      Object o;

      set.firstKey();
      o = set.nextKey();
      if(!(o == null))
      {
        out o.toString();
        o = set.nextKey();
      }

      while(!(o == null))
      {
        out separator;
        out o.toString();
        o = set.nextKey();
      }

      return null;
    }

    // PRIVATE
    // output a list using toString and a separator
    // (always returns null)
    Object output_list(List list, String separator)
    {
      List l;

      l = list;
      if(!(l == null))
      {
        out l.hd().toString();
        l = l.tl();
      }

      while(!(l == null))
      {
        out separator;
        out l.hd().toString();
        l = l.tl();
      }

      return null;
    }
}

// ---- Main Block -------------------------------------- //

// Reads and then outputs a grammar
Integer main()
{
  Grammar g;

  g = new Grammar();
  
  g.initialize();
  g.output();
}

