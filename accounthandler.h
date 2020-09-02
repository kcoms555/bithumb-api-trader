#ifndef __ACCOUNT_HANDLER__
#define __ACCOUNT_HANDLER__

#include "enum.h"
#include "account.h"
#include "mysqlhandler.h"
#include "api.h"
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#ifndef NOTTRADE
#include "brainhandler.h"
#include "dealhandler.h"
#endif
class _Accounthandler
{
private:
	_Account * account[(int)enum_Accounthandler::ACCOUNTNUM];
	_Mysqlhandler * mysqlhandler;
#ifndef NOTTRADE
	_Brainhandler * brainhandler;
	_Dealhandler * dealhandler;
#endif
	_API * api;
	int counts;
	int * coinvalue;
    char d_basedir[100];
    int d_basedir_stack;
public:
    //초기화 멤버 : brainhandler, dealhandler(trade할시), coinvalue
	_Accounthandler(_Mysqlhandler * mysqlhandler, _API * api);
	~_Accounthandler();
#ifndef NOTTRADE
	void init_run_member(_Brainhandler * brainhandler, _Dealhandler * dealhandler);
#endif
	void init_coinvalue(int * coinvalue);
    void init_account_coinvalue(const int * const coinvalue);
    void set_d_basedir(const char * const d_basedir);
    void account_dir_construct(void);
    void account_set_d_basedir(void);
    void account_set_dbindex(void);
	void _Remove_All_Account(void);
	void _Create_Account(int brain, int deal, double KRW);
	void _Load_Account_with_Coin_From_testdb(void);
	void _Load_Account_From_db(void);
	void _Load_Account_Coin_From_db(void);
	void _Load_Account_Coin_From_Server(void);
	void _Save_Account(void);
	void update_all_account_money(void);
	void show_all_test_account(void);
	void show_all_real_account(void);
	void show_all_orders_account(void);
	void show_all_transactions_account(void);
	void show_all_account_deposit(void);
	void fshow_all_account_rate(FILE * fp, double KRW);
	void show_all_account_rate(double KRW);
	void show_account(_Account * account);
    void fwrite_account_log(const _Account * account, const char * log);
    void fwrite_account_log(const int _index, const char * log);
    void fwrite_account_log(const _Account * account, const char * type, const char * order_id, const char * log);
    void fwrite_account_log(const int _index, const char * type, const char * order_id, const char * log);
#ifndef NOTTRADE
	void run_all_account(void);
#endif
};
#endif
