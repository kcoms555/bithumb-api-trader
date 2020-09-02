#define __FP__
#define fp stdout

#include "api.h"
#include "mysqlhandler.h"
#include "accounthandler.h"
#include "enum.h"


#include <ctime>
#include <unistd.h>

int main(void){
	time_t nowtime;

	_Mysqlhandler mysqlhandler;
	_API api;
	_Accounthandler accounthandler(&mysqlhandler, &api);

	int coinvalue[(int)enum_Coinvalue::COIN_COUNT_NO_KRW];

	//api.init_coinvalue(coinvalue);
	accounthandler.init_coinvalue(coinvalue);
	accounthandler._Load_Account_From_db();
    accounthandler.init_account_coinvalue(coinvalue);

	while(1){
		mysqlhandler.update_DB(1);
		mysqlhandler.set_timeshift(0);
		accounthandler._Load_Account_Coin_From_Server();
		accounthandler._Load_Account_Coin_From_db();
		mysqlhandler.get(coinvalue, (int)enum_Mysqlhandler::NOW);
		accounthandler.update_all_account_money();
	
		system("clear");
		time(&nowtime);
		printf("MONITOR %d %s", nowtime, ctime(&nowtime));
		accounthandler.show_all_real_account();
		accounthandler.show_all_test_account();
		sleep(3);
	}
	return 0;
}
#include "enum.cpp"
#include "account.cpp"
#include "accounthandler.cpp"
#include "dealhandler.cpp"
#include "brainhandler.cpp"
