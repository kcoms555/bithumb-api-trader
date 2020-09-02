//#define TESTER

#include <cstdio>


#define __FP__
#ifdef STDOUT
	#define fp stdout
#else
    FILE * fp;
#endif

#ifdef TESTER
    #define BUFFER 65536
#else
    #define BUFFER 60
#endif

#ifndef VERSION
#define VERSION 0.00
#endif

#include "enum.h"
#include "mysqlhandler.h"
#include "tradehandler.h"
#include "brainhandler.h"
#include "dealhandler.h"
#include "accounthandler.h"
#include "api.h"
#include "initor.cpp"

#include <ctime>
#include <unistd.h>
#include <sys/stat.h>
#include <libgen.h>

void get_baseDir(char * d_baseDir);
void dir_construct(const char * d_baseDir);
int check_cancel(const char * d_baseDir);
void fwrite_basedir(FILE * fp, const char * basedir);
void fwrite_time(FILE * fp);
void fwrite_option(FILE * fp);
int main(void){
    char d_basedir[100];
    get_baseDir(d_basedir);
    dir_construct(d_basedir);
    fwrite_basedir(fp, d_basedir);
    fwrite_time(fp);
    fwrite_option(fp);

	clock_t start,mid, end, timer;
	double duration1, duration2;

	start = clock();

	int coinvalue[(int)enum_Coinvalue::COIN_COUNT_NO_KRW];
	_Mysqlhandler mysqlhandler;
	_API api;
	_Brainhandler brainhandler(&mysqlhandler);
	_Tradehandler tradehandler(&mysqlhandler, &api);
	_Dealhandler dealhandler(&tradehandler);
	_Accounthandler accounthandler(&mysqlhandler, &api);	

#ifndef NOTTRADE
    accounthandler.init_run_member(&brainhandler, &dealhandler);
#endif
    accounthandler.init_coinvalue(coinvalue);
    accounthandler.set_d_basedir(d_basedir);
	brainhandler.get_rates();

	initor("init_member", &brainhandler, &dealhandler);

	dealhandler.init_deals_coinvalue(coinvalue);
	tradehandler.init_coinvalue(coinvalue);
    api.set_d_BaseDir(d_basedir);
    if(check_cancel(d_basedir) == 0){
        api.trade_activation();
    }
#ifdef NOTTRADE
    api.trade_disabled();
#endif

#ifdef TESTER
	int num;
    double initdeal[3];
	int brain_num; double brain_buylow;
	mysqlhandler.update_DB(BUFFER);
	printf("insert timeshift ( dbcount 보다 작을것 dbcount : %d, dbindex : %d )\n", mysqlhandler.get_dbcount(), mysqlhandler.get_dbindex());
	scanf("%d", &num);
	printf("insert deal initvalue buylimit, selllimit, buylow; buylow==0일시 기본 셋팅값\n");
	scanf("%lf", &initdeal[0]);
	if(initdeal[0]!=0){
		scanf("%lf", &initdeal[1]);
		scanf("%lf", &initdeal[2]);
		dealhandler.init_deals(initdeal[0], initdeal[1], initdeal[2]);
	}
	printf("insert brain initvalue brain, buylow OR insert 0\n");
	scanf("%d", &brain_num);
	while(brain_num != 0){
		scanf("%lf",&brain_buylow);
		brainhandler.init_brain(brain_num, brain_buylow);
		scanf("%d", &brain_num);
	}
	mysqlhandler.set_timeshift(num);
	accounthandler._Load_Account_with_Coin_From_testdb();
#else
	mysqlhandler.update_DB(BUFFER);
	mysqlhandler.set_timeshift(0); //현재, 매매는 1번 실행됨
	accounthandler._Load_Account_From_db();
	accounthandler._Load_Account_Coin_From_db();
	accounthandler._Load_Account_Coin_From_Server();
    accounthandler.account_dir_construct();
    accounthandler.account_set_d_basedir();
    accounthandler.account_set_dbindex();
#endif
    accounthandler.init_coinvalue(coinvalue);
	mysqlhandler.get(coinvalue, 0);
	accounthandler.update_all_account_money();
	accounthandler.show_all_real_account();
	accounthandler.show_all_test_account();

	
	mid = clock();
	while(1)
	{
#ifdef SHOWINDEX
		mysqlhandler.show_timeshift();
#endif
		mysqlhandler.get(coinvalue, 0);
		brainhandler.run_brains();
#ifndef NOTTRADE
		accounthandler.run_all_account();
#endif
		accounthandler.update_all_account_money();
		if(mysqlhandler.get_timeshift()==0)
			break;
		else
			mysqlhandler.run_timeshift();
	}

	accounthandler.show_all_real_account();
	accounthandler.show_all_test_account();
	end = clock();

#ifndef TESTER
	accounthandler._Save_Account();
#endif

	duration1 = (double)(mid - start) / CLOCKS_PER_SEC;
	duration2 = (double)(end - mid) / CLOCKS_PER_SEC;

	fprintf(fp, "Running time : %f + %f s\n", duration1, duration2);
	fclose(fp);
	return 0;
}

void get_baseDir(char * d_BaseDir){
    int d_ret = readlink("/proc/self/exe", d_BaseDir, 100);
    d_BaseDir[d_ret+1] = '\0';
    dirname(d_BaseDir);
}
void dir_construct(const char * d_BaseDir){
    char d_nowDir[100];
    char * path;
    struct stat dir_stat;

    strcpy(d_nowDir, d_BaseDir);
#ifndef TESTER
    path = strcat(d_nowDir, "/../account");
    if(stat(path, &dir_stat) == -1)
        mkdir(path, 0755);
#endif
#ifndef STDOUT
	#ifdef TESTER
    strcpy(d_nowDir, d_BaseDir);
    path = strcat(d_nowDir, "/log_tester");
	fp = fopen(path, "w");
	#else
    strcpy(d_nowDir, d_BaseDir);
    path = strcat(d_nowDir, "/log");
	fp = fopen(path, "w");
	#endif
#endif
}
void fwrite_basedir(FILE * fp, const char * basedir){
    fprintf(fp, "basedir : %s\n", basedir);
}
void fwrite_time(FILE * fp){
	time_t time_now;
	time(&time_now);
	fprintf(fp,"server time : %d %s", time_now, ctime(&time_now));
}
void fwrite_option(FILE * fp){
    fprintf(fp,"COMPILE OPTION : ");
#ifdef VERSION
    fprintf(fp, "VERSION=%.2f ", VERSION);
#endif
#ifdef TESTER
    fprintf(fp,"TESTER ");
#endif
#ifdef STDOUT
    fprintf(fp,"STDOUT ");
#endif
#ifdef SHOWINDEX
    fprintf(fp,"SHOWINDEX ");
#endif
#ifdef DEBUG
    fprintf(fp,"DEBUG ");
#endif
#ifdef DEAL_DEBUG
    fprintf(fp,"DEAL_DEBUG ");
#endif
#ifdef TRADE_DEBUG
    fprintf(fp,"TRADE_DEBUG ");
#endif
#ifdef API_DEBUG
    fprintf(fp,"API_DEBUG ");
#endif
#ifdef NOTTRADE
    fprintf(fp,"NOTTRADE ");
#endif
    fprintf(fp,"\n");
}
int check_cancel(const char * d_baseDir){
    char d_nowDir[100];
    char * path;
    struct stat dir_stat;

    strcpy(d_nowDir, d_baseDir);
    path = strcat(d_nowDir, "/cancel");
    return stat(path, &dir_stat);       //있으면 0; 없으면 -1
}
#include "enum.cpp"
#include "account.cpp"
#include "accounthandler.cpp"
#include "brainhandler.cpp"
#include "dealhandler.cpp"
/* compile :
g++ -I/usr/include/mariadb -DSHOWINDEX -lmariadb -lcurl -lcrypto -o main main.cpp
g++ -I/usr/include/mariadb -DTESTER -DSTDOUT -lmariadb -lcurl -lcrypto -o tester main.cpp
*/
