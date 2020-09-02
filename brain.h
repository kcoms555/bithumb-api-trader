#ifndef __BRAIN__
#define __BRAIN__

#include "enum.h"
#include "mysqlhandler.h"
#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdio>
namespace BRAIN
{
    void ban(double * rate){
        rate[(int)enum_Coinvalue::TRX] = -1.0;
    }
    void set_zero(double * rate){
        memset(rate, 0, sizeof(double)*(int)enum_Coinvalue::COIN_COUNT_NO_KRW);
    }
}
class _Brain0
{
private:
	double rate[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //rate : 현재값 대비 미래 증감률
public:	
	_Brain0(){}
	void run(void)
	{
        BRAIN::set_zero(rate);
	}
	void get(double ** rate)
	{
		*rate=this->rate;
	}
};

class _Brain1
{
private:
    int coin0[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //과거값
    int coin1[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //현재값
    double rate[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //rate : 현재값 대비 미래 증감률
    double buylow;
	_Mysqlhandler * mysqlhandler;
public:
    _Brain1(_Mysqlhandler * mysqlhandler):mysqlhandler(mysqlhandler),buylow(0.01){}
	void run(void)
    {
	    int _index = mysqlhandler->get_index();
        if(_index==-1 || _index%5!=0){ //5분마다 rate가 0이 아님!
            BRAIN::set_zero(rate);
            return;
        }
        mysqlhandler->get(coin1, 0);
        mysqlhandler->get(coin0, 5);
        for(int i=0;i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW;i++)
            if(coin1[i]==0 || coin0[i]==0){
                BRAIN::set_zero(rate);
                break;
            }
            else{
                rate[i]=(double)(coin0[i]-coin1[i])/coin1[i];
                if(rate[i]<buylow && rate[i]>0)
                    rate[i]=0.0;
            }
        BRAIN::ban(rate);
    }
    void get(double ** rate){*rate=this->rate;}
    void set_buylow(double buylow){this->buylow = buylow;}
};
class _Brain2
{
private:
    int coin0[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //과거값
    int coin1[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //현재값
    double rate[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //rate : 현재값 대비 미래 증감률
    double buylow;
	_Mysqlhandler * mysqlhandler;
public:
    _Brain2(_Mysqlhandler * mysqlhandler):mysqlhandler(mysqlhandler),buylow(0.01){}
	void run(void)
    {
		int _latest_index;
		int _index = mysqlhandler->get_index();
		FILE * fp = fopen("/home/pi/aip/brain2", "r");
		if(fp != NULL){
			fscanf(fp, "%d", &_latest_index);
			fclose(fp);
			if((_index - _latest_index) % 10 != 0){
                BRAIN::set_zero(rate);
				return;
			}
			else{
				remove("/home/pi/aip/brain2");
			}
		}
        mysqlhandler->get(coin1, 0);
        mysqlhandler->get(coin0, 5);
        for(int i=0;i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW;i++){
            if(coin1[i]==0 || coin0[i]==0){
                BRAIN::set_zero(rate);
                break;
            }
            else{
                rate[i]=(double)(coin0[i]-coin1[i])/coin1[i];
                if(rate[i]<buylow && rate[i]>0)
                    rate[i]=0.0;
			}
		}
        BRAIN::ban(rate);
        for(int i=0;i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW;i++){
			if(rate[i]>buylow){
				FILE * fp = fopen("/home/pi/aip/brain2", "w");
				fprintf(fp, "%d", _index);
				fclose(fp);
			}
		}
    }
    void get(double ** rate){*rate=this->rate;}
    void set_buylow(double buylow){this->buylow = buylow;}
};
class _Brain3
{
private:
    int coin[10][(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //과거값
    double rate[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //rate : 현재값 대비 미래 증감률
	double avr_rate;
    double buylow;
	double avr;
	_Mysqlhandler * mysqlhandler;
public:
    _Brain3(_Mysqlhandler * mysqlhandler):mysqlhandler(mysqlhandler),buylow(0.01){}
	void run(void)
    {
		int _latest_index;
		int _index = mysqlhandler->get_index();
		FILE * fp = fopen("/home/pi/aip/brain3", "r");
		if(fp != NULL){
			fscanf(fp, "%d", &_latest_index);
			fclose(fp);
			if((_index - _latest_index) % 10 != 0){
                BRAIN::set_zero(rate);
				return;
			}
			else{
				remove("/home/pi/aip/brain3");
			}
		}
		for(int i=0;i<10;i++)
        	mysqlhandler->get(coin[i], i*5);
        for(int i=0;i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW;i++){
            if(coin[0][i] && coin[1][i] && coin[2][i] && coin[3][i] && coin[4][i] && coin[5][i]
				&& coin[6][i] && coin[7][i] && coin[8][i] && coin[9][i]){ // and연산자로 어느 하나가 0이면 거짓으로 판단
                rate[i]=(double)(coin[1][i]-coin[0][i])/coin[0][i];
				avr = (double)(coin[0][i] + coin[1][i] +  coin[2][i] +  coin[3][i] +  coin[4][i] + coin[5][i] + coin[6][i] + coin[7][i] + coin[8][i] + coin[9][i])/10;
				avr_rate= (double)(avr - coin[0][i])/coin[0][i];

				rate[i] = (rate[i]>avr_rate) ? avr_rate : rate[i];
                if(rate[i]<buylow && rate[i]>0)
                    rate[i]=0.0;
            }
            else{
                BRAIN::set_zero(rate);
                break;
			}
		}
        BRAIN::ban(rate);
        for(int i=0;i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW;i++){
			if(rate[i]>buylow){
				FILE * fp = fopen("/home/pi/aip/brain3", "w");
				fprintf(fp, "%d", _index);
				fclose(fp);
			}
		}
    }
    void get(double ** rate){*rate=this->rate;}
    void set_buylow(double buylow){this->buylow = buylow;}
};
class _Brain4
{
private:
    double rate[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //rate : 현재값 대비 미래 증감률
    double buylow;
public:
    _Brain4(void):buylow(0.01){}
	void run(void)
    {
		BRAIN::set_zero(rate);
		int stack;
		int brain4_wait = 29;
		FILE * fp;
		fp = fopen("/home/pi/aip/brain4_stack","r");
		if(fp != NULL){
			fscanf(fp,"%d",&stack);
			fclose(fp);
			if(stack>0){
				stack--;
				fp = fopen("/home/pi/aip/brain4_stack","w");
				fprintf(fp, "%d", stack);
				fclose(fp);
				return;
			}
		}
		double prd = 0.0;
		fp = fopen("/home/pi/tf/predict_BTC","r");
		if(fp == NULL)
			return;
		fscanf(fp, "%lf", &prd);
		fclose(fp);
		fp = fopen("/home/pi/tf/predict_BTC","w");	//데이터 읽으면 삭제
		fprintf(fp, "0.0");
		fclose(fp);
		if(prd < this->buylow && prd > 0.0)
			this->rate[(int)enum_Coinvalue::BTC] = 0.0;
		else{
			this->rate[(int)enum_Coinvalue::BTC] = prd;
			if(prd>0.0){
				fp = fopen("/home/pi/aip/brain4_stack", "w");
				fprintf(fp,"%d", brain4_wait); //매수신호 보낼시 2시간 대기
				fclose(fp);
			}
		}
    }
    void get(double ** rate){*rate=this->rate;}
    void set_buylow(double buylow){this->buylow = buylow;}
};
class _Brain5
{
private:
    int coin_0[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //현재값
    int coin_5[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //5분전 값
    int coin_10[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //10분전 값
    double rate[(int)enum_Coinvalue::COIN_COUNT_NO_KRW]; //rate : 현재값 대비 미래 증감률
	_Mysqlhandler * mysqlhandler;
public:
    _Brain5(_Mysqlhandler * mysqlhandler):mysqlhandler(mysqlhandler){}
	void run(void)
	{
		int _index = mysqlhandler->get_index();
        if(_index == -1 || _index%5!=0){
            BRAIN::set_zero(rate);
            return;
        }
	    mysqlhandler->get(coin_0, 0);
        mysqlhandler->get(coin_5, 5);
        mysqlhandler->get(coin_10,10);

        double slope_5_0;
        double slope_10_5;

        for(int i=0;i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW;i++)
            if(coin_0[i]==0 || coin_5[i]==0 || coin_10[i]==0){
                BRAIN::set_zero(rate);
                break;
            }
            else
            {
                slope_5_0 = (double)(coin_0[i] - coin_5[i])/coin_0[i];
                slope_10_5 = (double)(coin_5[i] - coin_10[i])/coin_0[i];

                if(slope_10_5 - slope_5_0 < 0){
                    if(slope_5_0 < 0){
                        if(fabs(slope_10_5) > 1.5 * fabs(slope_5_0)){
                            rate[i] = -(slope_10_5 - slope_5_0);
                        }
                        else
                            rate[i]=0.0;
                    }
                    else
                        rate[i] = 0.0;
                }
                else{
                    if(slope_5_0 < 0)
                        rate[i] = 0.0;
                    else
                        rate[i] = -(slope_10_5 - slope_5_0);
                    
                }
            }
        BRAIN::ban(rate);
	}
    void get(double ** rate)
    {
        *rate=this->rate;
    }
};
#endif
