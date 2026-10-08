program Testes;
var
  a, b : integer;
  x : real;
  c : char;
begin
  a := 10;
  b := 3;
  x := 10.5 / 2.5;
  c := '\t';
  if a >= b and a <> 0 then
    write(c);
  else
    write('n');
  repeat
    a := a - 1;
  until a <= 0;
end.
