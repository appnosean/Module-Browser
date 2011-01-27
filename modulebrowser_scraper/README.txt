README.txt
==========

A module that adds a method to get data for projects and releases, for the module browser.
This method uses the standard XML files provided by the projects module and scraping to 
fetch data that is missing in the XML files.

INSTALLATION
============
Install as usual, see http://drupal.org/node/70151 for further information.

USAGE
=====
Provides a drush command: 
drush -l 'multi-site' mb-load-projects

This drush command will fetch the XML files, process them and then use scraping for the extra information.



AUTHOR/MAINTAINER
=================
- Stijn Vanden Brande
stijn DOT v DOT brande at gmail DOT com
SPONSERED BY KRIMSON