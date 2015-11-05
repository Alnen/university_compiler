Program test;
type mytype = integer;
operator *(a: mytype; b: integer) c: integer;
begin
      c  := a + b;
end;
var a : integer;
      b : mytype;
      d : rational;
      x: complex;
begin
      a := 5;
      b := 7;
      a := b * a;
      d := 1.3;
      b := d.denominator;
      x := {13, 2}
end.
