program Proc_Func;

var b: boolean;
var c: char;
var i, j: integer;

procedure procExample ();
var mp : boolean;
begin
  b := false;
  mp := not b;
  writeBool(mp);
  writeLn()
end;

function funcExample (a : integer, b : integer) : integer;
var mf1, mf2 : boolean;
var mf3 : boolean;
begin
    mf1 := true;
    myfunc := a + b
end;

begin
    c := 'z';
    i := 13;
    j := 0;
    b := not false or (13 <> j);

    procExample();
    j := funcExample(false, i);
    writeInt(j);

    i := 70 + 80 * 5 + 3;
    i := 1 * 2 + 3 * 4
end.