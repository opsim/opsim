
(* lexical analyzer template (TP Lex V3.0), V1.0 3-2-91 AG *)

(* global definitions: *)

function yylex : Integer;

procedure yyaction ( yyruleno : Integer );
  (* local definitions: *)

begin
  (* actions: *)
  case yyruleno of
  1:
              			return(CHF_HYDRATE);
  2:
                        return(CHF_NAME);
  3:
      	                return(CHF_NUM);
  4:
   	                return(CHF_LPAREN);
  5:
   	                return(CHF_RPAREN);
  6:
                        return(CHF_EOL);

  end;
end(*yyaction*);

(* DFA table: *)

type YYTRec = record
                cc : set of Char;
                s  : Integer;
              end;

const

yynmarks   = 7;
yynmatches = 7;
yyntrans   = 18;
yynstates  = 12;

yyk : array [1..yynmarks] of Integer = (
  { 0: }
  { 1: }
  { 2: }
  { 3: }
  2,
  { 4: }
  3,
  { 5: }
  4,
  { 6: }
  5,
  { 7: }
  6,
  { 8: }
  { 9: }
  2,
  { 10: }
  { 11: }
  1
);

yym : array [1..yynmatches] of Integer = (
{ 0: }
{ 1: }
{ 2: }
{ 3: }
  2,
{ 4: }
  3,
{ 5: }
  4,
{ 6: }
  5,
{ 7: }
  6,
{ 8: }
{ 9: }
  2,
{ 10: }
{ 11: }
  1
);

yyt : array [1..yyntrans] of YYTrec = (
{ 0: }
  ( cc: [ #10 ]; s: 7),
  ( cc: [ '(' ]; s: 5),
  ( cc: [ ')' ]; s: 6),
  ( cc: [ '.' ]; s: 2),
  ( cc: [ '0'..'9' ]; s: 4),
  ( cc: [ 'A'..'Z' ]; s: 3),
{ 1: }
  ( cc: [ #10 ]; s: 7),
  ( cc: [ '(' ]; s: 5),
  ( cc: [ ')' ]; s: 6),
  ( cc: [ '.' ]; s: 2),
  ( cc: [ '0'..'9' ]; s: 4),
  ( cc: [ 'A'..'Z' ]; s: 3),
{ 2: }
  ( cc: [ '0'..'9' ]; s: 2),
  ( cc: [ 'H' ]; s: 8),
{ 3: }
  ( cc: [ 'a'..'z' ]; s: 9),
{ 4: }
  ( cc: [ '0'..'9' ]; s: 4),
{ 5: }
{ 6: }
{ 7: }
{ 8: }
  ( cc: [ '2' ]; s: 10),
{ 9: }
{ 10: }
  ( cc: [ 'O' ]; s: 11)
{ 11: }
);

yykl : array [0..yynstates-1] of Integer = (
{ 0: } 1,
{ 1: } 1,
{ 2: } 1,
{ 3: } 1,
{ 4: } 2,
{ 5: } 3,
{ 6: } 4,
{ 7: } 5,
{ 8: } 6,
{ 9: } 6,
{ 10: } 7,
{ 11: } 7
);

yykh : array [0..yynstates-1] of Integer = (
{ 0: } 0,
{ 1: } 0,
{ 2: } 0,
{ 3: } 1,
{ 4: } 2,
{ 5: } 3,
{ 6: } 4,
{ 7: } 5,
{ 8: } 5,
{ 9: } 6,
{ 10: } 6,
{ 11: } 7
);

yyml : array [0..yynstates-1] of Integer = (
{ 0: } 1,
{ 1: } 1,
{ 2: } 1,
{ 3: } 1,
{ 4: } 2,
{ 5: } 3,
{ 6: } 4,
{ 7: } 5,
{ 8: } 6,
{ 9: } 6,
{ 10: } 7,
{ 11: } 7
);

yymh : array [0..yynstates-1] of Integer = (
{ 0: } 0,
{ 1: } 0,
{ 2: } 0,
{ 3: } 1,
{ 4: } 2,
{ 5: } 3,
{ 6: } 4,
{ 7: } 5,
{ 8: } 5,
{ 9: } 6,
{ 10: } 6,
{ 11: } 7
);

yytl : array [0..yynstates-1] of Integer = (
{ 0: } 1,
{ 1: } 7,
{ 2: } 13,
{ 3: } 15,
{ 4: } 16,
{ 5: } 17,
{ 6: } 17,
{ 7: } 17,
{ 8: } 17,
{ 9: } 18,
{ 10: } 18,
{ 11: } 19
);

yyth : array [0..yynstates-1] of Integer = (
{ 0: } 6,
{ 1: } 12,
{ 2: } 14,
{ 3: } 15,
{ 4: } 16,
{ 5: } 16,
{ 6: } 16,
{ 7: } 16,
{ 8: } 17,
{ 9: } 17,
{ 10: } 18,
{ 11: } 18
);


var yyn : Integer;

label start, scan, action;

begin

start:

  (* initialize: *)

  yynew;

scan:

  (* mark positions and matches: *)

  for yyn := yykl[yystate] to     yykh[yystate] do yymark(yyk[yyn]);
  for yyn := yymh[yystate] downto yyml[yystate] do yymatch(yym[yyn]);

  if yytl[yystate]>yyth[yystate] then goto action; (* dead state *)

  (* get next character: *)

  yyscan;

  (* determine action: *)

  yyn := yytl[yystate];
  while (yyn<=yyth[yystate]) and not (yyactchar in yyt[yyn].cc) do inc(yyn);
  if yyn>yyth[yystate] then goto action;
    (* no transition on yyactchar in this state *)

  (* switch to new state: *)

  yystate := yyt[yyn].s;

  goto scan;

action:

  (* execute action: *)

  if yyfind(yyrule) then
    begin
      yyaction(yyrule);
      if yyreject then goto action;
    end
  else if not yydefault and yywrap() then
    begin
      yyclear;
      return(0);
    end;

  if not yydone then goto start;

  yylex := yyretval;

end(*yylex*);


