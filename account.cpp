#include "account.h"
_Account::_Account(const int _index, const int owner, const char * const apikey, const char * const secretkey, const int brain, const int deal)
	: _index(_index), owner(owner), brain(brain), deal(deal), total_money(0.0), available_money(0.0), tradesum(0.0), stack(0), dbindex(-1), coinvalue(NULL)
	{
        set_d_basedir(".");
		memcpy((void*)this->apikey, apikey, sizeof(char)*(int)enum_Account::KEYLEN);
		memcpy((void*)this->secretkey, secretkey, sizeof(char)*(int)enum_Account::KEYLEN);
		//fprintf(fp, "account created! index : %d owner : %d, brain : %d, deal : %d\n", _index, owner, brain, deal);
	};
_Account::_Account(const int _index, const char * const apikey, const char * const secretkey): _index(_index), owner(-1), brain(0), deal(0), stack(0), dbindex(-1), coinvalue(NULL)
	{
        set_d_basedir(".");
		memcpy((void*)this->apikey, apikey, sizeof(char)*(int)enum_Account::KEYLEN);
		memcpy((void*)this->secretkey, secretkey, sizeof(char)*(int)enum_Account::KEYLEN);
	};
_Account::~_Account()
	{
	};
void _Account::set_dbindex(const int dbindex)
{
    this->dbindex=dbindex;
}
void _Account::init_coinvalue(const int * const coinvalue)
{
    this->coinvalue = coinvalue;
}
	void _Account::show_Account(void)
	{
		if(strcmp(apikey, "test") == 0)
			fprintf(fp, "TEST : ");
		else
			fprintf(fp, "REAL : ");
		fprintf(fp, "_index : %d, brain : %d, deal :%d, tradesum : %.0f\n", _index, brain, deal, tradesum);
        if(coinvalue==NULL){
		    fprintf(fp, "%30s %20s %20s\n", "total", "available", "in use");
		    fprintf(fp, "%25.0f %20.0f %20.0f\n", total_money, available_money, in_use_money);
		    for(int i=0; i<(int)enum_Account::COIN_COUNT; i++){
		    	get_name(name, i);
		    	fprintf(fp, "%7s : %20.4f %20.4f %20.4f\n", name, total_coin[i], available_coin[i], in_use_coin[i]);
		    }
        }
        else{
		    fprintf(fp, "%30s %20s %20s %20s\n", "total", "available", "in use", "total_value");
		    fprintf(fp, "%25.0f %20.0f %20.0f\n", total_money, available_money, in_use_money);
		    get_name(name, 0);
		    fprintf(fp, "%7s : %20.4f %20.4f %20.4f\n", name, total_coin[0], available_coin[0], in_use_coin[0]);
		    for(int i=1; i<(int)enum_Account::COIN_COUNT; i++){
		    	get_name(name, i);
		    	fprintf(fp, "%7s : %20.4f %20.4f %20.4f %20.0f\n", name, total_coin[i], available_coin[i], in_use_coin[i], total_coin[i] * coinvalue[i-1]);
		    }
        }


		//fprintf(fp, "_index : %d, apikey : %s, secretkey : %s, brain : %d, deal :%d, money : %f, tradesum : %f\n", _index, apikey, secretkey, brain, deal, money, tradesum);
	}
void _Account::set_d_basedir(const char * d_basedir)
{
    strcpy(this->d_basedir, d_basedir);
}
	void _Account::set_total_coin(double * coin)
	{
		memcpy((void*)this->total_coin, coin, sizeof(double)*(int)enum_Account::COIN_COUNT);
	}
	void _Account::set_available_coin(double * coin)
	{
		memcpy((void*)this->available_coin, coin, sizeof(double)*(int)enum_Account::COIN_COUNT);
	}
	void _Account::set_in_use_coin(double * coin)
	{
		memcpy((void*)this->in_use_coin, coin, sizeof(double)*(int)enum_Account::COIN_COUNT);
	}
	void _Account::set_all_coin_error(void)
	{
		for(int i=0; i<(int)enum_Account::COIN_COUNT; i++)
		{
			total_coin[i]=-1;
			available_coin[i]=-1;
			in_use_coin[i]=-1;
		}
	}
	void _Account::get_total_coin(double * coin)
	{
		for(int i=0;i<(int)enum_Account::COIN_COUNT;i++)
			coin[i]=this->total_coin[i];
	}
	void _Account::get_available_coin(double * coin)
	{
		for(int i=0;i<(int)enum_Account::COIN_COUNT;i++)
			coin[i]=this->available_coin[i];
    }
void _Account::fwrite_log(const char * log) const
{
    FILE * account_fp;
    char path[100];
    sprintf(path, "%s/../account/%d/log", d_basedir, _index);
    account_fp = fopen(path, "a");
    if(dbindex!=-1){
        fprintf(account_fp, "%d ", dbindex);
    }
    fputs(log, account_fp);
    fclose(account_fp);
}
void _Account::fwrite_log(const char * type, const char * order_id, const char * log) const
{
    FILE * account_fp;
    char path[100];
    sprintf(path, "%s/../account/%d/pending/%s/%s", d_basedir, _index, type, order_id);
    account_fp = fopen(path, "a");
    if(dbindex!=-1){
        fprintf(account_fp, "%d ", dbindex);
    }
    fputs(log, account_fp);
    fclose(account_fp);
}
int _Account::get_stack(void) const
{
    return stack;
}
void _Account::set_stack_inc(void) const
{
    stack++;
}
void _Account::set_stack_zero(void) const
{
    stack=0;
}
