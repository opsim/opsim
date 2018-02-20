Data types
==========

The following information is copied from the `Free Pascal reference quide <https://www.freepascal.org/docs-html/ref/refch3.html>`_.

OpSim's internal data structure only uses a limited amount of data types. Please check here which data type to use.

Real types
----------

Free Pascal uses the math co-processor (or emulation) for all its floating-point calculations. The Real native type is processor dependent, but it is either Single or Double. Only the IEEE floating point types are supported, and these depend on the target processor and emulation options. The true Turbo Pascal compatible types are listed in following table.

+----------+-------------------------+--------------+--------+--------+
| Type     | Range                   | Sign. digits | Size   | OpSim  |
+==========+=========================+==============+========+========+
| Real     | platform dependent      | ???          | 4 or 8 | No     |
+----------+-------------------------+--------------+--------+--------+
| Single   | 1.5E-45 .. 3.4E38       | 7-8          | 4      | No     |
+----------+-------------------------+--------------+--------+--------+
| Double   | 5.0E-324 .. 1.7E308     | 15-16        | 8      | No     |
+----------+-------------------------+--------------+--------+--------+
| Extended | 1.9E-4932 .. 1.1E4932   | 19-20        | 10     | Yes    |
+----------+-------------------------+--------------+--------+--------+
| Comp     | -2E64+1 .. 2E63-1       | 19-20        | 8      | No     |
+----------+-------------------------+--------------+--------+--------+
| Currency | -/+922337203685477.5808 | 19-20        | 8      | No     |
+----------+-------------------------+--------------+--------+--------+

The Comp type is, in effect, a 64-bit integer and is not available on all target platforms. To get more information on the supported types for each platform, refer to the Programmer’s Guide. The currency type is a fixed-point real data type which is internally used as an 64-bit integer type (automatically scaled with a factor 10000), this minimizes rounding errors. 

Integer types
-------------
The integer types, and their ranges and sizes, that are predefined in Free Pascal are listed in the following table. Please note that the QWord and Int64 types are not true ordinals, so some Pascal constructs will not work with these two integer types. 

+----------+---------------------------------------------+---------------+--------+
| Type     | Range                                       | Size in bytes | OpSim  |
+==========+=============================================+===============+========+
| Byte     | 0 .. 255                                    | 1             | Yes    |
+----------+---------------------------------------------+---------------+--------+
| Shortint | -128 .. 127                                 | 1             | No     |
+----------+---------------------------------------------+---------------+--------+
| Smallint | -32768 .. 32767                             | 2             | Yes    |
+----------+---------------------------------------------+---------------+--------+
| Word     | 0 .. 65535                                  | 2             | No     |
+----------+---------------------------------------------+---------------+--------+
| Integer  | either smallint or longint                  | size 2 or 4   | No     |
+----------+---------------------------------------------+---------------+--------+
| Cardinal | longword                                    | 4             | No     |
+----------+---------------------------------------------+---------------+--------+
| Longint  | -2147483648 .. 2147483647                   | 4             | Yes    |
+----------+---------------------------------------------+---------------+--------+
| Longword | 0 .. 4294967295                             | 4             | No     |
+----------+---------------------------------------------+---------------+--------+
| Int64    | -9223372036854775808 .. 9223372036854775807 | 8             | Yes    |
+----------+---------------------------------------------+---------------+--------+
| QWord    | 0 .. 18446744073709551615                   | 8             | No     |
+----------+---------------------------------------------+---------------+--------+

The integer type maps to the smallint type in the default Free Pascal mode. It maps to either a longint in either Delphi or ObjFPC mode. The cardinal type is currently always mapped to the longword type. Remark: All decimal constants which do no fit within the -2147483648..2147483647 range are silently and automatically parsed as 64-bit integer constants as of version 1.9.0. Earlier versions would convert it to a real-typed constant. As a pascal compiler, Free Pascal does automatic type conversion and upgrading in expressions where different kinds of integer types are used: 

  1. Every platform has a ”native” integer size, depending on whether the platform is 8-bit, 16-bit, 32-bit or 64-bit. e.g. On AVR this is 8-bit. 
	 
  2. Every integer smaller than the ”native” size is promoted to a signed version of the ”native” size. Integers equal to the ”native” size keep their signedness. 
	 
  3. The result of binary arithmetic operators (+, -, \*, etc.) is determined in the following way:
  
    a. If at least one of the operands is larger than the native integer size, the result is chosen to be the smallest type that encompasses the ranges of the types of both operands. This means that mixing an unsigned with a smaller or equal in size signed will produce a signed type that is larger than both of them. 

    b. If both operands have the same signedness, the result is the same type as them. The only exception is subtracting (-): in the case of unsigned-unsigned subtracting produces a signed result in FPC (as in Delphi, but not in TP7). 

    c. Mixing signed and unsigned operands of the ”native” int size produces a larger signed result. This means that mixing longint and longword on 32-bit platforms will produce an int64. Similarly, mixing byte and shortint on 8-bit platforms (AVR) will produce a smallint.

Boolean types
-------------
Free Pascal supports the Boolean type, with its two pre-defined possible values True and False. These are the only two values that can be assigned to a Boolean 
type. Of course, any expression that resolves to a boolean value, can also be assigned to a boolean type. 

+----------+------+-------------------+--------+
| Name     | Size | Ord(True)         | OpSim  |
+==========+======+===================+========+
| Boolean  | 1    | 1                 | Yes    |
+----------+------+-------------------+--------+
| ByteBool | 1    | Any nonzero value | No     |
+----------+------+-------------------+--------+
| WordBool | 2    | Any nonzero value | No     |
+----------+------+-------------------+--------+
| LongBool | 4    | Any nonzero value | No     |
+----------+------+-------------------+--------+

Free Pascal also supports the ByteBool, WordBool and LongBool types. These are of type Byte, Word or Longint, but are assignment compatible with a Boolean: the value False is equivalent to 0 (zero) and any nonzero value is considered True when converting to a boolean value. A boolean value of True is converted to -1 in case it is assigned to a variable of type LongBool. 