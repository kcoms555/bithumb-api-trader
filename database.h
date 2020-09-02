#ifndef __DATABASE__
#define __DATABASE__
#include <cstring>
#include "enum.h"
struct _DB{
        int _index;
        int _timestamp;
        char status[5];
        int coinvalue[(int)enum_Coinvalue::COIN_COUNT_NO_KRW];
        _DB(){}
        _DB(int _index, int _timestamp, char * status, int * coinvalue):_index(_index), _timestamp(_timestamp)
        {
                strcpy(this->status, status);
                memcpy(this->coinvalue, coinvalue, sizeof(int) * (int)enum_Coinvalue::COIN_COUNT_NO_KRW);
        }
        _DB(const _DB &DB):_index(DB._index), _timestamp(DB._timestamp)
        {
                strcpy(this->status, DB.status);
                memcpy(this->coinvalue, DB.coinvalue, sizeof(int) * (int)enum_Coinvalue::COIN_COUNT_NO_KRW);
        }
	void show_DB(void)
	{
		char name[64];
		printf("_index : %d; _timestamp : %d; status : %s;\n coinvalue : ", _index, _timestamp, status);
		for(int i = 0; i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW; i++){
			get_name_no_krw(name, i);
			printf("%s : %d\n", name, coinvalue[i]);
		}
	}
	void show_DB(const _DB &DB)
	{
		char name[64];
		printf("_index : %d; _timestamp : %d; status : %s;\n coinvalue : ", DB._index, DB._timestamp, DB.status);
		for(int i = 0; i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW; i++){
			get_name_no_krw(name, i);
			printf("%s : %d\n", name, DB.coinvalue[i]);
		}
	}
	void show_DB_simple(void)
	{
		char name[64];
		printf("_index : %d; _timestamp : %d; status : %s;\n", this->_index, this->_timestamp, this->status);
	}
	void show_DB_simple(const _DB &DB)
	{
		char name[64];
		printf("_index : %d; _timestamp : %d; status : %s;\n", DB._index, DB._timestamp, DB.status);
	}
};

struct _ACCOUNT{
        int _index;
        int owner;
        char apikey[(int)enum_Account::KEYLEN];
        char secretkey[(int)enum_Account::KEYLEN];
        int brain;
        int deal;
        double coin[(int)enum_Account::COIN_COUNT];
};
#endif
