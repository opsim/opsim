program hello;

{$mode objfpc}{$H+}

uses
  gettext;

resourcestring
  rsHelloWorld = 'Hello World';
  rsHelloAgain = 'Hello Again';

var
  lang, fallbacklang: string;
  
begin
  GetLanguageIDs(lang, fallbacklang);

  writeln('        lang: ', lang);
  writeln('fallbacklang: ', fallbacklang);
  writeln;

  TranslateResourcestrings('locale/%s/hello.mo');
  writeln(rsHelloWorld);
  writeln(rsHelloAgain);

  readln;
end.
