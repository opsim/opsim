program test_translate;

{$mode objfpc}

uses
  SysUtils, gettext;

resourcestring
  rsAtAGivenMoment = 'At a given moment';

//change to: es or nl for translations or %s for the locale language
//anything else will lead to the default English language
const
  LANG = 'es';

begin
  //put this function call in the beginning of the application to
  //ensure the translation of all translatable strings, be sure the load the
  //LANG variable from the preferences to load the correct language
  TranslateResourcestrings('locale/' + LANG + '/LC_MESSAGES/messages.mo');

  writeln(rsAtAGivenMoment);
  readln;
end.
