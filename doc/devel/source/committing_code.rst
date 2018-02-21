Procedures to send and commit code
==================================

.. note:: Please, take very seriously that GitHub provides no mechanism to delete permanently a piece of code previously committed. Git provides no means for removing this information, and since the repository is unrestricted for download, once the code is there it is free open source forever. Anyone can have it.

Sending Third Party Code
------------------------
Another important point is that if you have commit access to the repository, and receive directly from someone else a piece of code. Make sure that the contributor explicitly authorizes the publication. One way to document this is by getting the clause "I understand that the code becomes Open Source and GPL" written in the email message containing the code. If necessary, kindly ask the contributor to resend it. Therefore, just for legal precaution and to insure every contributor is remembered, each source file should have a header. The header contains among other things the following information:

    - the GPL2 license information
    - the date when the file was created (initial revision date)
    - an abstract describing the function of the unit
    - and the list of authors (contributors) of that module

If you are about to create a new file (unit), feel free to add the header by using the tool ``addheader`` that is located in the ``tools`` directory.

.. code-block:: bash

   >addheader myunit.pas


Basic rules before initializing a pull request
----------------------------------------------

For now, the rules for committing code to repository are fairly simple:

  - Do not break the build. Your code should compile well and not contain visible
    defects. If your local code base compile but generates an access violation,
    then do not commit it. Fix first.
  - If in doubt, pay attention to the first rule.


About pull request
------------------

Pull requests let you tell others about changes you've pushed to a repository on GitHub. Once a pull request is opened, you can discuss and review the potential changes with collaborators and add follow-up commits before the changes are merged into the repository.

.. note:: When working with pull requests, keep the following in mind: If you're working in the shared repository model, we recommend that you use a topic branch for your pull request. While you can send pull requests from any branch or commit, with a topic branch you can push follow-up commits if you need to update your proposed changes. When pushing commits to a pull request, don't force push. Force pushing can corrupt your pull request.

After initializing a pull request, you'll see a review page that shows a high-level overview of the changes between your branch (the compare branch) and the repository's base branch. You can add a summary of the proposed changes, review the changes made by commits, add labels, milestones, and assignees, and @mention individual contributors or teams. For more information, see "`Creating a pull request <https://help.github.com/articles/creating-a-pull-request>`_".
