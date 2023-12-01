int x,y,A[100];

void helloWorld(void)
  begin
    write "Hello world!";
    write "\n";
  end

int f(int a, int b)
  begin
    int p;
    if (a > b)
      then return a;
      else return b;
    endif
    return p;
  end

  void arrayFunction(int length)
    begin
      int i;
      i = 0;

      while (i < length)
        do
          begin
            write "A[";
            write i;
            write "] = ";
            write A[i];
            write "\n";
            i = i + 1;
          end
    end

  void main(void)
    begin
      //Local var decs
      boolean p, q;
      int i;

      p = true;
      q = false;

      write "Enter an integer(type '0' to quit): ";
      read x;

      i = 0;

      while (x != 0)
        do
          begin
            A[i] = x;
            i = i + 1;
            write "Enter an integer: ";
            read x;
          end

      arrayFunction(i);

      if i > 1
        then 
          begin
            write f(A[i-1], A[i-2]);
            write "\n";
            write f(A[0], A[i-1]);
            write "\n";
          end
      endif

    write "Testing conditionals";
    write "\n";

    write p;
    write "\n";
    write q;
    write "\n";

    write "Testing NOT";
    write "\n";

    write not p;
    write "\n";
    write not q;
    write "\n";

    write "Testing AND and OR";
    write "\n";

    write p and q;
    write "\n";
    write p or q;
    write "\n";
    write p and p;
    write "\n";
    write q or q;
    write "\n";
    write p and not p;
    write "\n";

    helloWorld();
    
    end