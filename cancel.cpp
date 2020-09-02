#include <cstdio>

#define __FP__
#define fp stdout

#include "api.h"
#include "account.h"
#include <unistd.h>
#include <ctime>
#include <libgen.h>

void get_baseDir(char * d_BaseDir){
    int d_ret = readlink("/proc/self/exe", d_BaseDir, 100);
    d_BaseDir[d_ret+1] = '\0';
    dirname(d_BaseDir);
}
int main(int argc, char *argv[]){
	time_t nowtime;
	time(&nowtime);
	int count;
    char buffer[50];
    char d_basedir[100];

    get_baseDir(d_basedir);

	_API api;
    api.set_d_BaseDir(d_basedir);
	_Account account(atoi(argv[2]), argv[3], argv[4]);
	sleep(atoi(argv[1]));
	api.cancel(&account, argv[5], argv[6], argv[7]);
	return 0;
}
#include "account.cpp"
#include "enum.cpp"
