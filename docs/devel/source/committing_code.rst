Procedures to send and commit code
==================================

.. note:: Please, take very seriously that SourceForge provides no mechanism to delete permanently a piece of code previouslysent. SVN provides no means for removing this information, and since the repository is unrestricted for download (checkout)bySourceForge, then once the code is there it is free open source forever. Anyone can have it.

Sending Third Party Code
------------------------
Another important point is that if you have commit access to the repository, and receive directly from someone else a piece o code. Make sure that the contributor explicitly authorizes the publication. One manner to document this is by getting the clause "Please put on Source Forge. I understand that the code becomes Open Source and GPL" written in the email message containing the code. If necessary, kindly ask the contributor to resend it. Therefore, just for legal precaution and to insure every contributor is remembered, each source file might have a header. It was designed based on other open source projects, like Lazarus itself. The header contains among other information:

    - the date when the file was created (initial revision date); 
    - a disclaimer; 
    - and the list of authors (contributors) of that module.

If you are about to create a new file (unit), feel free to borrow the header form an existent one and make the proper changes. For example, bellow is a sample header. This header pertains to the file ExtGraphics2.pas. It was a great contribution of a developer named Seppo because helps a lot to render the PFD images. I have just made a few improvements. 

The first line “{ $Id$ }” is a SNV token that expands to information about the file.

.. code:: pascal

  { $Id$ }
  { 
    /***************************************************************************
        Abstract: Painting routines for special objects. 
		Initial Revision : 19/03/2006
		Authors: 
			- Seppo 
			- Samuel Jorge Marques Cartaxo 
			- Additional contributors...
		** Some routines here are from the Lazarus Component 
		   Library (unit ExtGraphics.pas). 
		** New routines were added by Seppo. 
		** Since the new routines were not included in the 
		   Lazarus code base till now (current SVN checkout), 
		   and to avoid conflicts with the original unit, 
		   a different name was used.
  ***************************************************************************/
  ***************************************************************************** 
  * 																		  *
  * This file is part of the OpSim - OPEN SOURCE PROCESS SIMULATOR 			  * 
  * 																		  * 
  * See the file COPYING.GPL, included in this distribution, 				  * 
  * for details about the copyright. 										  * 
  * 																		  *
  * This program is distributed in the hope that it will be useful, 		  * 
  * but WITHOUT ANY WARRANTY; without even the implied warranty of 			  * 
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 					  * 
  * 																		  * 
  ***************************************************************************** }

Basic Principles for Committing Code
------------------------------------

For now, the rules for committing code to repository are fairly simple:

  - Do not break the build. Your code should compile well and not contain visible 
    defects. If your local code base compile but generates an access violation, 
    then do not commit it. Fix first. 
  - If in doubt, pay attention to the first rule.