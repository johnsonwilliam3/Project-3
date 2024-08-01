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

Windows specific commands are currently in use (Sleep) - Resolved
Let me know if you use Mac or Linux and Ill add Preprocessor Directives to adjust for other OSs - Resolved

After the program is done running, it calls a command to run a python program before shutting down
This can be used to load output data from the c++ program and run an interactive map.
I like the idea of utilizing the folium API for python to make this work.
Tableau seems fine as well but may have a money issue attached. We'll See. - William

Lots of algorithmic programming implemented. I have a lot of ideas about how to output data so that
it can be visualized in Python. The coordinates of events are accurate in that when plugged
into folium, the right region shows on the map. I will have to see how they can be colored for
the final product. - William

Custom Priority Queue should be fully implemented and complete. I'm unsure if we need it,
but it was very easy to finish so I just did it anyway. The function Algorithm::extractN 
pops the most important nodes relative to finalIndex. If we would like to change this to be based on congestion or any 
other index, this can be quickly done. - William