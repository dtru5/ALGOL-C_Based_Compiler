void p(void)
begin
  write "\n";
  write "hello";
  write "\n";
end


int f(int x, int y)
begin
  x = x + y;
  write x;
  write "\n";
  return x + y;
end


void main(void)
begin
  int x;
  x = 4;
  write f(2, 4+2+x);
  p();
end