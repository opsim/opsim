Unit CS2_PRC;
{$I CS2_DEF.INC}
Interface

Function ConvertProc(S: String) : String;
{
Converts a function header to a CajScript function header:
'Function test(S : String) : Boolean;' => '14 TEST S 8'
}

Implementation

Uses Cs2_UTL, CS2_Var;

Function IntToStr (I : LongInt) : String;
Var
  s : String;
Begin
  Str ( i, s);
  IntToStr := s;
End;

Type
  TGTyperec = Record
               ident : String[20];
               typeid : Word;
              End;
Const
  GTypes : Array [1..15] Of TGTyperec =(
  (Ident: 'BYTE'; typeid: CSV_UByte),
  (Ident: 'SHORTINT'; typeid: CSV_SByte),
  (Ident: 'CHAR'; typeid: CSV_Char),
  (Ident: 'WORD'; typeid: CSV_UInt16),
  (Ident: 'SMALLINT'; typeid: CSV_SInt16),
  (Ident: 'CARDINAL'; typeid: CSV_UInt32),
  (Ident: 'LONGINT'; typeid: CSV_SInt32),
  (Ident: 'INTEGER'; typeid: CSV_SInt32),
  (Ident: 'STRING'; typeid: CSV_String),
  (Ident: 'REAL'; typeid: CSV_Real),
  (Ident: 'SINGLE'; typeid: CSV_Single),
  (Ident: 'DOUBLE'; typeid: CSV_Double),
  (Ident: 'EXTENDED'; typeid: CSV_Extended),
  (Ident: 'COMP'; typeid: CSV_Comp),
  (Ident: 'BOOLEAN'; typeid: CSV_Bool));

Function GetType ( Const s : String ) : Word;
var
  i : Integer;
Begin
  For i:=1 To 15 Do Begin
    If s=GTypes[i].Ident Then Begin
      GetType:=GTypes[i].typeid;
      Exit;
    End;
  End;
  GetType:=0;
End;

Function ConvertProc(S: String) : String;
var
  Parser : PCS2PascalParser;
  CurrVar : String;
  FuncName,
  FuncParam : String;
  FuncRes,
  CurrType : Word;
Begin
  s:=s+#0;
  New(Parser);
  Parser^.CurrTokenPos:=0;
  Parser^.text:=@s[1];
  ParseToken(Parser);
  If Parser^. CurrTokenId = CSTII_Procedure Then
    FuncRes := 0
  Else
    FuncRes := 1;
  NextNoJunk (Parser);
  FuncName := FastUppercase (GetToken (Parser) );
  FuncParam := FuncName;
  CurrVar := '';
  NextNoJunk (Parser);
  If parser^. CurrTokenId = CSTI_OpenRound Then Begin
    NextNoJunk (Parser);
    While True Do Begin
      If Parser^. CurrTokenId = CSTII_Var Then Begin
        CurrVar := '!';
        NextNoJunk (Parser);
      End; {if}
      While True Do Begin
        CurrVar := CurrVar + fastuppercase (GetToken (Parser) ) + '|';
        NextNoJunk (parser);
        If Parser^. CurrTokenId = CSTI_Colon Then Break;
        NextNoJunk (Parser);
      End; {while}
      NextNoJunk (Parser);
      CurrType := GetType (FastUppercase (GetToken (Parser) ) );
      If Pos ('!', CurrVar) = 1 Then Begin
        Delete (currVar, 1, 1);
        While Pos ('|', CurrVar) > 0 Do Begin
          FuncParam := FuncParam + ' !' + Copy (CurrVar, 1, Pos ('|', CurrVar) - 1) + ' ' + IntToStr (CurrType);
          Delete (CurrVar, 1, Pos ('|', CurrVar) );
        End; {while}
      End Else Begin
        While Pos ('|', CurrVar) > 0 Do Begin
          FuncParam := FuncParam + ' ' + Copy (CurrVar, 1, Pos ('|', CurrVar) - 1) + ' ' + IntToStr (CurrType);
          Delete (CurrVar, 1, Pos ('|', CurrVar) );
        End; {while}
      End; {if}
      NextNoJunk (Parser);
      If Parser^. CurrTokenId = CSTI_CloseRound Then Begin
        NextNoJunk (Parser);
        Break;
      End; {if}
      NextNoJunk (Parser);
    End;
  End;
  If FuncRes = 1 Then Begin
    NextNoJunk (Parser);
    FuncRes :=  GetType (FastUppercase (GetToken (Parser) ) );
  End;
  FuncParam := InttoStr (FuncRes) + ' ' + FuncParam;
  Dispose(Parser);
  ConvertProc:=FuncParam;
end;

End.