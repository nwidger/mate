// Ken Graf: CS812 sample maTe paren error
// paren'd assignment in 2nd out causes mvm stack overflow
Integer main() {
  String s1, s2;
  s1="1"; s2="2";
  out "(s1=s2)=2? ";
  out (s1 = s2);
  out newline;
}

