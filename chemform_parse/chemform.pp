program chemform;

uses
  LinkedList,
  CHF_tokenizer;

var
  tok    : pCHFtoken;
  tstack : pListBase;
begin
  CHF_parse_chemfunc('CoCl2.6H2O', tstack);

  //now everything is on the stack pop every item
  tok := tstack^.first;
  while tok <> nil do
  begin
    writeln(tok^.value, ' ', tok^.token);
    tok := tok^.next;
  end;

    //token := yylex;
    //writeln(yytext, ' ', token);
    //
    //case token of
    //  CHF_NAME    = 0;
    //  CHF_NUM     = 1;
    //  CHF_LPAREN  = 2;
    //  CHF_RPAREN  = 3;
    //
    //  CHF_HYDRATE:
    //  begin
    //    snum := copy(yytext, 2, length(yytext) - 4);
    //    val(snum, inum, code);
    //    if code <> 0 then
    //      writeln('error at position ', code, ' : ', snum)
    //    else
    //      writeln('value : ', inum);
    //  end;
    //end;

  CHF_free(tstack);

  readln;
end.
