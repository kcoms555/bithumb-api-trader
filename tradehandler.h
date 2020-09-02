#ifndef __TRADEHANDLER__
#define __TRADEHANDLER__

#ifndef __FP__
#define fp stdout
#endif

#include <cstdio>
#include <iostream>
#include <cstring>
#include "mysqlhandler.h"
#include "account.h"
#include "enum.h"
#include "api.h"

#define TRADE_FEE 0.0015


class _Tradehandler
{
private:
	char name[64];
	const int * coinvalue;
	char query[1024];
	_Mysqlhandler * mysqlhandler;
	_API * api;
public:
	_Tradehandler(_Mysqlhandler * mysqlhandler, _API * api):mysqlhandler(mysqlhandler),api(api){}
	void init_coinvalue(const int * coinvalue)
	{
		this->coinvalue=coinvalue;
	}
	int run(_Account * account, int COIN, double volume)
	{
		get_name_no_krw(name, COIN);
        const int price = coinvalue[COIN];
        double total_order = price * volume;
		if(strcmp(account->apikey,"test")==0){
            if(volume > 0){
			    account->total_coin[(int)enum_Account::KRW] -= total_order;
			    account->available_coin[(int)enum_Account::KRW] -= total_order;
			    account->total_coin[COIN+(int)enum_Coinvalue::COIN_OFFSET] += volume * (1.0-TRADE_FEE);
			    account->available_coin[COIN+(int)enum_Coinvalue::COIN_OFFSET] += volume * (1.0-TRADE_FEE);
#ifdef TRADE_DEBUG
               	fprintf(fp, "TESTER INDEX %d :: BUY %s, volume : %f (%f)\n", account->_index, name, volume, total_order);
#endif
            }
            else{
			    account->total_coin[(int)enum_Account::KRW] -= total_order*(1.0-TRADE_FEE);
			    account->available_coin[(int)enum_Account::KRW] -= total_order*(1.0-TRADE_FEE);
			    account->total_coin[COIN+(int)enum_Coinvalue::COIN_OFFSET] += volume;
			    account->available_coin[COIN+(int)enum_Coinvalue::COIN_OFFSET] += volume;
#ifdef TRADE_DEBUG
            	fprintf(fp, "TESTER INDEX %d :: SELL %s, volume : %f (%f)\n", account->_index, name, -volume, -total_order);
#endif
            }
			account->tradesum += (total_order > 0)?total_order :-total_order;
		}
		else{				//매수시 volume 양수, 매도시 volume 음수!
        	if(volume > 0){
			    //account->in_use_coin[(int)enum_Account::KRW] += total_order;
			    //account->available_coin[(int)enum_Account::KRW] -= total_order;
           	    fprintf(fp, "TRADEHANDLER :: BUY %s, volume : %f (%f)\n", name, volume, total_order);
			}
        	else{
			    //account->in_use_coin[COIN+(int)enum_Coinvalue::COIN_OFFSET] += -volume;
			    //account->available_coin[COIN+(int)enum_Coinvalue::COIN_OFFSET] -= -volume;
             	fprintf(fp, "TRADEHANDLER :: SELL %s, volume : %f (%f)\n", name, -volume, -total_order);
			}
			api->trade(account, COIN, price, volume);
		}
       	return 0;
	}
};
#endif
