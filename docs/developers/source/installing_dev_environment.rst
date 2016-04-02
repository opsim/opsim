Lazarus/Firebird/Zeoslib Installation Procedures
================================================

For Windows
^^^^^^^^^^^

.. note:: Lazarus and the Zeos Database Libraries are under fairly intense development and so these installation procedures may need to be changed as the software matures.

The best components for using the firebird database, have just been recentlyportedto Lazarus from Delphi. In the process, several > errors were discovered in the released version of the Freepascal compiler. The problem has been fixed but is not in a release version. Therefore this procedure will install the beta version of the compiler.

Install Firebird
----------------

Download the Firebird 2.0 beta classic and superserver installer from: http://prdownloads.sourceforge.net/firebird/Firebird-2.0.0.12169-0-Win32.exe?download Download the file and install the classic server. Have it start up as a service and have the service start. This is a beta but appears to be in prettygood shape. The database file format has changed, so we decided that instead of working in 1.5 and then porting to 2.0 when it comes out, we would use 2.0 since there don't seem to be any major problems. To get this configuration, just press the Next bottom on every installation window. Nothing else is needed. 

Install Lazarus
---------------
If you have Lazarus installed, uninstall it. Download the daily snapshot of Lazarus with fpc here: http://www.de.freepascal.org/lazarus/ Choose the "Lazarus - Full" Option. This may take a while to download. It seems to be a slow connection. Install this on your computer. This will install FPC 2.1.1 and the latest Lazarus. 

Install Zeoslib Components
--------------------------
Download the zeoslib components from here: http://www.deq.ufc.br/deq/arquivos/zeosdbo_rework-samuel.zip Samuel has fixed a few problems. I recommend unzipping it into the c:\lazarus directory so that it is C:\lazarus\zeosdbo_rework-samuel Start Lazarus. Click on 'Components' and select 'Open Package File'. Go to the C:\lazarus\zeosdbo_rework-samuel\packages\lazarus\ and select zcomponents.lpk Click on Compile in the Package Window. Watchthe message box. If it shows that ZComponents compiled skip the next step. If it does not compile and complains about trying to find a file, do the following. There is sometimes a problem finding one of the source files. I haven’t tracked it down, but here is a solution. Click on 'Components' and select 'Open Package File'. Go to the C:\lazarus\zeosdbo_rework\packages\lazarus\ and select zplain.lpk Click the Add button. This opens up a box 'Add to Package' At the top click on 'Add Unit', then click the '...' button next to the 'Unit File Name' edit box. Go to the C:\lazarus\zeosdbo_rework-samuel\src\plain\ directory and select: ZPlainSqLiteDriver.pas (Its the last file in the directory) and click open. Then Click on 'Add Unit'. Back in the 'Package Zplain' Package Box, Click on 'Compile' . Inthe messages box, it will do a few things and then say it is compiled. Now go to the 'Package ZComponent' window and click on compile. You will get an error message saying "Ambiguous Units found' Click on Cancel. Go back to the 'Package ZPlain' Box. Scroll down to the last source file, PlainSqLiteDriver.pas, select it and hit 'Remove' and click okay. Click on 'Save'. Go to the 'Package ZComponent' and click on compile. I should now churn away for a bit then then finally say compiled in the message box. If this does not work, let me know and we will work throughthe problem. (nwmatt@gmail.com) Once ZComponent is compiled, do the following: Click on install. It will ask you if you want to recompile Lazarus now. Click yes and get a cup of coffee. It will churn for a few minutes. Then it mayask you if you wish to save changes. Say no. It will restart Lazarus. There should be a new tab onthe component Palette called "Zeos Access"