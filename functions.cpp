#include<iostream>
#include<winsock.h>
#include<mysql.h>
#include<string>
#include<queue>
#include<unordered_set>
using namespace std;
int signup(MYSQL* conn)
{
    string query;
    conn=mysql_init(0);
    if(conn)
    {
        conn=mysql_real_connect(conn,"127.0.0.1","root","root","socialnet",0,nullptr,0);
        if(!mysql_errno(conn))
        {
            cout<<"Enter name :";
            string name,dob,email,pass;
            getline(cin>>ws,name);
            cout<<"Enter dob Y/M/D:";
            cin>>dob;
            cout<<"Enter Email :";
            cin>>email;
            cout<<"Enter password :";
            cin>>pass;
            if((int)email.find('@')==-1)
            {
                cout<<"Invalid email"<<"\n";
                 return 0;
            }
            query="insert into users values('"+pass+"','"+name+"','"+dob+"','"+email+"');";
            if(mysql_query(conn,query.c_str()))
                cout<<"Error in query : "<<mysql_error(conn)<<"\n";
            else
            {
                if(mysql_affected_rows(conn))
                {
                    string username=email.substr(0,email.find("@"));
                    query="create table "+username+"_friends (friends varchar(40) primary key);";
                    if(mysql_query(conn,query.c_str()))
                    {
                        cout<<"Can't create account : "<<mysql_error(conn)<<"\n";
                        return 0;
                    }
                    query="create table "+username+"_posts (post varchar(200),addtime datetime);";
                    if(mysql_query(conn,query.c_str()))
                    {
                        cout<<"Can't create account : "<<mysql_error(conn)<<"\n";
                        return 0;
                    }
                    cout<<"Account created\n";
                    return 1;
                }
            }
        }
        else
        {
            cout<<"Error : "<<mysql_errno(conn)<<mysql_error(conn)<<"\n";
            return 0;
        }
    }
    else
    {
        cout<<"Connection Object Error : "<<mysql_error(conn)<<"\n";
        return 0;
    }
    return 0;
}
int addPost(MYSQL*conn,string username)
{
    conn=mysql_init(0);
    if(conn)
    {
        conn=mysql_real_connect(conn,"127.0.0.1","root","root","socialnet",0,nullptr,0);
        if(!mysql_errno(conn))
        {
            string post,query;
            cout<<"Type (MAX 200 characters;): "<<"\n";
            getline(cin>>ws,post);

            query="insert into "+username+"_posts values('"+post+"',+NOW());";
           int t=mysql_query(conn,query.c_str());
           if(t)
               cout<<mysql_error(conn)<<"\n";
           else
            cout<<"post added to your timeline"<<"\n";
        }
    }
    return 1;
}
int addFriend(MYSQL*conn,string username)
{
    conn=mysql_init(0);
    if(conn)
    {
        conn=mysql_real_connect(conn,"127.0.0.1","root","root","socialnet",0,nullptr,0);
        if(!mysql_errno(conn))
        {
            cout<<"Add a friend!\n";
            string query,frname;
            cout<<"Enter email : ";
            cin>>frname;
            query="select * from users where email='"+frname+"';";
            mysql_query(conn,query.c_str());
            if(mysql_errno(conn))
                cout<<mysql_error(conn)<<"\n";
            else if(mysql_num_rows(mysql_store_result(conn)))
            {
                query="insert into "+username+"_friends values('"+frname+"');";
                mysql_query(conn,query.c_str());
                if(mysql_errno(conn))
                   cout<<mysql_error(conn)<<"\n";
                else
                {
                    cout<<"friend added"<<"\n";
                }
            }
        }
    }
    return 1;
}
void seePost(MYSQL*conn,string user)
{
    conn=mysql_init(0);
    conn=mysql_real_connect(conn,"127.0.0.1","root","root","socialnet",0,nullptr,0);
    string query;
    if(mysql_errno(conn))
    {
        cout<<mysql_error(conn)<<"\n";
    }
    else
    {
        query="create temporary table seenposts (posts varchar(200),addtime datetime);";
        mysql_query(conn,query.c_str());
        if(mysql_errno(conn))
        {
            cout<<mysql_error(conn)<<"\n";
        }
        else
        {
            query="select * from "+user+"_friends;";
            MYSQL_ROW row,row1;
            MYSQL_RES*res,*res1;
            if(!mysql_query(conn,query.c_str()))
            {
                res=mysql_store_result(conn);
                while((row=mysql_fetch_row(res)))
                {
                    string name=row[0];
                    name=name.substr(0,name.find("@"));
                    query="insert into seenposts (select * from "+name+"_posts order by addtime desc);";

                    if(mysql_query(conn,query.c_str()))
                        cout<<mysql_error(conn)<<"\n";
                }
                query="select * from seenposts order by addtime desc;";
                mysql_query(conn,query.c_str());
                if(mysql_errno(conn))
                    cout<<mysql_error(conn)<<"\n";
                else
                {
                    res=mysql_store_result(conn);
                    cout<<"Posts: \n";
                    while((row=mysql_fetch_row(res)))
                    {
                        cout<<row[0]<<"\n";
                    }
                }

            }
            else
                cout<<mysql_error(conn)<<"\n";
        }
    }
}
int showFriends(MYSQL*conn,string username)
{
        conn=mysql_init(0);
        conn=mysql_real_connect(conn,"127.0.0.1","root","root","socialnet",0,nullptr,0);
        if(mysql_errno(conn))
        {
            cout<<mysql_error(conn)<<"\n";
            return 0;
        }
        MYSQL_ROW row;
        MYSQL_RES*res;
        string query="select friends from "+username.substr(0,username.find("@"))+"_friends;";
        if(mysql_query(conn,query.c_str()))
        {
            cout<<mysql_error(conn)<<"\n";
            return 0;
        }
        else
        {
            res=mysql_store_result(conn);
            if(res)
                while((row=mysql_fetch_row(res)))
                {
                    cout<<row[0]<<"\n";
                }
        }
        return 1;
}
int suggestFriends(MYSQL*conn,string user)
{
    conn=mysql_init(0);
    conn=mysql_real_connect(conn,"127.0.0.1","root","root","socialnet",0,nullptr,0);
    if(mysql_errno(conn))
    {
        cout<<mysql_error(conn)<<"\n";
        return 0;
    }
    MYSQL_ROW row;
    MYSQL_RES*res;
    unordered_set<string> visited;
    queue<string> fr;
    string username=user.substr(0,user.find("@"));
    string query="select friends from "+username+"_friends;";
    if(mysql_query(conn,query.c_str()))
    {
        cout<<mysql_error(conn)<<"\n";
        return 0;
    }
    else
    {
        visited.insert(user);

        res=mysql_store_result(conn);
        while((row=mysql_fetch_row(res)))
        {
            fr.push(row[0]);
            visited.insert(row[0]);
        }
        query="create temporary table suggested (friends varchar(40) primary key);";
        mysql_query(conn,query.c_str());
        if(mysql_errno(conn))
        {
            cout<<mysql_error(conn)<<"\n";
        }
        else
        {
            string frname;
            while(!fr.empty())
            {
                frname=fr.front();
                fr.pop();
                frname=frname.substr(0,frname.find("@"));
                query="select * from "+frname+"_friends;";
                mysql_query(conn,query.c_str());
                if(mysql_errno(conn))
                    cout<<mysql_error(conn)<<"\n";
                else
                {
                    res=mysql_store_result(conn);
                    while((row=mysql_fetch_row(res)))
                    {
                        frname=row[0];
                        if(visited.find(frname)==visited.end())
                        {
                            query="insert into suggested values('"+frname+"');";
                            mysql_query(conn,query.c_str());
                            visited.insert(frname);
                            fr.push(frname);
                        }
                    }
                }
            }
            query="select * from suggested;";// where friends!='"+user+"' and friends not in (select friends from "+username+"_friends);";
            mysql_query(conn,query.c_str());
            if(mysql_errno(conn))
                cout<<mysql_error(conn)<<"\n";
            else
            {
                res=mysql_store_result(conn);
                cout<<"Suggested: \n";
                while((row=mysql_fetch_row(res)))
                {
                    cout<<row[0]<<"\n";
                }
            }
        }
        mysql_query(conn,"drop temporary table suggested;");
    }
    return 1;
}

int login(MYSQL*conn)
{
    string email;
    string password;
    cout<<"enter email :";
    cin>>email;
    cout<<"enter password :";
    cin>>password;

    string query;
    query="select *from users where passkey='"+password+"' and email='"+email+"';";
    mysql_query(conn,query.c_str());
    if(mysql_errno(conn))
    {
        cout<<mysql_error(conn)<<"\n";
    }
    else if(mysql_num_rows(mysql_store_result(conn))==1)
    {
        string userp=email.substr(0,email.find("@"));
        system("cls");
        cout<<"login successful\n";
        cout<<email<<"\n";
//++++++++++++++++++++++++++++++++++++++
loop:
        int x;
        cout<<"1. Add Friends"<<"\n";
        cout<<"2. Add Post"<<"\n";
        cout<<"3. Show Friends"<<"\n";
        cout<<"4. Show Posts"<<"\n";
        cout<<"5. Find Friends"<<"\n";
        cout<<"0. Logout"<<"\n";
        cin>>x;

        if(x==2)
            addPost(conn,userp);
        else if(x==1)
        {
            addFriend(conn,userp);
        }
        else if (x==4)
            seePost(conn,userp);
        else if(x==3)
            showFriends(conn,userp);
        else if(x==5)
            suggestFriends(conn,email);
        else if(x==0)
            return 1;

        goto loop;
    }
    else
    {
        cout<<"Invalid id or password\n";
        return 0;
    }
    return 1;
}
