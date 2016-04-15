program chemform;

uses
  LinkedList,
  CHF_tokenizer;

var
  tok    : pCHFtoken;
  tstack : pListBase;
begin
  tstack := callocN(sizeof(ListBase));

  CHF_parse_chemfunc('CoCl2.6H2O', tstack);

  //now everything is on the stack pop every item
  tok := tstack^.first;
  while tok <> nil do
  begin
    writeln(tok^.value, ' ', tok^.token);
    tok := tok^.next;
  end;

  freelistN(tstack);
  freeN(tstack);
end.
