{$IFDEF VER130}{D5}{$B-}{$DEFINE DELPHI}{$DEFINE CLASS}{$ENDIF}
{$IFDEF VER120}{D4}{$B-}{$DEFINE DELPHI}{$DEFINE CLASS}{$DEFINE I32}{$ENDIF}
{$IFDEF VER100}{D3}{$B-}{$DEFINE DELPHI}{$DEFINE CLASS}{$DEFINE I32}{$ENDIF}
{$IFDEF VER90}{D2}{$B-}{$DEFINE DELPHI}{$DEFINE CLASS}{$ENDIF}
{$IFDEF VER80}{D1}{$B-}{$DEFINE DELPHI}{$DEFINE SS}{$ENDIF}

{$IFDEF VER125}{C4}{$B-}{$DEFINE CBUILDER}{$DEFINE I32}{$ENDIF}
{$IFDEF VER110}{C3}{$B-}{$DEFINE CBUILDER}{$DEFINE I32}{$ENDIF}
{$IFDEF VER93}{C1}{$B-}{$DEFINE CBUILDER}{$ENDIF}

{$IFDEF VER70}{BP7}{$B-}{$N+}{$DEFINE BP}{$DEFINE SS}{$ENDIF}

{$IFDEF FPC}{FPC}{$DEFINE SS}{$ENDIF}


{
Defines:
  DELPHI        - Delphi is used
  CBUILDER      - CBuilder is used
  FPC           - Free Pascal is used
  CLASS         - Script engine is defined as a class
  SS            - Shortstrings are used
  I32           - 32bits Pascal/Delphi version is used
} 
