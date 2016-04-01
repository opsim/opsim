(*
  This program tests the differences between classes and records. It tries to
  demonstrate the differences in;
    - memory consumption
    - creation speed
    - assign value speed
*)

program classtest;

uses
  Classes,
  SysUtils;

type
  TTestRecord = record
    MyDouble1: double;
    MyDouble: double;
    MyInt:    integer;
    MyWord:   word;
  end;

  { TTestClass }

  TTestClass = class(TObject)
  private
    MyDouble1: double;
    FMyDouble: double;
    FMyInt:    integer;
    FMyWord:   word;
    procedure SetMyDouble(AValue: double);
    procedure SetMyInt(AValue: integer);
    procedure SetMyWord(AValue: word);

  protected

  public

    constructor Create;
    destructor Destroy; override;

    property MyInt: integer read FMyInt write SetMyInt;
    property MyWord: word read FMyWord write SetMyWord;
    property MyDouble: double read FMyDouble write SetMyDouble;
  published
  end;

  { TTestClass }

  procedure TTestClass.SetMyInt(AValue: integer);
  begin
    if FMyInt = AValue then
      Exit;
    FMyInt := AValue;
  end;

  procedure TTestClass.SetMyDouble(AValue: double);
  begin
    if FMyDouble = AValue then
      Exit;
    FMyDouble := AValue;
  end;

  procedure TTestClass.SetMyWord(AValue: word);
  begin
    if FMyWord = AValue then
      Exit;
    FMyWord := AValue;
  end;

  constructor TTestClass.Create;
  begin
    inherited Create;

    MyDouble := 0;
    MyInt    := 0;
    MyWord   := 0;
  end;

  destructor TTestClass.Destroy;
  begin
    inherited Destroy;
  end;

const
  Count = 10000000;

var
  ClassArray   : array [1..Count] of TTestClass;
  RecordArray : array [1..Count] of ^TTestRecord;
  MyClass     : TTestClass;
  MyRecord    : ^TTestRecord;
  StartTime   : TDateTime;
  i           : Integer;
begin
  MyClass := TTestClass.Create;
  writeln('Tested class size      ', MyClass.InstanceSize, 'bytes');
  FreeAndNil(MyClass);
  writeln('Tested record size     ', sizeof(TTestRecord), 'bytes');

  writeln;
  writeln('Running ', Count, ' tests');
  writeln;

  write('Start making classes...');
  readln;
  for i := 1 to Count do
    ClassArray[i] := TTestClass.Create;
  writeln('done');
  readln;
  for i := 1 to Count do
    ClassArray[i].Free;

  write('Start making records...');
  readln;
  for i := 1 to Count do
    RecordArray[i] := getmem(sizeof(TTestRecord));
  writeln('done');
  readln;
  for i := 1 to Count do
    freemem(RecordArray[i]);
  readln;

  //class creation
  StartTime := Now;
  for i := 1 to Count do
  begin
    MyClass := TTestClass.Create;
    FreeAndNil(MyClass);
  end;
  writeln('Tested class creation in    ', FloatToStrF((Now - StartTime) * 24 * 3600 * 1000, ffFixed, 15, 0), ' ms');

  //record creation
  StartTime := Now;
  for i := 1 to Count do
  begin
    MyRecord := getmem(sizeof(TTestRecord));
    freemem(MyRecord);
  end;
  writeln('Tested record creation in   ', FloatToStrF((Now - StartTime) * 24 * 3600 * 1000, ffFixed, 15, 0), ' ms');

  //class assign values
  MyClass   := TTestClass.Create;
  StartTime := Now;
  for i := 1 to Count do
  begin
    MyClass.MyDouble := i;
    MyClass.MyInt    := i;
    MyClass.MyWord   := i;
  end;
  writeln('Tested class assignment in  ', FloatToStrF((Now - StartTime) * 24 * 3600 * 1000, ffFixed, 15, 0), ' ms');
  FreeAndNil(MyClass);

  //class assign values
  MyRecord  := getmem(sizeof(TTestRecord));
  StartTime := Now;
  for i := 1 to Count do
  begin
    MyRecord^.MyDouble := i;
    MyRecord^.MyInt    := i;
    MyRecord^.MyWord   := i;
  end;
  writeln('Tested record assignment in ', FloatToStrF((Now - StartTime) * 24 * 3600 * 1000, ffFixed, 15, 0), ' ms');
  freemem(MyRecord);

  readln;
end.
