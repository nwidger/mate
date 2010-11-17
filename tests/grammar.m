class Symbol {
  String name;

  Symbol(String n) {
    name = n;
  }

  Integer equals(Object obj) {
    if (obj instanceof Symbol)
      return name.equals(((Symbol)obj).name);
    return 0;
  }

  Integer hashCode() {
    return name.hashCode();
  }

  String toString() {
    return name;
  }
}

class NonterminalSymbol extends Symbol {
  NonterminalSymbol(String n) {
    super(n);
  }
}

class TerminalSymbol extends Symbol {
  TerminalSymbol(String n) {
    super(n);
  }
}

class Production {
  NonterminalSymbol lhs;
  Table symbols;

  Production(NonterminalSymbol l) {
    lhs = l;
    symbols = new Table();
  }
    
  Symbol addSymbol(Symbol s) {
    symbols.put((Object)s, (Object)s);
    return s;
  }

  String toString() {
    Integer i;
    Symbol symbol;
    String retval;

    retval = lhs.toString();

    symbols.firstKey();
    while (!((symbol = (Symbol)symbols.nextKey()) == null)) {
      retval = retval + " " + symbol.toString();
    }

    return retval;
  }
}

class Grammar {
  Integer tokensSize;
  Table tokens;

  NonterminalSymbol start;
    
  Table nonterminals;
  Table terminals;
  Table productions;

  Grammar() {
    tokensSize = 0;
    tokens = new Table();
    start = null;
    nonterminals = new Table();
    terminals = new Table();
    productions = new Table();
  }

  Integer input() {
    Integer i, j;
    String token;
    Production p;	
    NonterminalSymbol nt;
    TerminalSymbol t;

    token = in;

    while (!(token == null)) {
      addToken(token);
      token = in;
    }

    // loop over input once to get LHS of productions
    // and list of nonterminals
    i = 0;
    while (i < tokensSize) {
      token = getToken(i);

      nt = new NonterminalSymbol(token);

      if (i.equals(0)) {
	start = nt;
      }

      p = new Production(nt);
      addNonterminalSymbol(nt);

      while (!(getToken(i).equals(";"))) {
	i = i + 1;
      }
      
      i = i + 1;
    }

    // loop over input again to add RHS to each production
    i = 1;
    while (i < tokensSize) {
      token = getToken(i);

      if (getNonterminalSymbol(new NonterminalSymbol(token)) == null)
	addTerminalSymbol(new TerminalSymbol(token));
      
      i = i + 1;
    }
    
    return 0;
  }

  Integer output() {
    Integer i;
    NonterminalSymbol nt;
    TerminalSymbol t;
    Production p;

    // start symbol
    out start.toString() + newline;
    out newline;

    // nonterminals
    nonterminals.firstKey();
    while (!((nt = (NonterminalSymbol)nonterminals.nextKey()) == null)) {
      out nt.toString() + " ";
    }

    out newline;
    out newline;

    // terminals
    terminals.firstKey();
    while (!((t = (TerminalSymbol)terminals.nextKey()) == null)) {
      out t.toString() + " ";
    }
	
    out newline;
    out newline;

    // productions
    productions.firstKey();
    while (!((p = (Production)productions.nextKey()) == null)) {
      out p.toString() + " ";
    }
	
    return 0;
  }

  Integer outputNullDeriving() {
    return 0;
  }

  String addToken(String t) {
    tokensSize = tokensSize + 1;
    tokens.put((Object)t, (Object)t);
    return t;
  }

  String getToken(Integer i) {
    return (String)tokens.get((Object)i);
  }

  NonterminalSymbol addNonterminalSymbol(NonterminalSymbol n) {
    nonterminals.put((Object)n, (Object)n);
    return n;
  }

  NonterminalSymbol getNonterminalSymbol(NonterminalSymbol n) {
    return (NonterminalSymbol)nonterminals.get((Object)n);
  }

  TerminalSymbol addTerminalSymbol(TerminalSymbol t) {
    terminals.put((Object)t, (Object)t);
    return t;
  }

  Production addProduction(Production p) {
    productions.put((Object)p, (Object)p);
    return p;
  }
}

Integer main() {
  Grammar grammar;
    
  grammar = new Grammar();
    
  grammar.input();
  grammar.output();
  grammar.outputNullDeriving();
    
  return 0;
}
