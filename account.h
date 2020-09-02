#ifndef __ACCOUNT__
#define __ACCOUNT__

#include "enum.h"
#include <cstring>
#include <iostream>

class _Account
{
public:
	const int _index;
	const int owner;
	char apikey[(int)enum_Account::KEYLEN];
	char secretkey[(int)enum_Account::KEYLEN];
	const int brain;
	const int deal;
	double total_money;
	double available_money;
	double in_use_money;
	double available_coin[(int)enum_Account::COIN_COUNT];
	double total_coin[(int)enum_Account::COIN_COUNT];	//coin == total_coin
	double in_use_coin[(int)enum_Account::COIN_COUNT];
	double tradesum;
	char name[20];
    char d_basedir[100];
    mutable int stack;
    int dbindex;
    const int * coinvalue;

	_Account(const int _index, const int owner, const char * const apikey, const char * const secretkey, const int brain, const int deal);
	_Account(const int _index, const char * const apikey, const char * const secretkey);
	~_Account();
    void init_coinvalue(const int * const coinvalue);
    void set_dbindex(const int dbindex);
	void show_Account(void);
    void set_d_basedir(const char * d_basedir);
	void set_total_coin(double * coin);
	void set_available_coin(double * coin);
	void set_in_use_coin(double * coin);
	void set_all_coin_error(void);
	void get_total_coin(double * coin);
	void get_available_coin(double * coin);
    void fwrite_log(const char * log) const;
    void fwrite_log(const char * type, const char * order_id, const char * log) const;
    int get_stack(void) const;
    void set_stack_inc(void) const;
    void set_stack_zero(void) const;
};
#endif
