class NewString extends String {
    NewString(Integer n) {
        super("");
    }
}

Integer main() {
    NewString n;

    n = new NewString(5);

    out n + newline;
}
