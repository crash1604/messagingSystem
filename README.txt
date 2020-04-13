The following project is made for Dr. Huizhu Liu
for csci 370

made by chanakya sharma
	email:- chanakya1604@hotmail.com

Introduction
A basic message exchanging application developed in c++
	-major dependencies
		-ncurses:- for making windows 
		-occi.h:- for  database connection

start with make command in the directory to install the executable
Run sqlplus and then run iniit.sql in there to create tables and constraints and also load sample data

NOTE--This is not a realtime messaging app system as it is limited by a single thread
new messasges in a chat will be displayed after the chat log is selected  and then reselected.

Walkthrough
Security standards are set to a minimal so there won't be any password protection in any case

in cmd start with the following command
$./main
then it will prompt for database username and password

fill those out 

if u want to make a new account just type 'n' and press enter
 if not enter your user name then password

if you pressed 'n' it will ask you to chose a username
 then will prompt you to create a password then when you return it will again ask you for your password
enter your password again and load the window


if login was a success it will load the chat window
window on the upper screen will capture all the user input
window on the left will load all the conversations
window on thr right will have all the converstaion content with a specific user

you can use up and down arrow keys to select the conversation you wish to initiate.
you are allowed to select the conversations with by pressing enter
if you wish to chat with a new user press n can only be used 10 times as it is the set limit other wise will lead to a segmentation fault.
type the username and program willlook for the user. If the user was found your conversations will be displayed with them


once you hit enter in the menu window
the control goes to window on the top know as input window
it will record all the keystrokes;
beware because there is no backspace enabled
once you hit enter you can see your message but will not know if you were responded back till you reselect the conversation

if you wish to escape this window
press esc key (ascii code 27)
now if you wish to exit the application 
press tab key and the program will end cleanly.

Open Issues
-Single thread makes it difficult to become a realtime messaging application
-security is set to a minimum 
-open issues with the ncurses windows may arise
-some invalid inputs will lead to termination of the program with sql error

Application Layout
main.cpp
functions:-
	createnewUser(Connection,Statement *&s):-creates a new user
						further takes input from userabout their username and password
another.cpp
functions:-
	int getRecieverCount(Connection *conn,Statement *&stmt,int session)
		returns the number of conversations from one user where session is the logged in id
	int getUserID(WINDOW *userwin, Connection *conn, Statement *&s)
		takes input in a user window and looksup the first occurence of username and returns the id associated with it
	string getUserName(Connection *conn,Statement *& s, int id)
		takes a user id and maps it with the username and returns the username
	void loadWindow(WINDOW *win, Connection *conn, Statement *&s, int uid , int recieverid)
		loads the messages in the window win about the conversation between two users uid (the sender) and recievereid (the reciever)
		NOTE:::::must be run on a multi thread level for realtime system
	void enterMessage(WINDOW *inputwin, WINDOW *win,Connection *conn, Statement *& s, int uid, int rid)
		takes the input in inputwin and then sends it to the database with sender as uid and reciever as rid, then Window *win is used to call the loadWindow function which refreshes the conversation between the users
	int Preview(int uid, Connection *conn, Statement *&s, int list[],int listSize)
		called from main function. loads all the windows and intialises values to them start entering data in the menu window.
		takes session id as uid which determines the current user
		list will contain all the conversation ids and list size is helping to map it on the size changes as list is 10 greater than the actual conversation size for expansion.


Acknowledgement
Thanks csci helpcenter for introducing me to the concept of ncurses.