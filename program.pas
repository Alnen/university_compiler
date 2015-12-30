Program test;
type mytype = integer;
   month    = array[10, 10] of integer;
   months   = (JUN, FEB, DEC);

var Zero    : integer;	
   MasOfInt : array [10] of integer;
   Year	    : months;
   
operator *(a: mytype; b: integer) c: integer;
begin
      c  := a + b;
end;

label lab1,lab2,lab3;

var a	: integer;
      b	: mytype;
   d	: rational;
      x	: complex;
   i, z	: integer;
begin
      for i := 0 to 10 do
      begin;
            for z := 0 to 10 do
            begin;
                  a := i;
            end;
      end;
      a := 5;
      b := 7;
      a := b * a;
      d := 1.3;
      b := d.denominator;
      x := {13, 2}
end.
