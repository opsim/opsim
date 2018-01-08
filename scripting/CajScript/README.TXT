CajScript 2.16


Files:
License.txt          - License agreement.
History.txt          - Version history.
cascript.html        - Instructions.
cs2.pas              - Cajscript 2.0 engine.
cs2_var.pas          - Cajscript 2.0 variable management.
cs2_utl.pas          - Cajscript 2.0 parser.
cs2test.tst          - Test file for CajScript.
Demo_Del             - Directory to a demo for delphi.
Demo_Fpc             - Directory to a demo for FreePascal.
Libraries            - Directory with some usefull libraries.

The syntax of CajScript is almost the same as Delphi, except
that objects/classes and pointers are not supported yet.

Here is a summary of what now is supported:
procedure/Function
Uses
If
Begin (also nested)
Array (Only one dimension)
+ - * / 
div mod 
= > < <> >= <=
and or xor not
for/to/downto/do,
While/Begin/End
Case/Of/End

To do (in this order):
 - Libraries
 - Repeat/Until (maybe, because it is not really needed and when I add 
		 it, I have to change CajScript too much)
