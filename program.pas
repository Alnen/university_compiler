program Maldebrod;
var 
    field: array[5, 5, 2] of integer;
	limit, i, z, k, cr, ci, tmp, DEPTH, tmp2 : integer;

label f_end;
begin
	limit := 0;
	write("Enter limit: ");
	read(limit);
	write("Start ", limit);
	write("");
	DEPTH := 3;
	cr := 2;
	ci := 4;
	for i := 0 to 4 do
	begin
		for z := 0 to 4 do
		begin
		    write("Init ", i);
		    write(" ", z);
		    write(" ", i*5+z);
		    write(" ", limit);
		    write("");
			field[i][z][0] := z*5 - 10;
			field[i][z][1] := i*5 - 10;
		end;
	end;
	for i := 0 to 4 do
	begin
		for z := 0 to 4 do
		begin
			for k := 0 to DEPTH-1 do
			begin
		  		tmp := field[i][z][0]*field[i][z][0] - field[i][z][1]*field[i][z][1];
		  		field[i][z][1]  := 2*field[i][z][0]*field[i][z][1] + ci;
		  		field[i][z][0]  := tmp + cr;
		  		if ((field[i][z][0]*field[i][z][0] + field[i][z][1]*field[i][z][1]) > limit)  then
				begin
					write("L ", (field[i][z][0]*field[i][z][0] + field[i][z][1]*field[i][z][1]));
					write(" R ", limit);
		   			write("+");
		   			goto f_end;
				end;
			end;
			write("*");
			f_end:
		end;
		write("");
	end;
	write("");
    read(i);
end.
