Program test;
type mytype = integer;

var massive_of_int : array [10, 10] of integer;
    massive	   : array [15] of integer;
    i, z	   : integer;
    mas		   : array [10] of integer;
    k		   : integer;

label exit;
   
begin
      if (k = 10) then
	    begin
	       if (z < 5) then goto exit;
	       i := 1;
	    end
      else
	    begin
	       write(z);
	       z := 1;
	    end;
   exit:mas[5] := 1;
end.
