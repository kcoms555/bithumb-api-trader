#ifndef __ENUM__
#define __ENUM__
#include <cstring>

enum class enum_API{
	HASH_STR_LEN = 128,
	PARCHAR = 1
};
enum class enum_Deal{
	IGNORE = 1000
};
enum class enum_Mysqlhandler
{
	NOW = 0,
	PAST1 = 1,
	PAST2 = 2,
	PAST3 = 3,
	PAST4 = 4,
	PAST5 = 5
};
enum class enum_Brainhandler
{
	BRAINNUM = 10
};
enum class enum_Accounthandler
{
        ACCOUNTNUM = 20
};

char * get_name_no_krw(char * name, int n);
enum class enum_Coinvalue{
	COIN_OFFSET = 1,	//COIN_COUNT_NO_KRW + COIN_OFFSET = COIN_COUNT
	COIN_COUNT_NO_KRW = 19,
	FIRSTCOIN = 0,
	BTC = 0,
	ETH = 1,
	DASH = 2,
	LTC = 3,
	ETC = 4,
	XRP = 5,
	BCH = 6,
	XMR = 7,
	ZEC = 8,
	QTUM = 9,
	BTG = 10,
	EOS = 11,
	ICX = 12,
	VEN = 13,
	TRX = 14,
	MITH = 15,
	MCO = 16,
	OMG = 17,
	KNC = 18,
	LASTCOIN = 18
};
char * get_name(char * name, int n);
enum class enum_Account{
        KEYLEN = 32+1, //32byte의 key
        COIN_COUNT = 20,
        FIRSTCOIN = 0,
        KRW = 0,
        BTC = 1,
        ETH = 2,
        DASH = 3,
        LTC = 4,
        ETC = 5,
        XRP = 6,
        BCH = 7,
        XMR = 8,
        ZEC = 9,
        QTUM = 10,
        BTG = 11,
        EOS = 12,
        ICX = 13,
        VEN = 14,
        TRX = 15,
        MITH = 16,
        MCO = 17,
        OMG = 18,
        KNC = 19,
        LASTCOIN = 19
};
#endif

