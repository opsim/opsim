This test demonstrates performance differences between classes and records. The 
test is intended to show the memory usage, creation speed and assign speed of 
the differentvariable types.

The test of 10Mio runs was executed on the following machines, with the 
following results;

NOTE: The next comparison data is not correct. For records I have been using 
guarded alloc, while for classes this was not the case. Guarded alloc poses a 
significant overhead. However, even with guarded alloc record the performance is 
at least comparable. 

Windows 32bit
-----------------------------------
Tested class size      32b
Tested record size     24b
Tested class creation in    976 ms
Tested record creation in   974 ms
Tested class assignment in   97 ms
Tested record assignment in  41 ms

Windows 64bit
-----------------------------------
Tested class size      32b
Tested record size     24b
Tested class creation in    2830 ms
Tested record creation in   3418 ms
Tested class assignment in   222 ms
Tested record assignment in   63 ms

Linux 64bit
-----------------------------------
Tested class size      32b
Tested record size     24b
Tested class creation in    1525 ms
Tested record creation in   1239 ms
Tested class assignment in   138 ms
Tested record assignment in   58 ms

Overall one can conclude that the speed gain for records over classes is 
relatively small. On 10 Mio instances the differences are in the order of a few 
seconds at best. Measuring the actual memory consumption of a class is 
suprisingly  difficult. I have tried with MyClass.InstanceSize which seems to 
give back the size of the public variables together with the pointer to the 
class. Probably the pointers to the class functions are stored in a separate 
part of memory. The reported size difference is seemingly not very big but for 
10 Mio instances the difference is 76Mb of RAM or 25%.

================================================================================
The results shown below are without guarded alloc for records. Also the test has 
been adjusted so the actual memory consumption for classes and records can be 
checked by the memory consumption for the application instance e.g. taskmanager 
in Windows.

Windows 32bit
-----------------------------------
10Mio classes take 497 MB or RAM and leave 40 MB of fragmented RAM after release
10Mio records take 343 MB or RAM and leave 38 MB of fragmented RAM after release
Tested class creation in    1076 ms
Tested record creation in    228 ms
Tested class assignment in   112 ms
Tested record assignment in   45 ms

Windows 64bit
-----------------------------------
10Mio classes take 687 MB or RAM and leave 77 MB of fragmented RAM after release
10Mio records take 382 MB or RAM and leave 77 MB of fragmented RAM after release
Tested class creation in    2759 ms
Tested record creation in   1777 ms
Tested class assignment in   215 ms
Tested record assignment in   54 ms

Linux 64bit
-----------------------------------
10Mio classes take 681 MB or RAM and leave 77 MB of fragmented RAM after release
10Mio records take 381 MB or RAM and leave 77 MB of fragmented RAM after release
Tested class creation in    1606 ms
Tested record creation in    372 ms
Tested class assignment in   144 ms
Tested record assignment in   61 ms

The results from these tests indicate that records take up about 30-45% less 
RAM. Creating records is 1,5 to 6 times faster and assigning values is 2.3 to 4 
times faster. The creation time is less critical as this is typically done once 
(when loading a file) for large amounts and the overall time is relatively short 
for both methods. However assigning values is done regularly by the solver and 
in great amounts and can amount to a noticeable or even annoying lag of the UI. 
Also the amount of memory used is important as large models can lead to memory 
paging which lead to a significant drop in performance.