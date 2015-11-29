program while;

var b: boolean;
var i: Integer;

begin
    i := 0;

    while i <= 10 do
        i := i + 1;

    b := true;

    while b do
    begin
        writeBool(b);
        b := false
    end;

    printboolean(b)
end.