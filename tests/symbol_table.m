class SymbolTable {
    Integer size;
    Table table;

    SymbolTable() {
        size = 0;
        table = new Table();
    }

    SymbolTable(Integer n) {
        size = 0;
        table = new Table(n);
    }

    Integer addSymbol(String s, Integer i) {
        if (!(table.get((Object)s) == null)) {
            out "Duplicate symbol " + s + "!" + newline;
            return null;
        }

        size = size + 1;
        return (Integer)table.put((Object)s, (Object)i);
    }

    Integer removeSymbol(String s) {
        Integer retval;

        retval = (Integer)table.remove((Object)s);
        if (!(retval == null))
            size = size - 1;

        return retval;
    }

    Integer findSymbol(String s) {
        return (Integer)table.get((Object)s);
    }

    Integer dump() {
        String symbol;
        Integer address;

        if (table.firstKey() == 0) {
            out "Error dumping symbol table!" + newline;
            return 1;
        }

        //out size.toString() + newline;

        symbol = (String)table.nextKey();
        while (!(symbol == null)) {
            address = (Integer)table.get((Object)symbol);
            if (address == null) {
                out "Error dumping symbol table!" + newline;
                return 1;
            }

            out address.toString() + " " + symbol + newline;

            symbol = (String)table.nextKey();
        }

        return 0;
    }
}

Integer main() {
    String s, symbol, command;
    Integer size, address;
    SymbolTable symbolTable;

    //s = in;
    //if (s == null) {
    //    out "Error reading symbol table size!" + newline;
    //    return 1;
    //}

    //size = s.toInteger();
    //symbolTable = new SymbolTable(size);
    symbolTable = new SymbolTable();

    //while (1) {
    //    s = in;
    //    if (s == null)
    //        break;

    //    address = s.toInteger();

    //    s = in;
    //    if (s == null) {
    //        out "Error reading address!" + newline;
    //        return 1;
    //    }

    //    symbol = s;	

    //    symbolTable.addSymbol(symbol, address);
    //}

    while (1) {
        out "?> ";
        command = in;
        if (command == null)
            break;

        if (command.equals("exit")) {
            break;
        } else if (command.equals("help")) {
            out "dump" + newline;
            out "add [symbol] [address]" + newline;
            out "remove [symbol]" + newline;
            out "find [symbol]" + newline;
            out "help" + newline;
            out "exit" + newline;
        } else if (command.equals("dump")) {
            symbolTable.dump();
        } else if (command.equals("add")) {
            symbol = in;
            if (symbol == null)
                break;

            s = in;
            if (s == null)
                break;

            address = s.toInteger();
            symbolTable.addSymbol(symbol, address);

            out address.toString() + " " + symbol + newline;
        } else if (command.equals("remove")) {
            symbol = in;
            if (symbol == null)
                break;

            symbolTable.removeSymbol(symbol);
        } else if (command.equals("find")) {
            symbol = in;
            if (symbol == null)
                break;

            s = in;
            if (s == null)
                break;

            address = s.toInteger();
            address = symbolTable.findSymbol(symbol);

            if (!(address == null))
                out address.toString() + " " + symbol + newline;
        } else {
            out command + ": unknown command!" + newline;
        }
    }

    out newline;

    return 0;
}
