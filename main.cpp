#include <iostream>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include "another.cpp"
using namespace std;
using namespace oracle::occi;



string createnewUser(Connection *conn,Statement *&s)
{
	string user, password, check;
	password="";check="";
	cout<<"enter a username: ";
	cin>>user;
	cout<<endl<<"Enter password:";
	cin>>password;
	
	
	
	ResultSet *rs = s->executeQuery("select count(*) from pusers");
	int id;
	if(rs->next())
		id=rs->getInt(1);
	//create a random 3 digit for unique user id taken from the table
	id=id+101;
	s->closeResultSet(rs);
	
	string query="INSERT INTO pusers values (:1, :2 , :3 )";
	s = conn->createStatement(query);
	s->setInt(1,id);
	s->setString(2,user);
	s->setString(3,password);

	s->executeUpdate();
		//executes the insert query 
	conn->commit();
	cout<<"USER Created!!"<<endl;
	return user;
		//returns to login instance with user field with the new user defined here 
}

int main()
{

	string userName;
	string password;
	cout<<"enter database username:-";cin>>userName;
	cout<<"enter database password:-";cin>>password;
	const string connectString= "sunfire.csci.viu.ca";
		//the url of database(location) where we are trying to connect

		//setting environment variable
	Environment *env = Environment::createEnvironment();
	Connection *conn = env->createConnection(userName,password, connectString);
		//sets up the environment variable and now you are connected to the database
	Statement *stmt=conn->createStatement();

	string u,pw;
		//strings to contain user name and password
	cout<<"Enter your username (if not registered, press n then enter):";
	cin>>u;
	
		//check if the user is not registered
	if(u.compare("n")==0)
	{
		u=createnewUser(conn,stmt);
	}

	cout<<"Enter Password:";
	cin>>pw;
	cout<<endl;
	
	int session=-1;

	stmt = conn ->createStatement("select id, userid from pusers where userid = :1 and password = :2");
	stmt->setString(1,u);
	stmt->setString(2,pw);
	





	ResultSet *rs = stmt->executeQuery();
	
	
	string userid;
	if(rs->next())
	{	
		session =rs->getInt(1);
		userid=rs->getString(2);
	}	
	
	cout<<" username is "<<userid<<endl;
	//cleaning up the environment before leaving the program
	cout<<"loading chat window"<<endl;

	stmt->closeResultSet(rs);
	if(session ==-1)
	{
        	env->terminateConnection(conn);
	        Environment::terminateEnvironment(env);
		cout<<"ID  NOT FOUND! exiting\n";
		return 0;


	}

	int size=getRecieverCount(conn,stmt,session);

	int recieverList[size+10];
	stmt=conn->createStatement("select distinct reciever from messages where sender = :1 or reciever= :1");
	stmt->setInt(1,session);

	int i=0;
	ResultSet *menulist =stmt->executeQuery();
	while(menulist->next())
	{
		recieverList[i]=menulist->getInt(1);
		i++;
		
	}

	stmt->closeResultSet(menulist);


	Preview(session, conn,stmt,recieverList,size);


	
	
	//program ends
	env->terminateConnection(conn);
	Environment::terminateEnvironment(env);

	return 0;



}
