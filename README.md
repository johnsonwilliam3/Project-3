*READ ME*
Project 3 - 3OP3530 DSA
All c++ headers found in src/Headers
All class definitions found in src/Definitions
main.cpp is located in src and compiles to an exe in the build folder
Run command make in the command line to run the makefile and the exe, starting the application

I have provided some basic structure for the Graph Data Stucure. 
For now, vertices only have a single integer value.
However, it is a functional graph where edges can be created.

Custom Priority Queue in the works. Based on a max heap which is implented using an array
I'm currently not entirely sure how we wan to utilize these ADTs so they only have basic functionality
Dynamic Memory functionality included (Copy Constructor, assignment operator, destructor)

Windows specific commands are currently in use (Sleep)
Let me know if you use Mac or Linux and Ill add Preprocessor Directives to adjust for other OSs

After the program is done running, it calls a command to run a python program before shutting down
This can be used to load output data from the c++ program and run an interactive map.
I like the idea of utilizing the folium API for python to make this work.
Tableau seems fine as well but may have a money issue attached. We'll See.