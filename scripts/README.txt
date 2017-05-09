To run this create a file like the example below. The server will respond to the message 'C' with the response 'R'. So you can create a testfile with all the commands. Empty lines and '#' lines are ignored.

The server only matches C and R on the command name so there can only be one response.

I suggest you add commenst about the request name parameters and return values. I will then implement the actual functions later.


C request1 hello
R request1 world
C request2 hello2
R request2 world2

mini_server.pl  [-t] [-f <simfile>] [-d <level>] [-h] [-v]
       -t                       # run as test client
       -f <simfile>             # use simulator file
       -d <level>               # debug level
       -h                       # help
       -v                       # verbose output

To test this run the following in one terminal (as server):

	perl mini_server.pl -f testfile.txt

and then in another window as client (as the GUI):

	perl mini_server.pl -f testfile.txt -t 

The test client runs the 'C' command in sequence but the server only stores the last value.
