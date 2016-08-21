Coding Standards
================
The most important rules that developers need to adhere to are:

* When making changes, keep as close as possible to the style and conventions of the surrounding code.
* Strive for clarity, even if that means occasionally breaking the guidelines. Use your head and ask for advice if your common sense seems to disagree with the conventions.
* Comment your code as much as possible where it does make sense.

Language and Encoding
---------------------
There are some general conventions for OpSims code base.

* Use American spelling for all doc-strings variable names and comments.
* Use ASCII where possible, avoid special unicode characters such as '÷', '¶' or 'λ'.
* Use UTF8 encoding for all source files where unicode characters are required. 

Naming
------
* Use descriptive names for global variables and functions.
* Public function names should include module identifier, object and property they're operating and operation itself.

.. code-block:: pascal

   // Don't:
   function find_conversion(uq: pUnitQuantity; unit_: string): pUnitConversion;
   // Do: 
   function UNC_conversion_get(uq: pUnitQuantity; unit_: string): pUnitConversion;

* Local variables should be short and to the point.

Constants, Set types
--------------------
* Names of constants and labels in set types should be capitalized.

Function / procedure arguments
------------------------------
* variables of which function entry value is not used should be declared as an *out* parameter instead of a *var* parameter.

* return arguments should be grouped at the end of the argument list.
* *optionally*, put these arguments on a new line (especially when the argument list is already long and may be split across multiple lines anyway). 

.. code-block:: pascal

   // Don't:
   procedure Kelvin_to_Celcius(v: double; var temp: double);
 
   // Do:
   procedure Kelvin_to_Celcius(v: double; out temp: double);

Operators and Statements
------------------------

* C style operators are allowed to be used (\*=, +=, /= and -=)
* Spaces should be used around assign operators (:=, +=, /= and so on), one space on each side of it: 

.. code-block:: pascal

   // Don't:
   foo:= bar;
   // Do:
   foo := bar;

Space should be used between statement and brace: 

.. code-block:: pascal

   // Don't:
   if(ob = nil)then
   // Do:
   if (ob = nil) then

* The pointer qualifier is defined as type and is a lowercase *p*. 

.. code-block:: pascal

   // Don't:
   s: ^MyType;
   // Do:
   type
     pMyType = ^MyType;
   ...
   s: pMyType;

Case Statement
----------------
* The body of the case statement should be indented, so case keyword has next indentation level relative to switch, body of case statement has got next indentation level relative to case:

.. code-block:: pascal

   // Don't:
   case tok^.token of
   CHF_RPAREN:
   begin
     ...
   end;
   CHF_NUM: 
     val(tok^.value, lmul, code);
   end;
   // Do:
   case tok^.token of
     CHF_RPAREN:
     begin
       ...
     end;
     CHF_NUM: 
       val(tok^.value, lmul, code);
   end;

Executable sections
-------------------
This section covers placement of *begin* and *end* keywords.

The *begin* and *end* keywords should always start on a new line.

.. code-block:: pascal

  // Don't:
  procedure func; begin
    ...
  end;
  // Do:
  procedure func; 
  begin
    ...
  end;

.. code-block:: pascal

   // Don't:
   if a = b then
   begin
       d := 1;
   end else begin
       c := 2;
   end;
   // Do:
   if a = b then 
   begin
     d := 1;
     ...
   end
   else 
   begin
     c := 2;
   end;

Do not skip the *begin* *end* keywords for executable sections when using nested *else* statements. Try to avoid skipping them altogether.
   
Indentation
-----------
* Use two spaces for indentation for each block. 
* The whole idea behind indentation is to clearly define where a block of control starts and ends. Always indent when needed - that is, after if, do, while statements, function declarations, and so on.
* When defining an argument list over multiple lines, the lines preferable be indented with 8 spaces or align to the opening parenthesis for shorter functions. For example: 

.. code-block:: pascal

   // Don't:
   procedure procedure_foo(arg1, arg2, ..., argN,
       argN + 1, ..., argM,
       argM + 1, argM + 2);
 
   procedure procedure_foo_with_a_long_name_that_causes_a_lot_of_indent_alignment(arg1,
                                                                                  arg2,
                                                                                  arg3);
   // Do:
   procedure procedure_foo(
                 arg1, arg2, ..., argN,
                 argN + 1, ..., argM,
                 argM + 1, argM + 2);
 
   procedure short_procedure(arg1,
                             arg2,
                             arg3);
 
   // --- 120 chars long --- //

* When splitting if statement it's preferable to split it on disjunction operation and keeping wrapped conditions: 
 
.. code-block:: pascal

   // in this case wrapped opening brace is reading easier
   if ((tok = CHF_RPAREN) or (tok = CHF_LPAREN)) and
      ((tok = CHF_HYDRATE) or (tok = CHF_NAME)) then
   begin
       ...
   end;

Compile-time directives
-----------------------
* Compile-time directives should never be indented, the brace ({} character) should always be the first character on the line.

.. code-block:: pascal

   // Don't:
   procedure test;
   begin
     {$ifdef WINDOWS}
     a := 1;
     {$else}
     a := 2;
     {$endif}
   end;
 
   // Do:
   procedure test;
   begin
   {$ifdef WINDOWS}
     a := 1;
   {$else}
     a := 2;
   {$endif}
   end;

* When using nested directives it is advised to indent them for better readability. For short blocks this isn't needed, however when blocks of directives become confusing, 2 space indent should be used eg.

.. code-block:: pascal

   // Don't:
   {$IFDEF MSWINDOWS}
   {$IFDEF HAS_TAR_SUPPORT}
   {$DEFINE CREATE_TAR_FILE}
   {$ENDIF HAS_TAR_SUPPORT}
   {$ELSEIF}
   {$IFDEF UNIX}
   {$IFDEF HAS_TAR_SUPPORT}
   {$DEFINE CREATE_TAR_FILE}
   {$ENDIF HAS_TAR_SUPPORT}
   {$ENDIF UNIX}
   {$ENDIF}
 
   // Do:
   {$IFDEF MSWINDOWS}
     {$IFDEF HAS_TAR_SUPPORT}
       {$DEFINE CREATE_TAR_FILE}
     {$ENDIF HAS_TAR_SUPPORT}
   {$ELSEIF}
     {$IFDEF UNIX}
       {$IFDEF HAS_TAR_SUPPORT}
         {$DEFINE CREATE_TAR_FILE}
       {$ENDIF HAS_TAR_SUPPORT}
     {$ENDIF UNIX}
   {$ENDIF}


Trailing Space
--------------
In general, avoid trailing white-space.

*However this doesn't impact on code readability, so avoid large bulk code-cleanups for existing code (unless prior agreement).*

Breaking long lines
-------------------
* Lines should not be longer than 120 characters (columns) long.
* Statements longer than 120 columns will be broken into sensible chunks. Descendants are always substantially shorter than the parent and are placed substantially to the right. The same applies to function headers with a long argument list. Long strings are as well broken into shorter strings. 

Generally the only exceptions are for comments with example commands or URLs - to make cut and paste easier.

The other exception is for those rare cases where letting a line be longer (and wrapping on an 120-character window) is actually a better and clearer alternative than trying to split it into two lines. Sometimes this happens, but it's extremely rare.

**DO NOT** alter somebody else's code to re-wrap lines (or change whitespace) just because you found something that violates the rules. Let the group/author know, and resist the temptation to change it yourself.

Comment style
-------------
* Comments should explain what the code is doing, not how. The how should be more or less obvious from the way the code is written.
 
.. code-block:: pascal

   // this is a line comment
   
   (* This is a block comment 
    *)

* When using multiline comments, markers (star character, '*') should be used in the beginning of every line of comment: 

.. code-block:: pascal

   // Don't:
   (* Lorem ipsum dolor sit amet, 
      consectetur adipiscing elit. *)
      
   // Do:
   (* Lorem ipsum dolor sit amet, 
    * consectetur adipiscing elit. *)

* An XXX marker should be used only in comments, describing usage of a non-obvious solution caused by some design limitations which better be resolved after rethinking of design. 

.. code-block:: pascal

   //XXX: description of issue with code

Source documentation
--------------------
When writing more comprehensive comments that include for example, function arguments and return values, cross references to other functions... etc, we use PasDoc_ syntax comments.

.. _PasDoc: https://pasdoc.sipsolutions.net

If you choose to write PasDoc comments, here's an example of a typical comment for functions and procedures (many more in opsim code).

.. code-block:: pascal

   (**
    * Define a new unit conversion.
    *
    * \param quantity: the base unit this unit belongs to
    * \param unit_: the name of the unit
    * \param multiplier: multiplier, the base unit needs to be multiplied with
    * \param bias: the value offset the base unit needs to be offset with
    * \param flag: flag that determines the unit system so one can filter based on unit system in the UI
    *)
    procedure UNC_define_conversion(
                quantity, unit_  : string;
                multiplier, bias : double;
                flag             : word);

Note that this is just the typical paragraph style comment used in opsim with an extra leading '*'. Additionally also other elements in the code like variables, constants, set types and records can be documented. Please refer to the opsim code or to the PasDoc website for more information.

As for placement of documentation, we try to keep the comments close to the code. Terse, single line descriptions of functions in headers is fine, but for detailed explanations, add this directly about the code.

In summary:

* Use PasDoc comments to document the sourcecode.
* Keep comments close to code.
* Comments in header are optional but keep brief. 