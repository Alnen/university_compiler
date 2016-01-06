Program test;
type mytype = integer;

var massive_of_int : array [10, 10] of integer;
    massive	   : array [15] of integer;
    i, z	   : integer;
    mas		   : array [10] of integer;
    k		   : integer;
   
begin
      for i := 0 to 10 do
      begin
            for z := 0 to 10 do
            begin
                 massive_of_int[i][z] := 0;
            end;
      end;
   
      for i := 1 to 10 do
      begin
            for z := 0 to 10 do
            begin
		 write((((mas[0]/2) = 0) or (mas[0] <> 0)) and 1);
	    end;
      end;

      for i := 0 to 10 do
      begin
            for z := 0 to 10 do
            begin
	          write(massive[5] * 5);
	          write((massive_of_int[i][z] + 1 + 3 - massive[5])*3);
	    end;
      end;
end.
