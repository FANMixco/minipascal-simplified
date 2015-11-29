program if_stmt;

var i, k: integer;
var bar : boolean;

procedure test ();
var j : integer;
var foo : boolean;
begin
    i := 10;
    j := 3;

    writeInt(j);

    j := 77;

    writeInt(j);

    j := j * 77;

    writeInt(j);

    foo := false;

    if foo then
        printChar('t');

    foo := true;

    if foo then
        printChar('t');

    foo := false and true;

    if foo then
        printChar('t');


    foo := true and false;
    foo := true or foo;
    foo := j > 2 * 2;
    foo := 3 < 4;
    foo := 5 >= 6;
    foo := 7 <= 8;
    foo := j = i;
    foo := j <> i
end;

begin
    bar := true;
    i := 17;

    if ((1 = 1) or (2 = 3)) and (bar = false) then
    begin
        printchar('t');
        if bar then
            printChar ('B')
    end;

    test();

    if (i = k) then if false then
        writeInt(i);

    test();

    if bar then
        printChar('b');

    println()
end.
