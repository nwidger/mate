class SymbolTable extends Table {
    Integer size;

    SymbolTable() {
        super();
        size = 0;
    }

    SymbolTable(Integer n) {
        super(n);
        size = 0;
    }

    String put(String s) {
        size = size + 1;
        return (String)put((Object)size, (Object)s);
    }

    Integer dump() {
        Integer index;
        String symbol;

        if (firstKey() == 0) {
            out "Error dumping symbol table!" + newline;
            return 1;
        }

        index = (Integer)nextKey();
        while (!(index == null)) {
            symbol = (String)get((Object)index);
            if (symbol == null) {
                out "Error dumping symbol table!" + newline;
                return 1;
            }

            out symbol + newline;
            index = (Integer)nextKey();
        }

        return 0;
    }
}

Integer main() {
    String s, symbol;
    Integer len, address;
    SymbolTable symbolTable;

    symbolTable = new SymbolTable();

    s = in;
    while (!(s == null)) {
        len = s.length();
        if (s.substr(len-1, len-1).equals(":")) {
            symbolTable.put(s.substr(0, len-2));
        }

        s = in;
    }

    symbolTable.dump();
    return 0;
}
