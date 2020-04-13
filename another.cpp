#include <ncurses.h>
#include <iostream>
#include <string>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;
using namespace oracle::occi;


int getRecieverCount(Connection *conn,Statement *&stmt,int session)
{
	stmt=conn->createStatement("select count( distinct reciever) from messages where sender = :1");
	stmt->setInt(1,session);
	ResultSet *count= stmt->executeQuery();

	int size =0;
	if(count->next())
		size =count->getInt(1);

	stmt->closeResultSet(count);
	return size;
}



int getUserID(WINDOW *userwin, Connection *conn, Statement *&s)
{
	int id=-1;
	mvwprintw(userwin,2,1,"enter the username please");
	char str[16];
	int ch=getch();
	str[0]=ch; int i=1;
	mvwaddch(userwin,3,1,ch);
	while(1)
	{
		ch=getch();
		if(ch==10 && str[0]!=10)
		{
			s=conn->createStatement("select id from pusers where userid= :1");
			s->setString(1,str);
			ResultSet *set=s->executeQuery();
			if(set->next())
			{
				id=set->getInt(1);
			}
			s->closeResultSet(set);
			break;

		}
		else
		{

			str[i]=ch;
			i++;
			mvwaddch(userwin,3,i,ch);
			wrefresh(userwin);
		}
		
	}
	wclear(userwin);
	int left , right, top, bottom, tlc,trc, blc, brc;
        left=right= 124;
        top=bottom = 45;
        tlc=trc=blc=brc=43;
        wborder(userwin,left,right,top,bottom,tlc,trc,blc,brc);
	return id;
}



string getUserName(Connection *conn,Statement *& s, int id)
{
	s=conn->createStatement("select userid from pusers where id =:1");
	s->setInt(1,id);
	ResultSet *ret=s->executeQuery();
	
	string username;
	if(ret->next())
	{
		username=ret->getString(1);
	}
	s->closeResultSet(ret);
	return username;
}


void loadWindow(WINDOW *win, Connection *conn, Statement *&s, int uid , int recieverid)
{
	
	wclear(win);
	int left , right, top, bottom, tlc,trc, blc, brc;
	left=right= 124;top=bottom = 45;
        tlc=trc=blc=brc=43;
        wborder(win,left,right,top,bottom,tlc,trc,blc,brc);

	string sender=getUserName(conn,s,uid);
	string partner= getUserName(conn,s,recieverid);

	s=conn->createStatement("select * from messages where ( sender= :1 and reciever= :2 ) or  (sender= :2 and reciever= :1 ) order by timestamp desc");
	s->setInt(1,uid);
	s->setInt(2,recieverid);
	ResultSet *chatlog = s->executeQuery();

	string message;
	
	int linenum=1;
	while(chatlog->next())
	{
		int sID=chatlog->getInt(1);
		int rID=chatlog->getInt(2);
		message=chatlog->getString(3);
		string q;
		if(sID == uid && rID==recieverid)
		{
			q=sender+":::-"+message;
			mvwprintw(win,linenum, 1,"%s",q.c_str());
		}
		else if(sID==recieverid && rID==uid)
		{
			 q=partner+":::-"+message;
			 mvwprintw(win,linenum, 1, "%s",q.c_str());
		}
		linenum+=2;

	}
	wrefresh(win);
	s->closeResultSet(chatlog); 
	
}




void enterMessage(WINDOW *inputwin, WINDOW *win,Connection *conn, Statement *& s, int uid, int rid)
{
	wmove(inputwin,1,1);
	keypad(inputwin,TRUE);
	int choice=64;

	int left , right, top, bottom, tlc,trc, blc, brc;
        left=right= 124;
        top=bottom = 45;
        tlc=trc=blc=brc=43;
        
	char str[100];
	int i=0;
	choice=getch();
	while(1)
	{
		if(choice==27)
		{
			break;//if we hit escape key at any moment we leave the function
		}
		else if (choice==10 && str[0]!=10)
		{
			//if we hit enter and the first letter of str is not the enter
			 s=conn->createStatement("insert into messages values(:1, :2, :3 , sysdate)");
	                 s->setInt(1,uid);
	                 s->setInt(2,rid);
	                 s->setString(3,str);

			 s->execute();
			 conn->commit();

			 wclear(inputwin);

			                  
	                 wborder(inputwin,left,right,top,bottom,tlc,trc,blc,brc);
	                 wrefresh(inputwin);
			 loadWindow(win, conn, s, uid, rid);
		}
		else
		{
			//else we we write it to a string
                        str[i]=choice;
                        i++;
                        mvwaddch(inputwin,1,i,choice);
                        wrefresh(inputwin);
		}
               
		choice=getch();
	}


}

	



int Preview(int uid, Connection *conn, Statement *&s, int list[],int listSize)
{
        initscr();
	noecho();
	cbreak();
        /*
	 *uid is the session id of the ;logged in user
	 *int list is the reciever list passed in here from main
	 *listSize is helping to map the the menu strings
	 */
	

        int lines=1; 
        int yMax, xMax;
        getmaxyx(stdscr,yMax,xMax);
          
        WINDOW * win = newwin(yMax-5,xMax-30,5, 30);
 	WINDOW * inputwin =newwin(5,xMax,0,0);
             
        WINDOW * menuwin = newwin(yMax, 30,5,0);
        refresh();
        //box(win, (int)c, 0);
        int left , right, top, bottom, tlc,trc, blc, brc;
        left=right= 124;
        top=bottom = 45; 
        tlc=trc=blc=brc=43;
        
        wborder(win,left,right,top,bottom,tlc,trc,blc,brc);
        wborder(inputwin,left,right,top,bottom,tlc,trc,blc,brc);
        wborder(menuwin,left,right,top,bottom,tlc,trc,blc,brc);
	mvwprintw(inputwin,3,1,"press n for new chat\tpress tab to exit\t press enter to send message to selected id");

        mvwprintw(win,lines++,1,"this is my box");       
        wrefresh(win);
        wrefresh(inputwin);
        wrefresh(menuwin);

       //mapping choice with sender list


        keypad(menuwin, true);//helps us use the arrow keys
	//a qquery to initialize the count of number of users having a chat with
        string recieverList[listSize+10];

	for(int i=0;i<listSize;i++)
	{
		recieverList[i]=getUserName(conn, s, list[i]);	

	        mvwprintw(win,i+1,1,recieverList[i].c_str());	
		wrefresh(win);
	}


        int choice;
        int highlight =0;
	while(1)
        {
                for(int i=0;i<listSize;i++)
                {
                        if(i==highlight)
                                wattron(menuwin,A_REVERSE);
                        mvwprintw(menuwin,i+1,1,recieverList[i].c_str());
                        wattroff(menuwin,A_REVERSE);
                }
                choice = wgetch(menuwin);


                switch(choice)
                {
                        case KEY_UP:
                                highlight--;
                                if(highlight==-1)
                                        highlight=0;
                                break;
                        case KEY_DOWN:
                                highlight++;
                                if(highlight==listSize)
                                        highlight=listSize-1;//use size of the array and -1 to it
                                break;
			case 10://press enter \n
				//get the userid from here highlight is the array
				//pass it as an int to input win and connection conn
				//loadwindowfunctioni
				loadWindow(win,conn, s, uid , list[highlight]);
				enterMessage(inputwin,win,conn,s,uid,list[highlight]);
				break;
			case 110://press n
				//make another element for the list size
				{
					int xid = getUserID(inputwin,conn,s);
					if(xid>100&&xid<999)
					{
						list[listSize]=xid;
						recieverList[listSize]=getUserName(conn, s, list[xid]);
						listSize++;

					}
					wrefresh(menuwin);
				}
				break;
				//get its ID
				//then proceed
			case 9://press delete to close the window
				endwin();
				return 0;

			default:
				wmove(menuwin,10,10);
                }
        }


	//exitseqquence()
        endwin();
        return 0;
        
 }

