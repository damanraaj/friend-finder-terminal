#include <iostream>
#include<winsock.h>
#include <mysql.h>
#include "functions.h"
using namespace std;
int main()
{
    MYSQL* conn;
    conn=mysql_init(0);
    if(conn)
    {
        //cout<<"Connection Object Created!!\n";
        conn=mysql_real_connect(conn,"127.0.0.1","root","root","socialnet",0,nullptr,0);

        if(!mysql_errno(conn))
        {
            cout<<"Database connected\n";
            //if(mysql_query(conn,"show databases;"))
            //    cout<<"Error in query : "<<mysql_error(conn);
            //else
            //{
            cout<<"\n\n";
            cout<<"\t\t\tFriend Finder\n";
                int n;
                while(n)
                {
                    cout<<"1. Signup \n"<<"2.Login \n";
                    cout<<"0. Exit\n";
                    cin>>n;
                    if(n==1)
                    {
                        signup(conn);
                    }
                    else if(n==2)
                    {
                        login(conn);
                    }
                    else
                        return 1;
                }

            //}
        }
        else
        {
            cout<<"Error : "<<mysql_errno(conn)<<mysql_error(conn)<<"\n";
        }
    }
    else
    {
        cout<<"Connection Object Error : "<<mysql_error(conn)<<"\n";
    }
    return 0;
}
