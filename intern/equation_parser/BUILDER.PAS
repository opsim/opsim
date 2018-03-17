unit builder;
{$O+,F+}
interface
uses realtype,pars7glb;

procedure parsefunction(s:string;var fop:operationpointer;
            var pointx,pointy,pointt,a,b,c,d,e:rpointer;var numop:integer;
           var error:boolean; showprogress:boolean);
implementation

type sstring=string;

     termsorttype=(variab,constant,brack,minus,sum,diff,prod,divis,
                    intpower,realpower,cosine,sine,expo,logar,sqroot,arctang,
                    square,third,forth,abso,maxim,minim,heavi,
                    phase,randfunc,argu,hypersine,hypercosine,radius,
                    randrand);

procedure chopblanks(var s:sstring);  forward;
{deletes all blanks in s}

procedure checkbracketnum(s:sstring; var result:boolean); forward;
{checks whether # of '(' equ. # of ')'}

procedure checknum(s:sstring;var num:float;var result:boolean); forward;
{checks whether s is a number}

procedure checkvar(s:sstring;var varsort:word;var result:boolean); forward;
{checks whether s is a variable string}

procedure checkparam(s:sstring;var parsort:word;var result:boolean); forward;
{checks whether s is a parameter string}

procedure checkbrack(s:sstring;var s1:sstring;var result:boolean); forward;
{checks whether s =(...(s1)...) and s1 is a valid term}

procedure checkmin(s:sstring;var s1:sstring;var result:boolean); forward;
{checks whether s denotes the negative value of a valid operation}

procedure checksum(s:sstring;var s1,s2:sstring;var result:boolean); forward;
{checks whether '+' is the primary operation in s}

procedure checkdiff(s:sstring;var s1,s2:sstring;var result:boolean); forward;
{checks whether '-' is the primary operation in s}

procedure checkprod(s:sstring;var s1,s2:sstring;var result:boolean); forward;
{checks whether '*' is the primary operation in s}

procedure checkdiv(s:sstring;var s1,s2:sstring;var result:boolean);  forward;
{checks whether '/' is the primary operation in s}

procedure check2varfunct(s:sstring;var s1,s2:sstring;var fsort:
    termsorttype;var result:boolean);  forward;
{checks whether s=f(s1,s2); s1,s2 being valid terms}

procedure checkfunct(s:sstring;var s1:sstring;var fsort:termsorttype;
var result:boolean); forward;
{checks whether s denotes the evaluation of a function fsort(s1)}

procedure checkintpower(s:sstring;var s1,s2:sstring;var result:boolean); forward;
{checks whether s=s1^s2, s2 integer}

procedure checkrealpower(s:sstring;var s1,s2:sstring;var result:boolean); forward;
{checks whether s=s1^s2, s2 real}

procedure chopblanks(var s:sstring);
var i:byte;
begin
  while pos(' ',s)>0 do
  begin
    i:=pos(' ',s);
    delete(s,i,1);
  end;
end;

procedure checkbracketnum(s:sstring; var result:boolean);
var lauf,lzu,i:integer;
begin
  lauf:=0;lzu:=0;i:=0;
  result:=false;
  repeat
    i:=i+1;
    if copy(s,i,1)='(' then
      lauf:=lauf+1;
    if copy(s,i,1)=')' then
      lzu:=lzu+1;
  until i>=length(s);
  if lauf=lzu then
    result:=true;
end;

procedure checknum(s:sstring;var num:float;var result:boolean);
var code,p,i:integer;  n:longint; num1:float;  s1,s2:sstring;
begin
  result:=false;
  if s='Pi' then
  begin
    result:=true;
    num:=Pi;
    exit;
  end
  else
  begin
    val(s,num,code);
    if code=0 then
      result:=true;
  end;
end;

procedure checkparam(s:sstring; var parsort:word; var result:boolean);
begin
  result:=false;
  if length(s)<>1 then exit else
  begin
    if s='A' then begin
      result:=true; parsort:=1; exit; end;
    if s='B' then begin
      result:=true; parsort:=2; exit; end;
    if s='C' then begin
      result:=true; parsort:=3; exit; end;
    if s='D' then begin
      result:=true; parsort:=4; exit; end;
    if s='E' then begin
      result:=true; parsort:=5; exit; end;
  end;
end;


procedure checkvar(s:sstring;var varsort:word;var result:boolean);
begin
  result:=false;
  if length(s)<>1 then exit else
  begin
    if s='x' then
    begin
      result:=true;
      varsort:=1;
      exit;
    end;
    if s='y' then
    begin
      result:=true;
      varsort:=2;
      exit;
    end;
    if s='t' then
    begin
      result:=true;
      varsort:=3;
      exit;
    end;
  end;
end;



procedure checkbrack(s:sstring;var s1:sstring;var result:boolean);
var s2,s3:sstring;   num:float;  fsort:termsorttype; varsort:word;
begin
  result:=false;
  if copy(s,1,1)='(' then
    if copy(s,length(s),1)=')' then
    begin
      s1:=copy(s,2,length(s)-2);
      checksum(s1,s2,s3,result); if result then exit;
      checknum(s1,num,result); if result then exit;
      checkdiff(s1,s2,s3,result); if result then exit;
      checkmin(s1,s2,result);if result then exit;
      checkprod(s1,s2,s3,result);if result then exit;
      checkdiv(s1,s2,s3,result);if result then exit;
      check2varfunct(s1,s2,s3,fsort,result);if result then exit;
      checkfunct(s1,s2,fsort,result);if result then exit;
      checkvar(s1,varsort,result);if result then exit;
      checkparam(s1,varsort,result);if result then exit;
      checkintpower(s1,s2,s3,result);if result then exit;
      checkrealpower(s1,s2,s3,result);if result then exit;
      checkbrack(s1,s2,result);
      if result then begin s1:=s2;  exit; end;
    end;
end;

procedure checkmin(s:sstring;var s1:sstring;var result:boolean);
var s2,s3:sstring;  num:float;   fsort:termsorttype; varsort:word;
begin
  result:=false;
  if copy(s,1,1)='-' then
  begin
    s1:=copy(s,2,length(s)-1);
    checkbrack(s1,s2,result);
    if result then begin
      s1:=s2;  exit; end;
    checkvar(s1,varsort,result); if result then exit;
    checkparam(s1,varsort,result); if result then exit;
    checkfunct(s1,s2,fsort,result); if result then exit;
    check2varfunct(s1,s2,s3,fsort,result); if result then exit;
    checkintpower(s1,s2,s3,result); if result then exit;
    checkrealpower(s1,s2,s3,result); if result then exit;
  end;
end;

procedure checksum(s:sstring;var s1,s2:sstring;var result:boolean);
var s3,s4:sstring; i,j:byte; num:float;    fsort:termsorttype;varsort:word;
begin
  result:=false;
  i:=0;
  repeat
    j:=pos('+',copy(s,i+1,length(s)-i));
    if j>0 then
    begin
      i:=i+j;
      if (i<length(s)) and (i>1) then
      begin
        s1:=copy(s,1,i-1);  s2:=copy(s,i+1,length(s)-i);
        checkbracketnum(s1,result); if result then
          checkbracketnum(s2,result); if result then
        begin
          checkvar(s1,varsort,result);
          if not result then
          checknum(s1,num,result);
          if not result then
          checkparam(s1,varsort,result);
          if not result then
          begin
          checkbrack(s1,s3,result);
          if result then s1:=s3; end;
          if not result then
          checkmin(s1,s3,result);
          if not result then
          checkdiff(s1,s3,s4,result);
          if not result then
          checkprod(s1,s3,s4,result);
          if not result then
          checkdiv(s1,s3,s4,result);
          if not result then
          check2varfunct(s1,s3,s4,fsort,result);
          if not result then
          checkfunct(s1,s3,fsort,result);
          if not result then
          checkintpower(s1,s3,s4,result);
          if not result then
            checkrealpower(s1,s3,s4,result);
          if result then
          begin
            checkvar(s2,varsort,result); if result then exit;
              checknum(s2,num,result);if result then exit;
              checkparam(s2,varsort,result); if result then exit;
              checkbrack(s2,s3,result);
              if result then begin
                s2:=s3; exit; end;
              checksum(s2,s3,s4,result);if result then exit;
              checkdiff(s2,s3,s4,result);if result then exit;
              checkprod(s2,s3,s4,result);if result then exit;
              checkdiv(s2,s3,s4,result);if result then exit;
              checkfunct(s2,s3,fsort,result);if result then exit;
              check2varfunct(s2,s3,s4,fsort,result);if result then exit;
              checkintpower(s2,s3,s4,result);if result then exit;
              checkrealpower(s2,s3,s4,result);if result then exit;
          end;
        end;
      end;
    end;
  until result or (i>=length(s)) or (j=0);
end;

procedure checkdiff(s:sstring;var s1,s2:sstring;var result:boolean);
var s3,s4:sstring; i,j:integer;  num:float;   fsort:termsorttype;varsort:word;
begin
  result:=false;
  i:=0;
  repeat
    j:=pos('-',copy(s,i+1,length(s)-i));
    if j>0 then
    begin
    i:=i+j;
    if (i<length(s)) and (i>1) then
    begin
      s1:=copy(s,1,i-1);  s2:=copy(s,i+1,length(s)-i);
      checkbracketnum(s1,result);
      if result then
        checkbracketnum(s2,result);
      if result then
      begin
      checkvar(s1,varsort,result);
      if not result then
        checknum(s1,num,result);
      if not result then
        checkparam(s1,varsort,result);
      if not result then
      begin
        checkbrack(s1,s3,result);
        if result then
          s1:=s3;
      end;
      if not result then
        checkmin(s1,s3,result);
      if not result then
        checkdiff(s1,s3,s4,result);
      if not result then
        checkprod(s1,s3,s4,result);
      if not result then
        checkdiv(s1,s3,s4,result);
      if not result then
             check2varfunct(s1,s3,s4,fsort,result);
      if not result then
        checkfunct(s1,s3,fsort,result);
      if not result then
        checkintpower(s1,s3,s4,result);
      if not result then
        checkrealpower(s1,s3,s4,result);
      if result then
      begin
        checkvar(s2,varsort,result); if result then exit;
          checknum(s2,num,result);if result then exit;
          checkparam(s2,varsort,result);if result then exit;
          checkbrack(s2,s3,result);
          if result then  begin
            s2:=s3; exit; end;
          checkprod(s2,s3,s4,result);if result then exit;
          checkdiv(s2,s3,s4,result);if result then exit;
         checkfunct(s2,s3,fsort,result);if result then exit;
          check2varfunct(s2,s3,s4,fsort,result);if result then exit;
         checkintpower(s2,s3,s4,result);if result then exit;
         checkrealpower(s2,s3,s4,result);if result then exit;
        end;
      end;
    end;
    end;
  until result or (i>=length(s)) or (j=0);
end;

procedure checkprod(s:sstring;var s1,s2:sstring;var result:boolean);
var s3,s4:sstring; i,j:integer;    num:float;    fsort:termsorttype;varsort:word;
begin
  result:=false;
  i:=0;
  repeat
    j:=pos('*',copy(s,i+1,length(s)-i));
    if j>0 then
    begin
      i:=i+j;
    if (i<length(s)) and (i>1) then
    begin
      s1:=copy(s,1,i-1);  s2:=copy(s,i+1,length(s)-i);
      checkbracketnum(s1,result);
      if result then
        checkbracketnum(s2,result);
      if result then
      begin
      checkvar(s1,varsort,result);
      if not result then
        checknum(s1,num,result);
      if not result then
        checkparam(s1,varsort,result);
      if not result then
      begin
        checkbrack(s1,s3,result);
        if result then
            s1:=s3;
      end;
      if not result then
        checkmin(s1,s3,result);
      if not result then
        checkdiv(s1,s3,s4,result);
      if not result then
        checkfunct(s1,s3,fsort,result);
      if not result then
             check2varfunct(s1,s3,s4,fsort,result);
      if not result then
       checkintpower(s1,s3,s4,result);
      if not result then
       checkrealpower(s1,s3,s4,result);
      if result then
      begin
        checkvar(s2,varsort,result); if result then exit;
          checknum(s2,num,result);if result then exit;
          checkparam(s2,varsort,result); if result then exit;
          checkbrack(s2,s3,result);
          if result then begin
            s2:=s3; exit; end;
          checkprod(s2,s3,s4,result);if result then exit;
          checkdiv(s2,s3,s4,result);if result then exit;
         checkfunct(s2,s3,fsort,result);if result then exit;
         check2varfunct(s2,s3,s4,fsort,result);if result then exit;
         checkintpower(s2,s3,s4,result);if result then exit;
         checkrealpower(s2,s3,s4,result);if result then exit;
      end;
      end;
    end;
    end;
  until result or (i>=length(s)) or (j=0);
end;

procedure checkdiv(s:sstring;var s1,s2:sstring;var result:boolean);
var s3,s4:sstring; i,j:integer;  varsort:word; num:float; fsort:termsorttype;
begin
  result:=false;
  i:=0;
  repeat
    j:=pos('/',copy(s,i+1,length(s)-i));
    if j>0 then
    begin
      i:=i+j;
    if (i<length(s)) and (i>1) then
    begin
      s1:=copy(s,1,i-1);  s2:=copy(s,i+1,length(s)-i);
      checkbracketnum(s1,result);
      if result then
        checkbracketnum(s2,result);
      if result then
      begin
      checkvar(s1,varsort,result);
      if not result then
        checknum(s1,num,result);
      if not result then
        checkparam(s1,varsort,result);
      if not result then
      begin
        checkbrack(s1,s3,result);
        if result then
            s1:=s3;
      end;
      if not result then
        checkmin(s1,s3,result);
      if not result then
        checkdiv(s1,s3,s4,result);
      if not result then
         checkfunct(s1,s3,fsort,result);
      if not result then
             check2varfunct(s1,s3,s4,fsort,result);
      if not result then
         checkintpower(s1,s3,s4,result);
      if not result then
         checkrealpower(s1,s3,s4,result);
      if result then
      begin
        checkvar(s2,varsort,result); if result then exit;
          checknum(s2,num,result);if result then exit;
        checkparam(s2,varsort,result); if result then exit;
          checkbrack(s2,s3,result);
          if result then  begin
            s2:=s3;  exit; end;
         checkfunct(s2,s3,fsort,result);if result then exit;
             check2varfunct(s2,s3,s4,fsort,result);if result then exit;
         checkintpower(s2,s3,s4,result);if result then exit;
         checkrealpower(s2,s3,s4,result);if result then exit;
      end;
      end;
    end;
    end;
  until result or (i>=length(s)) or (j=0);
end;

procedure check2varfunct(s:sstring;var s1,s2:sstring;var fsort:termsorttype;var result:boolean);
  procedure checkcomma(s:sstring;var s1,s2:sstring; var result:boolean);
  var s3:sstring; i,j:integer;
  begin
    result:=false;
    i:=0;
    repeat
      j:=pos(',',copy(s,i+1,length(s)-i));
      if j>0 then
      begin
        i:=i+j;
        if (i<length(s)) and (i>1) then
        begin
          s1:=copy(s,1,i-1);  s2:=copy(s,i+1,length(s)-i);
          s3:='('+s1+')';
          checkbrack(s3,s1,result);
          if result then
          begin
            s3:='('+s2+')';
            checkbrack(s3,s2,result);
          end;
        end;
      end;
    until result or (i>=length(s)) or (j=0);
  end;
var ss:sstring;
begin
  result:=false;
  if copy(s,1,3)='min' then
  begin
    ss:=copy(s,4,length(s)-3);
    if (ss[1]='(') and (ss[length(ss)]=')') then
    begin
      ss:=copy(ss,2,length(ss)-2);
      checkcomma(ss,s1,s2,result);
    end;
    if result then begin fsort:=minim; exit; end;
  end;
  if copy(s,1,3)='max' then
  begin
    ss:=copy(s,4,length(s)-3);
    if (ss[1]='(') and (ss[length(ss)]=')') then
    begin
      ss:=copy(ss,2,length(ss)-2);
      checkcomma(ss,s1,s2,result);
    end;
    if result then begin fsort:=maxim; exit; end;
  end;
  if copy(s,1,2)='rn' then
  begin
    ss:=copy(s,3,length(s)-2);
    if (ss[1]='(') and (ss[length(ss)]=')') then
    begin
      ss:=copy(ss,2,length(ss)-2);
      checkcomma(ss,s1,s2,result);
    end;
    if result then begin fsort:=randfunc; exit; end;
  end;
  if copy(s,1,3)='arg' then
  begin
    ss:=copy(s,4,length(s)-3);
    if (ss[1]='(') and (ss[length(ss)]=')') then
    begin
      ss:=copy(ss,2,length(ss)-2);
      checkcomma(ss,s1,s2,result);
    end;
    if result then begin fsort:=argu; exit; end;
  end;
  if copy(s,1,1)='r' then
  begin
    ss:=copy(s,2,length(s)-1);
    if (ss[1]='(') and (ss[length(ss)]=')') then
    begin
      ss:=copy(ss,2,length(ss)-2);
      checkcomma(ss,s1,s2,result);
    end;
    if result then begin fsort:=radius; exit; end;
  end;
  if copy(s,1,2)='rm' then
  begin
    ss:=copy(s,3,length(s)-1);
    if (ss[1]='(') and (ss[length(ss)]=')') then
    begin
      ss:=copy(ss,2,length(ss)-2);
      checkcomma(ss,s1,s2,result);
    end;
    if result then begin fsort:=randrand; exit; end;
  end;

end;


procedure checkfunct(s:sstring;var s1:sstring;var fsort:termsorttype;var result:boolean);
var s2,s3,s4:sstring; i,j:integer; num:float; ffsort:termsorttype;varsort:word;
begin
  result:=false;
  if copy(s,1,3)='cos' then
  begin
    s2:=copy(s,4,length(s)-3);
    checkbrack(s2,s1,result);
    if result then
      begin fsort:=cosine; exit; end;
  end;
  if copy(s,1,3)='sin' then
  begin
    s2:=copy(s,4,length(s)-3);
    checkbrack(s2,s1,result);
    if result then
      begin fsort:=sine; exit; end;
  end;
  if copy(s,1,3)='exp' then
  begin
    s2:=copy(s,4,length(s)-3);
    checkbrack(s2,s1,result);
    if result then
      begin fsort:=expo; exit; end;
  end;
  if copy(s,1,2)='ln' then
  begin
    s2:=copy(s,3,length(s)-2);
    checkbrack(s2,s1,result);
    if result then
      begin fsort:=logar; exit; end;
  end;
  if copy(s,1,6)='arctan' then
  begin
    s2:=copy(s,7,length(s)-6);
    checkbrack(s2,s1,result);
    if result then
      begin fsort:=arctang; exit; end;
  end;
  if copy(s,1,4)='sqrt' then
  begin
    s2:=copy(s,5,length(s)-4);
    checkbrack(s2,s1,result);
    if result then
      begin fsort:=sqroot; exit; end;
  end;
  if copy(s,1,3)='abs' then
  begin
    s2:=copy(s,4,length(s)-3);
    checkbrack(s2,s1,result);
    if result then
      begin fsort:=abso; exit; end;
  end;
  if copy(s,1,4)='heav' then
  begin
    s2:=copy(s,5,length(s)-4);
    checkbrack(s2,s1,result);
    if result then
      begin fsort:=heavi; exit; end;
  end;
  if copy(s,1,2)='ph' then
  begin
    s2:=copy(s,3,length(s)-2);
    checkbrack(s2,s1,result);
    if result then begin fsort:=phase; exit; end;
  end;
  if copy(s,1,4)='sinh' then
  begin
    s2:=copy(s,5,length(s)-4);
    checkbrack(s2,s1,result);
    if result then begin fsort:=hypersine; exit; end;
  end;
  if copy(s,1,4)='cosh' then
  begin
    s2:=copy(s,5,length(s)-4);
    checkbrack(s2,s1,result);
    if result then begin fsort:=hypercosine; exit; end;
  end;
  if not result then
  begin
    i:=0;
    repeat
      j:=pos('^',copy(s,i+1,length(s)-i));
      if j>0 then
      begin
      i:=i+j;
      if (1<i) and (i<length(s)) then
      begin
        s1:=copy(s,1,i-1);
        s2:=copy(s,i+1,length(s)-i);
        checkbracketnum(s1,result);
        if result then
          checkbracketnum(s2,result);
        if result then
        begin
          checkvar(s1,varsort,result);
          if not result then
          begin
            checkbrack(s1,s3,result);
            if result then
              s1:=s3;
          end;
          if not result then
            checknum(s1,num,result);
          if not result then
            checkparam(s1,varsort,result);
          if not result then
            checkfunct(s1,s3,ffsort,result);
          if not result then
            check2varfunct(s1,s3,s4,ffsort,result);
          if result then
          begin
          checknum(s2,num,result);
          if result then
            if (trunc(num)<>num) or (num<0) then
          result:=false
          else if trunc(num) in [2,3,4] then
          begin
            case trunc(num) of
              2:fsort:=square;
              3:fsort:=third;
              4:fsort:=forth;
            end;
          end
          else
            result:=false;
        end;
      end;
    end;
    end;
  until result or (i>=length(s)) or (j=0);
  end;
end;

procedure checkintpower(s:sstring;var s1,s2:sstring;var result:boolean);
var s3,s4:sstring; i,j:integer; num:float; fsort:termsorttype;varsort:word;
begin
  result:=false;
  i:=0;
  repeat
    j:=pos('^',copy(s,i+1,length(s)-i));
    if j>0 then
    begin
      i:=i+j;
    if (1<i) and (i<length(s)) then
    begin
      s1:=copy(s,1,i-1);
      s2:=copy(s,i+1,length(s)-i);
      checkbracketnum(s1,result);
      if result then
        checkbracketnum(s2,result);
      if result then
      begin
      checkvar(s1,varsort,result);
      if not result then
        checkparam(s1,varsort,result);
      if not result then
      begin
        checkbrack(s1,s3,result);
        if result then
          s1:=s3;
      end;
      if not result then
         checknum(s1,num,result);
      if not result then
         checkfunct(s1,s3,fsort,result);
      if not result then
         check2varfunct(s1,s3,s4,fsort,result);
      if result then
      begin
        checknum(s2,num,result);
        if result then
          if (trunc(num)<>num) then
            result:=false
          else if trunc(num) in [2,3,4] then
            result:=false;
      end;
      end;
    end;
    end;
  until result or (i>=length(s)) or (j=0);
end;

procedure checkrealpower(s:sstring;var s1,s2:sstring;var result:boolean);
var  i,j:integer; num:float; s3,s4:sstring; fsort:termsorttype;varsort:word;
begin
  result:=false;
  i:=0;
  repeat
    j:=pos('^',copy(s,i+1,length(s)-i));
    if j>0 then
    begin
      i:=i+j;
    if (1<i) and (i<length(s)) then
    begin
      s1:=copy(s,1,i-1);
      s2:=copy(s,i+1,length(s)-i);
      checkbracketnum(s1,result);
      if result then
        checkbracketnum(s2,result);
      if result then
      begin
      checkvar(s1,varsort,result);
      if not result then
        checkparam(s1,varsort,result);
      if not result then
        checknum(s1,num,result);
      if not result then
      begin
        checkbrack(s1,s3,result);
        if result then
          s1:=s3;
      end;
      if not result then
        checkfunct(s1,s3,fsort,result);
      if not result then
         check2varfunct(s1,s3,s4,fsort,result);
      if result then
      begin
        checknum(s2,num,result);
        if result then
        begin
          if (trunc(num)=num) then
            result:=false; exit end;
        checkvar(s2,varsort,result);if result then exit;
        checkparam(s2,varsort,result); if result then exit;
        checkbrack(s2,s3,result);
        if result then begin
            s2:=s3; exit; end;
        checkfunct(s2,s3,fsort,result);if result then exit;
        check2varfunct(s2,s3,s4,fsort,result);if result then exit;
      end;
      end;
    end;
    end;
  until result or (i>=length(s)) or (j=0);
end;



const maxlevels=20;  maxlevelsize=50;




type

         termpointer=^termrec;

         operation1pointer=^operation1;

         termrec=record
                 s:sstring;
                 termsort:termsorttype;
                 s1,s2:sstring;
                 posit:array[1..3] of integer;
                 next1,next2,prev:termpointer
                 end;



         operation1=record
                   theop:operationpointer;
                   end;




         levelsizearray=array[0..maxlevels] of integer;

  procedure ini(var theop:operationpointer;term:termsorttype);
  begin
    new(theop);
    with theop^ do
    begin
      arg1:=nil; arg2:=nil; dest:=nil; next:=nil;
      opnum:=ord(term);
    end;
  end;

procedure parsefunction(s:string;var fop:operationpointer;
            var pointx,pointy,pointt,a,b,c,d,e:rpointer;var numop:integer;
           var error:boolean; showprogress:boolean);

var result,done,found:boolean; code,l,i,levels,p:integer;
      ab,levelsize:levelsizearray; s3,blanks:sstring;
    firstterm,next1term,next2term,lastterm:termpointer; fsort:termsorttype;
    matrix:array[0..maxlevels,1..maxlevelsize] of operation1pointer;
    lastop:operationpointer;
    num:float; varsort:word;



begin
  error:=false;
  new(pointx); new(pointy); new(pointt);
  new(a); new(b); new(c); new(d); new(e);
  blanks:=' ';
  chopblanks(s);
  repeat
    checkbrack(s,s3,result);
    if result then s:=s3;
  until result=false;
  done:=false;
  levels:=0;
  levelsize[0]:=1;
  for l:=0 to maxlevels do
    ab[l]:=0;
  new(firstterm);
  firstterm^.s:=s;
  with firstterm^ do
  begin
    s1:=blanks; s2:=blanks; termsort:=variab;
    next1:=nil; next2:=nil; prev:=nil;
    new(matrix[0,1]);
    new(matrix[0,1]^.theop);
    with matrix[0,1]^.theop^ do
    begin
      arg1:=nil; arg2:=nil; dest:=nil;
      opnum:=ord(variab); next:=nil;
    end;
  end;
  lastterm:=firstterm;
  lastterm^.posit[1]:=0;
  lastterm^.posit[2]:=1;
  lastterm^.posit[3]:=1;
  repeat
    code:=0;
    repeat
       l:=lastterm^.posit[1];
       i:=lastterm^.posit[2];
       if showprogress then write('.');
      if lastterm^.next1=nil then
      with lastterm^ do
      begin
        checkvar(s,varsort,result);
        if result then
        begin
          termsort:=variab;
          if varsort=1 then
            if posit[3]=1 then matrix[l,i]^.theop^.arg1:=pointx
              else matrix[l,i]^.theop^.arg2:=pointx
          else if varsort=2 then
            if posit[3]=1 then matrix[l,i]^.theop^.arg1:=pointy
              else matrix[l,i]^.theop^.arg2:=pointy
          else
            if posit[3]=1 then matrix[l,i]^.theop^.arg1:=pointt
              else matrix[l,i]^.theop^.arg2:=pointt;
        end
        else
        begin
          checkparam(s,varsort,result);
          if result then
          begin
            termsort:=constant;
            if varsort=1 then
              if posit[3]=1 then matrix[l,i]^.theop^.arg1:=a
                else matrix[l,i]^.theop^.arg2:=a
            else if varsort=2 then
              if posit[3]=1 then matrix[l,i]^.theop^.arg1:=b
                else matrix[l,i]^.theop^.arg2:=b
            else if varsort=3 then
              if posit[3]=1 then matrix[l,i]^.theop^.arg1:=c
                else matrix[l,i]^.theop^.arg2:=c
            else if varsort=4 then
              if posit[3]=1 then matrix[l,i]^.theop^.arg1:=d
                else matrix[l,i]^.theop^.arg2:=d
            else if varsort=5 then
              if posit[3]=1 then matrix[l,i]^.theop^.arg1:=e
                else matrix[l,i]^.theop^.arg2:=e;
          end
          else
          begin
            checknum(s,num,result);
            if result then
            begin
               termsort:=constant;
              if posit[3]=1 then
              begin
                new(matrix[l,i]^.theop^.arg1);
                matrix[l,i]^.theop^.arg1^:=num;
              end else
              begin
                new(matrix[l,i]^.theop^.arg2);
                matrix[l,i]^.theop^.arg2^:=num;
              end;
            end
            else
            begin
              checkmin(s,s1,result);
              if result then
                termsort:=minus
              else
              begin
                checksum(s,s1,s2,result);
                if result then
                  termsort:=sum
                else
                begin
                  checkdiff(s,s1,s2,result);
                  if result then
                    termsort:=diff
                  else
                  begin
                    checkprod(s,s1,s2,result);
                    if result then
                      termsort:=prod
                    else
                    begin
                      checkdiv(s,s1,s2,result);
                      if result then
                        termsort:=divis
                      else
                      begin
                        checkfunct(s,s1,fsort,result);
                        if result then
                        begin
                          termsort:=fsort;
                        end
                        else
                        begin
                          checkintpower(s,s1,s2,result);
                          if result then
                            termsort:=intpower
                          else
                          begin
                            checkrealpower(s,s1,s2,result);
                            if result then
                              termsort:=realpower
                            else
                            begin
                              check2varfunct(s,s1,s2,fsort,result);
                              if result then
                              begin
                                termsort:=fsort;
                                if fsort=randfunc then
                                begin
                                  val(s1,num,code);
                                  randomsize:=round(num);
                                  randomiterates:=true;
                                  randomize;
                                end;
                                if termsort=randrand then
                                begin
                                  contrand:=true;
                                  randomize;
                                end;
                              end
                              else
                              begin
                                error:=true;
                                writeln('Syntax Error!');
                                exit;
                              end;
                            end;
                          end;
                        end;
                      end;
                    end;
                  end;
                end;
              end;
            end;
          end;
        end;
      end; {with lastterm^}
      if lastterm^.termsort in [brack,minus,cosine,sine,expo,logar,
                          sqroot,arctang,square,third,forth,
                          abso,heavi,phase,hypersine,hypercosine] then
        begin
          new(next1term);
          l:=l+1;
          if l>maxlevels then
          begin
            writeln('Too many nestings!');
            error:=true; exit;
          end;
          if levels<l then
            levels:=l;
          i:=ab[l]+1;
          if i>maxlevelsize then
          begin
            writeln('Term too long, sorry!');
            error:=true; exit;
          end;
          with next1term^ do
          begin
            s:=lastterm^.s1;
            prev:=lastterm;
            posit[1]:=l;  posit[2]:=i; posit[3]:=1;
            termsort:=variab;
            s1:=blanks; s2:=blanks; num:=0;
             next1:=nil; next2:=nil;
             new(matrix[l,i]);
             ini(matrix[l,i]^.theop,lastterm^.termsort);
             p:=lastterm^.posit[3];
             new(matrix[l,i]^.theop^.dest);
             matrix[l,i]^.theop^.dest^:=0;
             if p=1 then
               matrix[lastterm^.posit[1],lastterm^.posit[2]]^.theop^.arg1:=
                           matrix[l,i]^.theop^.dest else
               matrix[lastterm^.posit[1],lastterm^.posit[2]]^.theop^.arg2:=
                           matrix[l,i]^.theop^.dest;
           end;
          lastterm^.next1:=next1term;
          ab[l]:=ab[l]+1;
        end;
      if lastterm^.termsort in
                 [sum,diff,prod,divis,intpower,realpower,maxim,minim,
                 randfunc,argu,radius,randrand] then
        begin
          if lastterm^.next1=nil then
          begin
            new(next1term);
            l:=l+1;
            if l>maxlevels then
            begin
              writeln('Too many nestings!');
              error:=true; exit;
            end;
            if levels<l then
              levels:=l;
            i:=ab[l]+1;
            if i>maxlevelsize then
            begin
              writeln('Term too long, sorry!');
              error:=true; exit;
            end;
            with next1term^ do
            begin
              s:=lastterm^.s1;
              prev:=lastterm;
              posit[1]:=l;
              posit[2]:=i; posit[3]:=1;
              num:=0;
              s1:=blanks; s2:=blanks; termsort:=variab;
              next1:=nil; next2:=nil;
              new(matrix[l,i]);
              ini(matrix[l,i]^.theop,lastterm^.termsort);
              p:=lastterm^.posit[3];
              new(matrix[l,i]^.theop^.dest);
              matrix[l,i]^.theop^.dest^:=0;
              if p=1 then
               matrix[lastterm^.posit[1],lastterm^.posit[2]]^.theop^.arg1:=
                           matrix[l,i]^.theop^.dest else
               matrix[lastterm^.posit[1],lastterm^.posit[2]]^.theop^.arg2:=
                           matrix[l,i]^.theop^.dest;
            end;
            lastterm^.next1:=next1term;
          end
          else
          begin
            new(next2term);
            l:=l+1;
            if l>maxlevels then
            begin
              writeln('Too many nestings!');
              error:=true; exit;
            end;
            if levels<l then
              levels:=l;
            i:=ab[l]+1;
            if i>maxlevelsize then
            begin
              writeln('Term too long, sorry!');
              error:=true; exit;
            end;
            with next2term^ do
            begin
              s:=lastterm^.s2;
              prev:=lastterm;
              posit[1]:=l; posit[2]:=i; posit[3]:=2;
              num:=0;
              s1:=blanks; s2:=blanks; termsort:=variab;
              next1:=nil; next2:=nil;
            end;
            lastterm^.next2:=next2term;
            ab[l]:=ab[l]+1;
          end;
        end;
      if lastterm^.next1=nil then
        code:=1
      else
        if lastterm^.next2=nil then
          lastterm:=lastterm^.next1
        else
          lastterm:=lastterm^.next2;
   until code=1;
   if lastterm=firstterm then
   begin
     done:=true;
     dispose(lastterm);
     firstterm:=nil;
   end
   else
   begin
     repeat
       if lastterm^.next1<>nil then
         dispose(lastterm^.next1);
       if lastterm^.next2<>nil then
         dispose(lastterm^.next2);
       lastterm:=lastterm^.prev;
     until ((lastterm^.termsort in [sum,diff,prod,divis,intpower,realpower,
                  maxim,minim,randfunc,argu,radius,randrand])
             and
            (lastterm^.next2=nil)) or (lastterm=firstterm);
     if (lastterm=firstterm) and ((firstterm^.termsort in [brack,minus,cosine,sine,
                expo,logar,sqroot,arctang,square,third,forth,
                abso,heavi,phase,hypersine,hypercosine])
                   or ((firstterm^.termsort in [sum,diff,prod,divis,intpower,
                      realpower,maxim,minim,randfunc,argu,radius,randrand])
                       and (firstterm^.next2<>nil))) then
         done:=true;
   end;
 until done;
 if firstterm<>nil then
 begin
   if firstterm^.next1<>nil then dispose(firstterm^.next1);
   if firstterm^.next2<>nil then dispose(firstterm^.next2);
   dispose(firstterm);
 end;
 for l:=1 to levels do
   levelsize[l]:=ab[l];
 if levels=0 then
 begin
   fop:=matrix[0,1]^.theop;
   fop^.dest:=fop^.arg1;
   numop:=0;
   dispose(matrix[0,1]);
 end
 else
 begin
   for l:=levels downto 1 do
   for i:=1 to levelsize[l] do
   begin
     if (l=levels) and (i=1) then
     begin
       numop:=1;
       fop:=matrix[l,i]^.theop;
       lastop:=fop;
       dispose(matrix[l,i]);
     end
     else
     begin
       inc(numop);
       lastop^.next:=matrix[l,i]^.theop;
       lastop:=lastop^.next;
       dispose(matrix[l,i]);
     end;
   end;
   with matrix[0,1]^.theop^ do
   begin
     arg1:=nil; arg2:=nil; dest:=nil;
   end;
   dispose(matrix[0,1]^.theop);
   dispose(matrix[0,1]);
 end;
end;
end.