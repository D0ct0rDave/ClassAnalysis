-------------------------------------------------------------------
Class Analyzer Library 

Copyright (C) 2009 David Márquez de la Cruz.
dementedworldatgmaildotcom
-------------------------------------------------------------------



AIM OF THE PROJECT
------------------
The aim for this library is to provide means to analyze the header files
belonging to a given project, and retrieve a list of the definitions found
inside of them.

The initial purpose for the library was the development of an application to
check the syntax of a given library/framework/engine and retrieve information
about what clases or definitions were not meeting a set of syntax rules provided.

For instance, I wanted to check what classes and definitions in  my own 3D engine 
were not meeting my simplified set of hungarian notation syntax rules, but I had 
no means to analyze them, so I first investigated for free applications to perform 
such task with no success. Then I tried to look for some source code to parse the
headers files to retrieve a list of the found definitions, which then,
I should parse to check for the correct syntax of its elements. In the end, 
I didn't anything that meet my needs, so this is the reason I started doing it by
myself.

The library itself is not a sophistication in sofware enginering or
programming language science, in fact this is far from its pretensions. It just a 
piece of software that tries to analyze some classes and retrieve information from 
them :)


TODO: Class Analyzer Library.
-----------------------------
- Document the library using Doxygen style comments.
- Create a state machine diagram to depict the class analysis mechanic.
- Fix as many bugs as possible!
- Support for the remaining features.

TODO: SyntaxChecker.
--------------------
- Probably make an application to search/replace the occurrences of elements
that does not meet the provided syntax specification.

LICENSE:
--------
The project is released under the GNU GPL terms license. Please see license-gpl.txt
for the details.


This library currently uses Deelx Regular Expression Engine by Sswater Shi 
Copyright 2006 (c) RegExLab.com
All Rights Reserved.

The sample applications use icons borrowed from the Eclipse IDE project.
