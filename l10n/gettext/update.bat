::create the .pot file with rstconv
rstconv -i hello.rsj -o locale/hello.pot

::for new translation files use msginit
::msginit --input=locale/hello.pot --locale=fr --output=locale/fr/hello.po

::to merge translations do msgmerge
msgmerge --update locale/nl/hello.po locale/hello.pot

::create the mo files
msgfmt --output-file=locale/nl/hello.mo locale/nl/hello.po
msgfmt --output-file=locale/fr/hello.mo locale/fr/hello.po