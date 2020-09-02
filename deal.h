#ifndef __FP__
#define fp stdout
#endif

#ifndef __DEAL__
#define __DEAL__

#include <cstdio>
#include <cstring>
#include <iostream>
#include "account.h"
#include "enum.h"
#include "array.h"
#include "mysqlhandler.h"
#include "tradehandler.h"

class _Deal0
{
private:
        char name[64];
        int * coinvalue;
        _Tradehandler * tradehandler;
public:
        _Deal0(_Tradehandler * tradehandler):tradehandler(tradehandler){}
	    void init_coinvalue(int * coinvalue)
	    {
		    this->coinvalue = coinvalue;
	    }
        void run(_Account * account, double * rate) //가지고 있는 모든 자본을 항상 매도한다
        {
                const double * const coin = account->available_coin;
                for(int i=0; i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW; i++){
                    if( (coin[i+(int)enum_Coinvalue::COIN_OFFSET]*coinvalue[i]) > (double)enum_Deal::IGNORE)
                        tradehandler->run(account, i, -coinvalue[i]);
                }
        }
		void init(double buylimit, double selllimit, double buylow=0.01){}
};
class _Deal1
{
private:
        char name[64];
        double available_money;
        double buy_limit;
        double sell_limit;
        double buy_limit_rate;
        double sell_limit_rate;
	double buylow;
        int * coinvalue;
        _Tradehandler * tradehandler;
public:
        _Deal1(_Tradehandler * tradehandler):tradehandler(tradehandler){}
	void init_coinvalue(int * coinvalue)
	{
		this->coinvalue = coinvalue;
	}
	void init(double buylimit, double selllimit, double buylow=0.01){
		buy_limit_rate = buylimit;
		sell_limit_rate = selllimit;
		this->buylow = buylow;
	}
        void run(_Account * account, double * rate)
        {
                available_money = account->available_money;
                const double * const coin = account->available_coin;
                buy_limit = available_money * buy_limit_rate;
                sell_limit = available_money * sell_limit_rate;
                int index;
                int i;

                i=1;
                while(i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW && sell_limit>0 && rate[index = arraymin(rate, (int)enum_Coinvalue::COIN_COUNT_NO_KRW, i++)]<0){
                        if(coinvalue[index]*coin[index+(int)enum_Coinvalue::COIN_OFFSET]>(double)enum_Deal::IGNORE){
#ifdef DEAL_DEBUG
                                fprintf(fp, "%s : %f\n",get_name_no_krw(name, index), rate[index]);
#endif
                                if(coinvalue[index]*coin[index+(int)enum_Coinvalue::COIN_OFFSET] > sell_limit){
                                        tradehandler->run(account, index, -sell_limit/coinvalue[index]);
                                        sell_limit -= sell_limit/coinvalue[index] * coinvalue[index];   //sell_limit = 0
                                }
                                else{
                                        tradehandler->run(account, index, -coin[index+(int)enum_Coinvalue::COIN_OFFSET]);
                                        sell_limit -= coin[index+(int)enum_Coinvalue::COIN_OFFSET] * coinvalue[index];  //sell_limit -= 수량 * 가격
                                }
                        }

                }

                i=1;
                if(buy_limit>0 && rate[index = arraymax(rate, (int)enum_Coinvalue::COIN_COUNT_NO_KRW, i)]>buylow){
#ifdef DEAL_DEBUG
                        fprintf(fp, "%s : %f\n",get_name_no_krw(name, index), rate[index]);
#endif
                        if(coin[(int)enum_Account::KRW]>(double)enum_Deal::IGNORE && coin[(int)enum_Account::KRW]>=buy_limit){
                                tradehandler->run(account, index, buy_limit/coinvalue[index]);
                                buy_limit -= buy_limit/coinvalue[index] * coinvalue[index];
                        }
                        if(coin[(int)enum_Account::KRW]>(double)enum_Deal::IGNORE && coin[(int)enum_Account::KRW]<buy_limit){
                                tradehandler->run(account, index, coin[(int)enum_Account::KRW]/coinvalue[index]);                       //최대 물량 = 보유한돈/물량 가격
                                buy_limit -= coin[(int)enum_Account::KRW]/coinvalue[index] * coinvalue[index];
                        }

                }

        }
};
class _Deal2
{
private:
        char name[64];
        double available_money;
        double buy_limit;
        double sell_limit;
        double buy_limit_rate;
        double sell_limit_rate;
	double buylow;
        int * coinvalue;
        _Tradehandler * tradehandler;
public:
        _Deal2(_Tradehandler * tradehandler):tradehandler(tradehandler){}
	void init_coinvalue(int * coinvalue)
	{
		this->coinvalue = coinvalue; 
	}
	void init(double buylimit, double selllimit, double buylow=0.001){
		buy_limit_rate = buylimit;
		sell_limit_rate = selllimit;
		this->buylow = buylow;
	}
        void run(_Account * account, double * rate)
        {
                available_money = account->available_money;
                const double * const coin = account->available_coin;
                buy_limit = available_money * buy_limit_rate;
                sell_limit = available_money * sell_limit_rate;
                int index;
                int i;

                i=1;
                while(i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW && sell_limit>0 && rate[index = arraymin(rate, (int)enum_Coinvalue::COIN_COUNT_NO_KRW, i++)]<0){
#ifdef DEAL_DEBUG
                        fprintf(fp, "%s : %f\n",get_name_no_krw(name, index), rate[index]);
#endif
                        if(coinvalue[index]*coin[index+(int)enum_Coinvalue::COIN_OFFSET]>(double)enum_Deal::IGNORE){
                                if(coinvalue[index]*coin[index+(int)enum_Coinvalue::COIN_OFFSET] > sell_limit){
                                        tradehandler->run(account, index, -sell_limit/coinvalue[index]);
                                        sell_limit -= sell_limit/coinvalue[index] * coinvalue[index];   //sell_limit = 0
                                }
                                else{
                                        tradehandler->run(account, index, -coin[index+(int)enum_Coinvalue::COIN_OFFSET]);
                                        sell_limit -= coin[index+(int)enum_Coinvalue::COIN_OFFSET] * coinvalue[index];  //sell_limit -= 수량 * 가격
                                }
                        }

                }

                i=1;
                if(buy_limit>0 && rate[index = arraymax(rate, (int)enum_Coinvalue::COIN_COUNT_NO_KRW, i)]>buylow){
#ifdef DEAL_DEBUG
                        fprintf(fp, "%s : %f\n",get_name_no_krw(name, index), rate[index]);
#endif
                        if((coin[(int)enum_Account::KRW]/2)>(double)enum_Deal::IGNORE){
                            if(coin[(int)enum_Account::KRW]/2 >= buy_limit/2){
                                tradehandler->run(account, index, buy_limit/2/coinvalue[index]);
                                buy_limit -= buy_limit/2/coinvalue[index] * coinvalue[index];
                            }
                            else{
                                tradehandler->run(account, index, (coin[(int)enum_Account::KRW] / 2 )/coinvalue[index]);                       //최대 물량 = 보유한돈/물량 가격
                                buy_limit -= (coin[(int)enum_Account::KRW]/2)/coinvalue[index] * coinvalue[index];
                            }
                        }

                }
                i=2;
                if(buy_limit>0 && rate[index = arraymax(rate, (int)enum_Coinvalue::COIN_COUNT_NO_KRW, i)]>buylow){
#ifdef DEAL_DEBUG
                        fprintf(fp, "%s : %f\n",get_name_no_krw(name, index), rate[index]);
#endif
                        if(coin[(int)enum_Account::KRW]>(double)enum_Deal::IGNORE && coin[(int)enum_Account::KRW]>=buy_limit){
                                tradehandler->run(account, index, buy_limit/coinvalue[index]);
                                buy_limit -= buy_limit/coinvalue[index] * coinvalue[index];
                        }
                        if(coin[(int)enum_Account::KRW]>(double)enum_Deal::IGNORE && coin[(int)enum_Account::KRW]<buy_limit){
                                tradehandler->run(account, index, coin[(int)enum_Account::KRW]/coinvalue[index]);                       //최대 물량 = 보유한돈/물량 가격
                                buy_limit -= coin[(int)enum_Account::KRW]/coinvalue[index] * coinvalue[index];
                        }

                }

        }
};


#endif
