unit CHF_tokenizer;

{$mode objfpc}{$H-}

interface

uses
  lexlib,
  LinkedList;

const
  CHF_NAME    = 0;
  CHF_NUM     = 1;
  CHF_LPAREN  = 2;
  CHF_RPAREN  = 3;
  CHF_EOL     = 4;
  CHF_HYDRATE = 5;

type
  pCHFtoken = ^CHFtoken;
  CHFtoken = record
    next, prev : pCHFtoken;
    token      : integer;
    value      : string;
  end;

procedure CHF_parse_chemfunc(chemfunc   : string;
                             var tstack : pListBase);

implementation

{$i CHF_lexer.pas}

procedure CHF_parse_chemfunc(chemfunc   : string;
                             var tstack : pListBase);
var
  token  : integer;
  tok    : pCHFtoken;
begin
  if tstack = nil then
    exit;

  yyset_buffer(chemfunc);

  //push all tokens onto the stack
  token := yylex;
  while token <> CHF_EOL do
  begin
    tok := callocN(sizeof(CHFtoken));
    tok^.token := token;
    tok^.value := yytext;

    addhead(tstack, tok);
    token := yylex;
  end;
end;

end.

