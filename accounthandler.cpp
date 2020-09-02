#include "accounthandler.h"
_Accounthandler::_Accounthandler(_Mysqlhandler * mysqlhandler, _API * api):mysqlhandler(mysqlhandler), api(api), counts(0), d_basedir_stack(0)
{
    set_d_basedir(".");
}
_Accounthandler:: ~_Accounthandler()
{
       	for(int i=0; i<counts;i++){
               	delete account[i];
        }
	counts=0;
}
#ifndef NOTTRADE
void _Accounthandler::init_run_member(_Brainhandler * brainhandler, _Dealhandler * dealhandler)
{
	this->brainhandler = brainhandler;
	this->dealhandler = dealhandler;
}
#endif
void _Accounthandler::init_coinvalue(int * coinvalue)
{
	this->coinvalue = coinvalue;
    this->init_account_coinvalue(coinvalue);
}
void _Accounthandler::init_account_coinvalue(const int * const coinvalue)
{
    for(int i=0; i<counts; i++){
        account[i]->init_coinvalue(coinvalue);
    }
}
void _Accounthandler::set_d_basedir(const char * const d_basedir)
{
    d_basedir_stack++;
    strcpy(this->d_basedir, d_basedir);
}
void _Accounthandler::account_set_dbindex(void)
{
    const int num = mysqlhandler->get_index();
    for(int i=0; i<counts; i++)
        account[i]->set_dbindex(num);
}
void _Accounthandler::account_dir_construct(void)
{
    char d_nowdir[100];
    const char * path=d_nowdir;
    struct stat dir_stat;

    if(d_basedir_stack == 1)
        fprintf(fp, "d_basedir_stack == 1! set basedir!!\n");
    for(int i=0; i<counts; i++){
		if(strcmp(account[i]->apikey, "test") != 0){
            sprintf(d_nowdir, "%s/../account/%d", d_basedir, account[i]->_index);
            if(stat(path, &dir_stat) == -1){
                fprintf(fp, "Creating account/%d/\n", account[i]->_index);
                char d_accountnowdir[100];
                strcat(d_accountnowdir, path);
                mkdir(path, 0755);
                sprintf(d_nowdir, "%s/pending", d_accountnowdir);
                mkdir(path, 0755);
                sprintf(d_nowdir, "%s/pending/ask", d_accountnowdir);
                mkdir(path, 0755);
                sprintf(d_nowdir, "%s/pending/bid", d_accountnowdir);
                mkdir(path, 0755);
            }
        }
    }
}
void _Accounthandler::account_set_d_basedir(void)
{
    if(d_basedir_stack == 1)
        fprintf(fp, "d_basedir_stack == 1! set basedir!!\n");
    for(int i=0; i<counts; i++)
        account[i]->set_d_basedir(d_basedir);
}
void _Accounthandler::_Remove_All_Account(void){
	for(int i=0; i<counts;i++){
		delete account[i];
	}
	counts=0;
}
void _Accounthandler::_Create_Account(int brain, int deal, double KRW=1000000){
	double ACCOUNT_COIN[(int)enum_Account::COIN_COUNT];
	ACCOUNT_COIN[0]=KRW;
	for(int i=1;i<(int)enum_Account::COIN_COUNT;i++)
		ACCOUNT_COIN[i]=0.0;
	account[counts] = new _Account(counts, 0, "test","test",brain, deal);
	account[counts]->set_total_coin(ACCOUNT_COIN);
	account[counts]->set_available_coin(ACCOUNT_COIN);
	ACCOUNT_COIN[0]=0;
	account[counts]->set_in_use_coin(ACCOUNT_COIN);
	counts++;
}
void _Accounthandler::_Load_Account_with_Coin_From_testdb(void)
{
        counts += mysqlhandler->_Load_TestAccount_with_coin(account);
}
void _Accounthandler::_Load_Account_From_db(void)
{
        counts += mysqlhandler->_Load_Account(account);
}
void _Accounthandler::_Load_Account_Coin_From_db(void)
{
	for(int i=0;i<counts;i++)
		if(strcmp(account[i]->apikey, "test") == 0)
			mysqlhandler->get_account_coin_from_db(account[i]);
}
void _Accounthandler::_Load_Account_Coin_From_Server(void)
{
	for(int i=0;i<counts;i++)
		if(strcmp(account[i]->apikey, "test") != 0)
			api->get_account_coin_from_server(account[i]);
}
void _Accounthandler::_Save_Account(void)
{
	mysqlhandler->_Save_Account(account, counts);
}
void _Accounthandler::update_all_account_money(void)
{
	double total_money, available_money, in_use_money;
	for(int i=0; i<counts;i++){
		total_money = 0;
		available_money = 0;
		in_use_money = 0;

		total_money += account[i]->total_coin[0];
		for(int j = 0; j<(int)enum_Coinvalue::COIN_COUNT_NO_KRW; j++)
			total_money += account[i]->total_coin[1+j] * coinvalue[j];
		account[i]->total_money = total_money;

		available_money += account[i]->available_coin[0];
		for(int j = 0; j<(int)enum_Coinvalue::COIN_COUNT_NO_KRW; j++)
			available_money += account[i]->available_coin[1+j] * coinvalue[j];
		account[i]->available_money = available_money;

		in_use_money += account[i]->in_use_coin[0];
		for(int j = 0; j<(int)enum_Coinvalue::COIN_COUNT_NO_KRW; j++)
			in_use_money += account[i]->in_use_coin[1+j] * coinvalue[j];
		account[i]->in_use_money = in_use_money;
	}
}
void _Accounthandler::show_all_test_account(void)
{
	for(int i=0; i<counts;i++){
		if(strcmp(account[i]->apikey, "test") == 0){
			fprintf(fp, "[%d] :: ",i);
			account[i]->show_Account();
		}
	}
}
void _Accounthandler::show_all_real_account(void)
{
	for(int i=0; i<counts;i++){
		if(strcmp(account[i]->apikey, "test") != 0){
			fprintf(fp, "[%d] :: ",i);
			account[i]->show_Account();
		}
	}
}
void _Accounthandler::show_all_orders_account(void)
{
	for(int i=0; i<counts;i++){
		if(strcmp(account[i]->apikey, "test") != 0){
			fprintf(fp, "[%d] :: ",i);
			for(int i=0; i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW; i++){
				if(i%5==0)
					sleep(1);
				api->show_orders(account[i], "ask", i);
			}
			for(int i=0; i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW; i++){
				if(i%5==0)
					sleep(1);
				api->show_orders(account[i], "bid", i);
			}
		}
	}
}
void _Accounthandler::show_all_transactions_account(void)
{
	for(int i=0; i<counts;i++){
		if(strcmp(account[i]->apikey, "test") != 0){
			fprintf(fp, "[%d] :: ",i);
			api->show_transactions(account[i], 0);
		}
	}
}
void _Accounthandler::show_all_account_deposit(void)
{
	for(int i=0; i<counts;i++){
                if(strcmp(account[i]->apikey, "test") != 0){
                        fprintf(fp, "[%d] :: ",i);
                        api->show_krw_deposit(account[i]);
                        api->show_btc_address(account[i]);
                }
        }
}

void _Accounthandler::fshow_all_account_rate(FILE * fp, double KRW=1000000)
{
	for(int i=0; i<counts;i++){
		fprintf(fp, "%10.3f", (account[i]->total_money-KRW)/KRW * 100);
	}
}
void _Accounthandler::show_all_account_rate(double KRW=1000000)
{
	for(int i=0; i<counts;i++){
		fprintf(fp,"profit : %f%\n", (account[i]->total_money-KRW)/KRW * 100);
	}
}
void _Accounthandler::show_account(_Account * account)
{
	account->show_Account();
}
void _Accounthandler::fwrite_account_log(const _Account * account, const char * log)
{
    fwrite_account_log(account->_index, log);
}
void _Accounthandler::fwrite_account_log(const int _index, const char * log)
{
    FILE * account_fp;
    char path[100];
    sprintf(path, "%s/../account/%d/log", d_basedir, _index);
    account_fp = fopen(path, "a");
    fputs(log, account_fp);
    fclose(account_fp);
}
void _Accounthandler::fwrite_account_log(const _Account * account, const char * type, const char * order_id, const char * log)
{
    fwrite_account_log(account->_index, type, order_id, log);
}
void _Accounthandler::fwrite_account_log(const int _index, const char * type, const char * order_id, const char * log)
{
    FILE * account_fp;
    char path[100];
    sprintf(path, "%s/../account/%d/pending/%s/%s", d_basedir, _index, type, order_id);
    account_fp = fopen(path, "a");
    fputs(log, account_fp);
    fclose(account_fp);
}
#ifndef NOTTRADE
void _Accounthandler::run_all_account(void)
{
    for(int i=0; i<counts;i++){
                dealhandler->run(account[i], brainhandler->rate[account[i]->brain]);
	}
}
#endif
