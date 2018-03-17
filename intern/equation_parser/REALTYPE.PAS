unit realtype;
interface
type float=double;
     dfloat=extended;

function max(x,y:float):float;

function min(x,y:float):float;

implementation

function max(x,y:float):float;
begin
 if x<y then max:=y else max:=x;
end;

function min(x,y:float):float;
begin
 if x<y then min:=x else min:=y;
end;
end.
