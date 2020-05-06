#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
int signup(MYSQL* conn);
int login(MYSQL*conn);
int showHome(MYSQL*,char*);
#endif // FUNCTIONS_H_INCLUDED
