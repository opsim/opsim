unit CS2_UTL; {Cajscript 2.0 Utilities (Parser, Pascal:TList, TStringlist)}
{$I CS2_DEF.INC}
interface

type
  TCs2TokenId = (
  {Errors}
    CSTI_SyntaxError,
    CSTI_CommentEOFError,
    CSTI_CharError,
    CSTI_StringError,
  {Tokens}
    CSTI_EOF,
    CSTI_Whitespace,
    CSTI_Comment,
    CSTI_Identifier,
    CSTI_SemiColon,
    CSTI_Comma,
    CSTI_Period,
    CSTI_Colon,
    CSTI_OpenRound,
    CSTI_CloseRound,
    CSTI_OpenBlock,
    CSTI_CloseBlock,
    CSTI_Assignment,
    CSTI_Equal,
    CSTI_NotEqual,
    CSTI_Greater,
    CSTI_GreaterEqual,
    CSTI_Less,
    CSTI_LessEqual,
    CSTI_Plus,
    CSTI_Minus,
    CSTI_Divide,
    CSTI_Multiply,
    CSTI_Integer,
    CSTI_Real,
    CSTI_String,
    CSTI_Char,
    CSTI_HexInt,
  {Identifiers}
    CSTII_and,
    CSTII_begin,
    CSTII_case,
    CSTII_const,
    CSTII_div,
    CSTII_do,
    CSTII_downto,
    CSTII_else
    ,
    CSTII_end,
    CSTII_for,
    CSTII_function,
    CSTII_if,
    CSTII_in,
    CSTII_mod,
    CSTII_not,
    CSTII_of,
    CSTII_or,
    CSTII_procedure,
    CSTII_program,
    CSTII_repeat,
    CSTII_set,
    CSTII_shl,
    CSTII_shr,
    CSTII_string,
    CSTII_then,
    CSTII_to,
    CSTII_type,
    CSTII_until,
    CSTII_uses,
    CSTII_var,
    CSTII_while,
    CSTII_with,
    CSTII_xor
    );
  PCs2PascalParser = ^TCs2PascalParser;
  TCs2PascalParser = record
    CurrTokenId: TCs2Tokenid;
    CurrTokenLen: LongInt;
    CurrTokenPos: LongInt;
    Text: PChar;
  end;

procedure NextNoJunk(data: PCs2PascalParser);
{Go to the next nojunk token}
function GetToken(data: PCs2PascalParser): string;
{Return the token}
procedure ParseToken(data: PCs2PascalParser);
{Parse at active position}

type
  PIfListItem = ^TIfListItem;
  TIfListItem = record
    Next: PIfListItem;
    Prev: PIfListItem;
    Ptr: Pointer;
  end;
  TIfList = object
  Private
    FCount: LongInt;
    RootItem: PIfListItem;
    CurrItem: PIfListItem;
    currItemIdx: Longint;
  Public
    function Count: LongInt;
    function GetItem(Nr: LongInt): Pointer;
    procedure SetItem(Nr: LongInt; P: Pointer);
    procedure Add(P: Pointer);
    procedure Remove(P: Pointer);
    procedure Delete(Nr: Longint);
    procedure Clear;
    constructor Create;
    destructor Destroy;
  end;

  TIfStringList = object
  Private
    List: TIfList;
  Public
    function Count: LongInt;
    function GetItem(Nr: LongInt): string;
    procedure SetItem(Nr: LongInt; const s: string);
    procedure Add(const P: string);
    procedure Delete(NR: LongInt);
    procedure Clear;
    constructor Create;
    destructor Destroy;
  end;

function FastUpperCase(const s: string): string;
{Fast uppercase}

function Fw(const S: string): string;
{
First word
}
procedure Rs(var S: string);
{
  Remove space left (TrimLeft)
}
implementation

function Fw(const S: string): string;
{
First word
}
begin
  if Pos(' ', s) > 0 then
    Fw := Copy(S, 1, Pos(' ', s) - 1)
  else

    Fw := S;
end;

procedure Rs(var S: string);
{
  Remove space left (TrimLeft)
}
begin
  while (Length(s) > 0) do begin
    if s[1] = ' ' then
      Delete(S, 1, 1)
    else
      Break;
  end;
end;


constructor TIfList.Create;
begin
  FCount := 0;
  RootItem := nil;
  CurrItem := nil;
  curritemidx := -1;
end;

procedure TIfList.Add(P: Pointer);
var
  w: PIfListItem;
begin
  if RootItem = nil then
  begin
    new(rootitem);
    rootitem^.prev := nil;
    rootitem^.next := nil;
    rootitem^.Ptr := p;
    CurrItem := rootitem;
    curritemidx := 0;
    inc(fcount);
  end else
  begin
    while assigned(CurrItem^.next) do begin
      inc(CurrItemIdx);
      curritem := curritem^.next;
    end;
    new(w);
    curritem^.next := w;
    w^.Next := nil;
    w^.prev := curritem;
    w^.Ptr := p;
    inc(fcount);
  end;
end;

procedure TIfList.Delete(Nr: Longint);
begin
  if not Assigned(RootItem) or (nr >= fCount) then
    Exit;
  if nr < curritemidx then
  begin
    CurrItem := RootItem;
    CurrItemIdx := 0;
  end;
  while curritemidx < nr do begin
    CurrItem := CurrItem^.Next;
    Inc(CurrItemIdx);
  end;
  if assigned(curritem^.prev) then
  begin
    curritem^.prev^.next := curritem^.next;
    if assigned(CurrItem^.next) then
      curritem^.next^.prev := currItem^.prev;
  end
  else

  begin
    rootitem := curritem^.next;
    if assigned(curritem^.next) then
      curritem^.next^.prev := nil;
  end;
  dispose(curritem);
  currItemIdx := 0;
  CurrItem := rootitem;
  dec(fcount);
end;

procedure TIfList.Remove(P: Pointer);
begin
  CurrItem := rootitem;
  while assigned(curritem) do
  begin
    if curritem^.Ptr = p then
    begin
      if assigned(curritem^.prev) then
      begin
        curritem^.prev^.next := curritem^.next;
        if assigned(CurrItem^.next) then
          curritem^.next^.prev := currItem^.prev;
      end
      else

      begin
        rootitem := curritem^.next;
        if assigned(curritem^.next) then
          curritem^.next^.prev := nil;
      end;
      dispose(curritem);
      dec(fcount);
      break;
    end;
    curritem := curritem^.next;
  end;
  currItemIdx := 0;
  CurrItem := rootitem;
end;

procedure TIfList.Clear;
begin
  curritem := rootitem;
  while assigned(curritem) do begin
    if assigned(curritem^.next) then
    begin
      curritem := curritem^.next;
      dispose(curritem^.prev);
    end else
    begin
      dispose(curritem);
      curritem := nil;
    end;
  end;
  rootitem := nil;
  curritem := nil;
  curritemidx := -1;
  fcount := 0;
end;

destructor TIfList.Destroy;
begin
  clear;
end;

procedure TIfList.SetItem(Nr: LongInt; P: Pointer);
begin
  if not Assigned(RootItem) or (nr >= fCount) then
    Exit;
  if nr < curritemidx then
  begin
    CurrItem := RootItem;
    CurrItemIdx := 0;
  end;
  while curritemidx < nr do begin
    CurrItem := CurrItem^.Next;
    Inc(CurrItemIdx);
  end;
  CurrItem^.Ptr := p;
end;

function TifList.GetItem(Nr: LongInt): Pointer;
begin
  getitem := nil;
  if not Assigned(RootItem) or (nr >= fCount) then
    Exit;
  if nr < curritemidx then
  begin
    CurrItem := RootItem;
    CurrItemIdx := 0;
  end;
  while curritemidx < nr do begin
    CurrItem := CurrItem^.Next;
    Inc(CurrItemIdx);
  end;
  getitem := CurrItem^.Ptr;
end;

function TifList.Count: LongInt;
begin
  count := Fcount;
end;


function TIfStringList.Count: LongInt;
begin
  count := List.count;
end;
{$IFDEF SS}
type TsmallStr = record
    Size: Byte;
    Str: array[1..255] of Char;
  end;

function TifStringList.GetItem(Nr: LongInt): string;
var S: ^TSMALLSTR;
  tel: Byte;
begin
  getitem := '';
  s := List.GetItem(Nr);
  if s = nil then
    Exit;
  GetItem[0] := Chr(S^.Size);
  for tel := 1 to S^.Size do
    GetItem[tel] := S^.Str[tel];
end;

procedure TifStringList.SetItem(Nr: LongInt; const s: string);
var
  p: ^TSMALLSTR;
  tel: LongInt;
begin
  p := List.GetItem(Nr);
  if p = nil then
    Exit;
  FreeMem(p, p^.Size + 1);
  GetMem(p, Length(s) + 1);
  p^.Size := Length(s);
  for tel := 1 to p^.Size do
    p^.Str[tel] := s[tel];
  List.SetItem(Nr, p);
end;

procedure TifStringList.Add(const P: string);
var S: ^TSMALLSTR;
  tel: Byte;
begin
  GetMem(S, 1 + Length(P));
  s^.Size := Length(p);
  for tel := 1 to s^.Size do
    s^.Str[tel] := p[tel];
  List.Add(S);
end;

procedure TifStringList.Delete(NR: LongInt);
var P: ^TSMALLSTR;
begin
  p := list.getitem(nr);
  list.Remove(P);
  dispose(p);
end;

{$else
}
type pStr = ^string;

function TifStringList.GetItem(Nr: LongInt): string;
var
  S: PStr;
begin
  s := List.GetItem(Nr);
  if s = nil then
    Result := ''
  else

    Result := s^;
end;

procedure TifStringList.SetItem(Nr: LongInt; const s: string);
var
  p: PStr;
begin
  p := List.GetItem(Nr);
  if p = nil
    then
    Exit;
  p^ := s;
end;

procedure TifStringList.Add(const P: string);
var
  w: PStr;
begin
  new(w);
  w^ := p;
  List.Add(w);
end;

procedure TifStringList.Delete(NR: LongInt);
var
  W: PStr;
begin
  W := list.getitem(nr);
  if assigned(w) then
  begin
    dispose(w);
    list.Delete(Nr);
  end;
end;
{$ENDIF}

procedure TifStringList.Clear;
begin
  while List.Count > 0 do Delete(0);
end;

constructor TifStringList.Create;
begin
  List.Create;
end;

destructor TifStringList.Destroy;
var I: LongInt;
begin
  for I := 0 to list.count - 1 do
    Delete(I);
  List.Destroy;
end;

const chartab: array[Char] of Char = (
{Char tab is used for making a string in uppercase. This way is much faster}
    #0, #1, #2, #3, #4, #5, #6, #7, #8, #9, #10, #11, #12, #13, #14, #15, #16,
    #17, #18, #19, #20, #21, #22, #23, #24, #25, #26, #27, #28, #29, #30, #31,
    #32, #33, #34, #35, #36, #37, #38, #39, #40, #41, #42, #43, #44, #45, #46,
    #47, #48, #49, #50, #51, #52, #53, #54, #55, #56, #57, #58, #59, #60, #61,
    #62, #63, #64, #65, #66, #67, #68, #69, #70, #71, #72, #73, #74, #75, #76,
    #77, #78, #79, #80, #81, #82, #83, #84, #85, #86, #87, #88, #89, #90, #91,
    #92, #93, #94, #95, #96, #65, #66, #67, #68, #69, #70, #71, #72, #73, #74,
    #75, #76, #77, #78, #79, #80, #81, #82, #83, #84, #85, #86, #87, #88, #89,
    #90, #123, #124, #125, #126, #127, #128, #129, #130, #131, #132, #133, #134,
    #135, #136, #137, #138, #139, #140, #141, #142, #143, #144, #145, #146,
    #147, #148, #149, #150, #151, #152, #153, #154, #155, #156, #157, #158,
    #159, #160, #161, #162, #163, #164, #165, #166, #167, #168, #169, #170,
    #171, #172, #173, #174, #175, #176, #177, #178, #179, #180, #181, #182,
    #183, #184, #185, #186, #187, #188, #189, #190, #191, #192, #193, #194,
    #195, #196, #197, #198, #199, #200, #201, #202, #203, #204, #205, #206,
    #207, #208, #209, #210, #211, #212, #213, #214, #215, #216, #217, #218,
    #219, #220, #221, #222, #223, #224, #225, #226, #227, #228, #229, #230,
    #231, #232, #233, #234, #235, #236, #237, #238, #239, #240, #241, #242,
    #243, #244, #245, #246, #247, #248, #249, #250, #251, #252, #253, #254,
    #255);

function FastUpperCase(const s: string): string;
{Fast uppercase}
var
  i: Integer;
begin
  FastUpperCase := s;
  for i := 1 to Length(s) do FastUpperCase[i] := chartab[s[i]]; {Fast uppercase}
end;

function CheckIdent(data: PCs2PascalParser): Boolean;
{Check if an identifier is a reserved word}
var
  s: string;
begin
  s := FastUppercase(GetToken(Data));
  if s = 'AND' then
    Data^.CurrTokenId := CSTII_and else ;

  if s = 'BEGIN' then
    Data^.CurrTokenId := CSTII_begin
  else

    if s = 'CASE' then
      Data^.CurrTokenId := CSTII_case
    else

      if s = 'CONST' then
        Data^.CurrTokenId := CSTII_const
      else

        if s = 'DIV' then
          Data^.CurrTokenId := CSTII_div
        else

          if s = 'DO' then
            Data^.CurrTokenId := CSTII_do
          else

            if s = 'DOWNTO' then
              Data^.CurrTokenId := CSTII_downto
            else

              if s = 'ELSE' then
                Data^.CurrTokenId := CSTII_else
              else

                if s = 'END' then
                  Data^.CurrTokenId := CSTII_end
                else

                  if s = 'FOR' then
                    Data^.CurrTokenId := CSTII_for
                  else

                    if s = 'FUNCTION' then
                      Data^.CurrTokenId := CSTII_function
                    else

                      if s = 'IF' then
                        Data^.CurrTokenId := CSTII_if
                      else

                        if s = 'IN' then
                          Data^.CurrTokenId := CSTII_in
                        else

                          if s = 'MOD' then
                            Data^.CurrTokenId := CSTII_mod
                          else

                            if s = 'NOT' then
                              Data^.CurrTokenId := CSTII_not
                            else

                              if s = 'OF' then
                                Data^.CurrTokenId := CSTII_of
                              else

                                if s = 'OR' then
                                  Data^.CurrTokenId := CSTII_or
                                else

                                  if s = 'PROCEDURE' then
                                    Data^.CurrTokenId := CSTII_procedure
                                  else

                                    if s = 'PROGRAM' then
                                      Data^.CurrTokenId := CSTII_program
                                    else

                                      if s = 'REPEAT' then
                                        Data^.CurrTokenId := CSTII_repeat
                                      else

                                        if s = 'SET' then
                                          Data^.CurrTokenId := CSTII_set
                                        else

                                          if s = 'SHL' then
                                            Data^.CurrTokenId := CSTII_shl
                                          else

                                            if s = 'SHR' then
                                              Data^.CurrTokenId := CSTII_shr
                                            else

                                              if s = 'THEN' then
                                                Data^.CurrTokenId := CSTII_then
                                              else

                                                if s = 'TO' then
                                                  Data^.CurrTokenId := CSTII_to
                                                else

                                                  if s = 'TYPE' then
                                                    Data^.CurrTokenId := CSTII_type else

                                                    if s = 'UNTIL' then
                                                      Data^.CurrTokenId := CSTII_until else

                                                      if s = 'USES' then
                                                        Data^.CurrTokenId := CSTII_uses else

                                                        if s = 'VAR' then
                                                          Data^.CurrTokenId := CSTII_var else

                                                          if s = 'WHILE' then
                                                            Data^.CurrTokenId := CSTII_while else

                                                            if s = 'WITH' then
                                                              Data^.CurrTokenId := CSTII_with else

                                                              if s = 'XOR' then
                                                                Data^.CurrTokenId := CSTII_xor else

                                                              begin
                                                                CheckIdent := False;
                                                                Exit;
                                                              end;
  CheckIdent := True;
end;

procedure ParseToken(data: PCs2PascalParser);
{Parse the token}
var
  ct, ci: LongInt;
  hs: Boolean;
begin

  ct := data^.CurrTokenPos;
  case data^.Text[ct] of
    #0:
      begin
        Data^.CurrTokenId := CSTI_EOF;
        Data^.CurrTokenLen := 0;
      end;
    'A'..'Z', 'a'..'z', '_':
      begin
        ci := ct + 1;
        while (data^.Text[ci] in ['_', '0'..'9', 'a'..'z', 'A'..'Z']) do begin
          Inc(ci);
        end;
        Data^.CurrTokenLen := ci - ct;
        if not CheckIdent(data) then
        begin
          Data^.CurrTokenId := CSTI_Identifier;
        end;
      end;
    '$':
      begin
        ci := ct + 1;
        while (data^.Text[ci] in ['0'..'9', 'a'..'z', 'A'..'Z']) do begin
          Inc(ci);
        end;
        Data^.CurrTokenId := CSTI_HexInt;
        Data^.CurrTokenLen := ci - ct;
      end;

    '0'..'9':
      begin
        hs := False;
        ci := ct;
        while (data^.Text[ci] in ['0'..'9']) do begin
          Inc(ci);
          if (data^.Text[ci] = '.') and (not hs) then
          begin
            hs := True;
            Inc(ci);
          end;
        end;
        if hs then
          Data^.CurrTokenId := CSTI_Real
  else

    Data^.CurrTokenId := CSTI_Integer;
    Data^.CurrTokenLen := ci - ct;
  end;
  #39:
  begin
    ci := ct + 1;
    while (data^.Text[ci] <> #0) and (data^.Text[ci] <> #13) and
      (data^.Text[ci] <> #10) and (data^.Text[ci] <> #39)
      do begin
      Inc(ci);
    end;
    if data^.Text[ci] = #39 then
      Data^.CurrTokenId := CSTI_String
    else

      Data^.CurrTokenId := CSTI_StringError;
    Data^.CurrTokenLen := ci - ct + 1;
  end;
  '#':
  begin
    ci := ct + 1;
    if data^.Text[ci] = '$' then
    begin
      while (data^.Text[ci] in ['A'..'Z', 'a'..'z', '0'..'9']) do begin
        Inc(ci);
      end;
      Data^.CurrTokenId := CSTI_Char;
      Data^.CurrTokenLen := ci - ct - 1;
    end else
    begin
      while (data^.Text[ci] in ['0'..'9']) do begin
        Inc(ci);
      end;
      if data^.Text[ci] in ['A'..'Z', 'a'..'z', '_'] then
        Data^.CurrTokenId := CSTI_CharError
      else

        Data^.CurrTokenId := CSTI_Char;
      Data^.CurrTokenLen := ci - ct;
    end;
  end;
  '=':
  begin
    Data^.CurrTokenId := CSTI_Equal;
    Data^.CurrTokenLen := 1;
  end;
  '>':
  begin
    if data^.Text[ct + 1] = '=' then
    begin
      Data^.CurrTokenid := CSTI_GreaterEqual;
      Data^.CurrTokenLen := 2;
    end else
    begin
      Data^.CurrTokenid := CSTI_Greater;
      Data^.CurrTokenLen := 1;
    end;
  end;
  '<':
  begin
    if data^.Text[ct + 1] = '=' then
    begin
      Data^.CurrTokenId := CSTI_LessEqual;
      Data^.CurrTokenLen := 2;
    end else
      if data^.Text[ct + 1] = '>' then
      begin
        Data^.CurrTokenId := CSTI_NotEqual;
        Data^.CurrTokenLen := 2;
      end else
      begin
        Data^.CurrTokenId := CSTI_Less;
        Data^.CurrTokenLen := 1;
      end;
  end;
  ')':
  begin
    Data^.CurrTokenId := CSTI_CloseRound;
    Data^.CurrTokenLen := 1;
  end;
  '(':
  begin
    if Data^.Text[ct + 1] = '*' then
    begin
      ci := ct + 1;
      while (Data^.Text[ci] <> #0) do begin
        if (Data^.Text[ci] = '*') and (Data^.Text[ci + 1] = ')') then
          Break;
        Inc(ci);
      end;
      if (Data^.Text[ci] = #0) then
        Data^.CurrTokenId := CSTI_CommentEOFError
      else
      begin
        Data^.CurrTokenId := CSTI_Comment;
        Inc(ci);
      end;
      Data^.CurrTokenLen := ci - ct;
    end
    else
    begin
      Data^.CurrTokenId := CSTI_OpenRound;
      Data^.CurrTokenLen := 1;
    end;
  end;
  '[':
  begin
    Data^.CurrTokenId := CSTI_OpenBlock;
    Data^.CurrTokenLen := 1;
  end;
  ']':
  begin
    Data^.CurrTokenId := CSTI_CloseBlock;
    Data^.CurrTokenLen := 1;
  end;
  ',':
  begin
    Data^.CurrTokenId := CSTI_Comma;
    Data^.CurrTokenLen := 1;
  end;
  '.':
  begin
    Data^.CurrTokenId := CSTI_Period;
    Data^.CurrTokenLen := 1;
  end;
  ';':
  begin
    Data^.CurrTokenId := CSTI_Semicolon;
    Data^.CurrTokenLen := 1;
  end;
  ':':
  begin
    if Data^.Text[ct + 1] = '=' then
    begin
      Data^.CurrTokenId := CSTI_Assignment;
      Data^.CurrTokenLen := 2;
    end else
    begin
      Data^.CurrTokenId := CSTI_Colon;
      Data^.CurrTokenLen := 1;
    end;
  end;
  '+':
  begin
    Data^.CurrTokenId := CSTI_Plus;
    Data^.CurrTokenLen := 1;
  end;
  '-':
  begin
    Data^.CurrTokenId := CSTI_Minus;
    Data^.CurrTokenLen := 1;
  end;
  '*':
  begin
    Data^.CurrTokenId := CSTI_Multiply;
    Data^.CurrTokenLen := 1;
  end;
  '/':
  begin
    if data^.Text[ct + 1] = '/' then
    begin
      ci := ct + 1;
      while (Data^.Text[ci] <> #0) and (Data^.Text[ci] <> #13) and
        (Data^.Text[ci] <> #10) do begin
        Inc(ci);
      end;
      if (Data^.Text[ci] = #0) then
        Data^.CurrTokenId := CSTI_CommentEOFError
      else
      begin
        if Data^.Text[ci + 1] = #10 then
          Inc(ci) else

          if Data^.Text[ci + 1] = #13 then
            Inc(ci);
        Data^.CurrTokenId := CSTI_Comment;
      end;
      Data^.CurrTokenLen := ci - ct + 1;
    end else
    begin
      Data^.CurrTokenId := CSTI_Divide;
      Data^.CurrTokenLen := 1;
    end;
  end;
  #32, #9, #13, #10:
  begin
    ci := ct + 1;
    while (Data^.Text[ci] in [#32, #9, #13, #10]) do begin
      Inc(ci);
    end;
    Data^.CurrTokenId := CSTI_Whitespace;
    Data^.CurrTokenLen := ci - ct;
  end;
  '{':
  begin
    ci := ct + 1;
    while (Data^.Text[ci] <> #0) and (Data^.Text[ci] <> '}') do begin
      Inc(ci);
    end;
    if (Data^.Text[ci] = #0) then
      Data^.CurrTokenId := CSTI_CommentEOFError
    else
      Data^.CurrTokenId := CSTI_Comment;
    Data^.CurrTokenLen := ci - ct + 1;
  end;
  else
    Data^.CurrTokenId := CSTI_SyntaxError;
    Data^.CurrTokenLen := 1;
end;
end;

function GetToken(data: PCs2PascalParser): string;
{Return the token}
var
  i: LongInt;
  len: LongInt;
  s: string;
begin
  len := data^.CurrTokenLen;
{$IFDEF SS}
  len := len and $FF;
  s[0] := Chr(len);
{$ELSE}
  setlength(s, len);
{$ENDIF}
  for i := 1 to Len do begin
    s[i] := data^.Text[data^.CurrTokenPos + i - 1];
  end;
  GetToken := s;
end;

procedure NextNoJunk(data: PCs2PascalParser);
{Next no junk}
begin
  Data^.CurrTokenPos := Data^.CurrTokenPos + Data^.CurrTokenLen;
  ParseToken(data);
  while (Data^.CurrTokenId = CSTI_COMMENT) or (Data^.CurrTokenId =
    CSTI_WHITESPACE) do
  begin
    if (Data^.CurrTokenId = CSTI_EOF) or (Data^.CurrTokenId = CSTI_SyntaxError)
      or
      (Data^.CurrTokenId = CSTI_CommentEOFError) or (Data^.CurrTokenId =
      CSTI_CharError) or
      (Data^.CurrTokenId = CSTI_StringError)
      then
    begin
      Break;
    end;
    Data^.CurrTokenPos := Data^.CurrTokenPos + Data^.CurrTokenLen;
    ParseToken(data);
  end;
end;


end.

