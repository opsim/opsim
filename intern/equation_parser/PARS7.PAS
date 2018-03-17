unit pars7;
{$O+,F+}
interface

uses builder,pars7glb,realtype;

type

PParse = ^OParse;

OParse = object
  fstring:string;
  px,py,pt,pa,pb,pc,pd,pe: rpointer;
  numop:integer;
  fop:operationpointer;
  constructor init(s:string; showprogress: boolean; var error:boolean);
  procedure setparams(a,b,c,d,e:float);
  procedure f(x,y,t:float;var r:float);
  destructor done;
end;

implementation

var lastop:operationpointer;


procedure mynothing;
begin
end;

procedure mysum;
begin
  lastop^.dest^:=lastop^.arg1^+lastop^.arg2^;
end;

procedure mydiff;
begin
  with lastop^ do
     dest^:=arg1^-arg2^;
end;

procedure myprod;
begin
  with lastop^ do
     dest^:=arg1^*arg2^;
end;

procedure mydivis;
begin
  with lastop^ do
     dest^:=arg1^/arg2^;
end;

procedure myminus;
begin
  with lastop^ do
     dest^:=-arg1^;
end;

procedure myintpower;
var n,i:longint;
begin
  with lastop^ do
  begin
    n:=trunc(abs(arg2^))-1;
    case n of
    -1: dest^:=1;
     0: dest^:=arg1^;
    else
    begin
      dest^:=arg1^;
      for i:=1 to n do
       dest^:=dest^*arg1^;
    end;
   end;
  if arg2^<0 then dest^:=1/dest^;
 end;
end;

procedure mysquare;
begin
  with lastop^ do
    dest^:=sqr(arg1^);
end;

procedure mythird;
begin
  with lastop^ do
    dest^:=arg1^*arg1^*arg1^;
end;

procedure myforth;
begin
  with lastop^ do
    dest^:=sqr(sqr(arg1^));
end;

procedure myrealpower;
begin;
  with lastop^ do
    dest^:=exp(arg2^*ln(arg1^));
end;

procedure mycos;
begin
  with lastop^ do
    dest^:=cos(arg1^);
end;

procedure mysin;
begin
  with lastop^ do
    dest^:=sin(arg1^);
end;

procedure myexp;
begin
  with lastop^ do
    dest^:=exp(arg1^);
end;

procedure myln;
begin
  with lastop^ do
    dest^:=ln(arg1^);
end;

procedure mysqrt;
begin
  with lastop^ do
    dest^:=sqrt(arg1^);
end;

procedure myarctan;
begin
  with lastop^ do
    dest^:=arctan(arg1^);
end;

procedure myabs;
begin
  with lastop^ do
    dest^:=abs(arg1^);
end;

procedure mymin;
begin
  with lastop^ do
    if arg1^<arg2^ then dest^:=arg1^ else dest^:=arg2^;
end;

procedure mymax;
begin
  with lastop^ do
    if arg1^<arg2^ then dest^:=arg2^ else dest^:=arg1^;
end;

procedure myheavi;
begin
  with lastop^ do
    if arg1^<0 then dest^:=0 else dest^:=1;
end;


procedure myphase;
var a:float;
begin
  with lastop^ do
  begin
    a:=arg1^/2/pi;
    dest^:=2*pi*(a-round(a));
  end;
end;

procedure myrand;
var j:word;
begin
  with lastop^ do
  begin
  j:=round(arg2^);
  if j=randomresult then dest^:=1 else dest^:=0;
  end;
end;

procedure myarg;
begin
  with lastop^ do
  if arg1^<0 then dest^:=arctan(arg2^/arg1^)+Pi else
  if arg1^>0 then dest^:=arctan(arg2^/arg1^) else if arg2^>0
  then dest^:=Pi/2 else dest^:=-Pi/2;
end;

procedure mycosh;
begin
  with lastop^ do
    dest^:=(exp(arg1^)+exp(-arg1^))/2;
end;

procedure mysinh;
begin
  with lastop^ do
    dest^:=(exp(arg1^)-exp(-arg1^))/2;
end;

procedure myradius;
begin
  with lastop^ do
    dest^:=sqrt(sqr(arg1^)+sqr(arg2^));
end;

procedure myrandrand;
begin
  with lastop^ do
  dest^:=arg1^+arg2^*contrandresult;
end;


{OParse}

constructor OParse.init(s:string; showprogress:boolean;var error:boolean);
var i:integer; lop:operationpointer;
begin
    fstring:=s;
    parsefunction(s,fop,px,py,pt,pa,pb,pc,pd,pe,numop,error,showprogress);
    lop:=fop;
    while lop<>nil do
    begin
      with lop^ do
      begin
        case opnum of
          0,1,2: op:=@mynothing;
          3: op:=@myminus;
          4: op:=@mysum;
          5: op:=@mydiff;
          6: op:=@myprod;
          7: op:=@mydivis;
          8: op:=@myintpower;
          9: op:=@myrealpower;
          10:op:=@mycos;
          11:op:=@mysin;
          12:op:=@myexp;
          13:op:=@myln;
          14:op:=@mysqrt;
          15:op:=@myarctan;
          16:op:=@mysquare;
          17:op:=@mythird;
          18:op:=@myforth;
          19:op:=@myabs;
          20:op:=@mymax;
          21:op:=@mymin;
          22:op:=@myheavi;
          23:op:=@myphase;
          24:op:=@myrand;
          25:op:=@myarg;
          26:op:=@mysinh;
          27:op:=@mycosh;
          28:op:=@myradius;
          29:op:=@myrandrand;
        end; {case}
      end; {with lop^}
      lop:=lop^.next
    end; {while lop<>nil}
end;

procedure OParse.setparams(a,b,c,d,e:float);
begin
  pa^:=a; pb^:=b; pc^:=c; pd^:=d; pe^:=e;
end;


procedure OParse.f(x,y,t:float;var r:float);
begin
    px^:=x; py^:=y; pt^:=t;
    lastop:=fop;
    while lastop^.next<>nil do
    begin
      lastop^.op;
      lastop:=lastop^.next;
    end;
    lastop^.op;
    r:=lastop^.dest^;
end;

destructor OParse.done;
var i,j:integer; lastop,nextop:operationpointer;
begin
  lastop:=fop;
  while lastop<>nil do
  begin
    nextop:=lastop^.next;
    while nextop<>nil do
    begin
          if nextop^.arg1 = lastop^.arg1 then nextop^.arg1:=nil;
          if nextop^.arg2 = lastop^.arg1 then nextop^.arg2:=nil;
          if nextop^.dest = lastop^.arg1 then nextop^.dest:=nil;
          if nextop^.arg1 = lastop^.arg2 then nextop^.arg1:=nil;
          if nextop^.arg2 = lastop^.arg2 then nextop^.arg2:=nil;
          if nextop^.dest = lastop^.arg2 then nextop^.dest:=nil;
          if nextop^.arg1 = lastop^.dest then nextop^.arg1:=nil;
          if nextop^.arg2 = lastop^.dest then nextop^.arg2:=nil;
          if nextop^.dest = lastop^.dest then nextop^.dest:=nil;
          nextop:=nextop^.next;
    end;
    with lastop^ do
    begin
      if (arg1=px) or (arg1=py) or (arg1=pt) or (arg1=pa) or
      (arg1=pb) or (arg1=pc) or (arg1=pd) or (arg1=pe) then arg1:=nil;
      if (arg2=px) or (arg2=py) or (arg2=pt) or (arg2=pa) or
      (arg2=pb) or (arg2=pc) or (arg2=pd) or (arg2=pe) then arg2:=nil;
      if (dest=px) or (dest=py) or (dest=pt) or (dest=pa) or
      (dest=pb) or (dest=pc) or (dest=pd) or (dest=pe) then dest:=nil;
      if arg1<>nil then dispose(arg1);
      if arg2<>nil then dispose(arg2);
      if dest<>nil then dispose(dest);
    end;
    nextop:=lastop^.next;
    dispose(lastop);
    lastop:=nextop;
  end;
  dispose(px); dispose(py); dispose(pt);
  dispose(pa); dispose(pb); dispose(pc);
  dispose(pd); dispose(pe);
end;



end.
