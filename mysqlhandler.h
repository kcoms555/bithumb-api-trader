#ifndef __MYSQL_HANDLER__
#define __MYSQL_HANDLER__

#include <mariadb/mysql.h>
#include <cstdio>
#include <cstring>
#include "enum.h"
#include "account.h"
#include "database.h"

#define IP "localhost"
#define DATABASE "coin"
#define PORT 3306


#ifndef BUFFER
    #ifdef TESTER
    #define BUFFER 65536
    #else
    #define BUFFER 60
    #endif
#endif

class _Mysqlhandler
{
private:
	MYSQL mysql;
	MYSQL_RES * res;
	MYSQL_ROW row;
	int counts;
	double ACCOUNT_COIN[(int)enum_Account::COIN_COUNT];
	double ACCOUNT_COIN_ZERO[(int)enum_Account::COIN_COUNT];
	int COIN[(int)enum_Coinvalue::COIN_COUNT_NO_KRW];
	char query[1024];
	char name[64];
	char tmp[64];
	double total_money;
	double available_money;
	double in_use_money;


	//TIMESHIFTER
	_DB tmpDB;
	int dbcount;
	int dbindex;
	_DB * DB[BUFFER];
	int timeshift;
public:
	_Mysqlhandler():dbcount(0),dbindex(-1),timeshift(0)
	{
		for(int i = 0; i<(int)enum_Account::COIN_COUNT; i++)
			ACCOUNT_COIN_ZERO[i] = 0.0;
		mysql_init(&mysql);
		mysql_real_connect(&mysql, IP , ID, PASSWORD, DATABASE, PORT, (char *)NULL, 0);
	}
	~_Mysqlhandler()
	{
		if(dbcount!=0){
			for(int i = 0; i<dbcount; i++)
				delete DB[i];
			fprintf(fp, "DB : %d deleted!\n", dbcount);
		}
		if(this->res!=NULL)
			mysql_free_result(res);	
		mysql_close(&mysql);
	}
	void send(const char * const query)
	{
		mysql_query(&mysql, query);
	}
	MYSQL_RES * call(const char * const query)
	{
		mysql_query(&mysql, query);	
		res = mysql_store_result(&mysql);
		return res;
	}
	void test(){
		res = call("SELECT * FROM dbf where _index = 6841");
		printf("TEST\n");
		if(res==NULL)
			printf("res == NULL!");
		row = mysql_fetch_row(res);
		printf("TEST\n");
		printf("\n");
		if(row[0]==NULL)
			printf("row[i]==NULL");
		else
			printf("%d\n", atoi(row[0]));
		
		printf("TEST\n");
	}
	int update_DB(int n){
		if(n>BUFFER) n=BUFFER;
		sprintf(query, "SELECT * FROM db order by _index DESC limit 0, %d", n); //최신 데이터로부터 n개 로드
		res = call(query);
		int i=0;
		while(i<n){
			row = mysql_fetch_row(res);
			if(row!=NULL){
				for(int j=0;j<(int)enum_Coinvalue::COIN_COUNT_NO_KRW;j++)
					if(row[3+j]==NULL)
						tmpDB.coinvalue[j]=0;
					else
						tmpDB.coinvalue[j]=atoi(row[3+j]);
				if(row[0]==NULL) tmpDB._index=-1; else tmpDB._index=atoi(row[0]);
				if(row[1]==NULL) tmpDB._timestamp=0; else tmpDB._timestamp=atoi(row[1]);
				if(row[2]==NULL) strcpy(tmpDB.status,"9999"); else strcpy(tmpDB.status, row[2]);
				DB[i]=new _DB(tmpDB);
				i++;
			}
			else
				break;
		}
		dbcount = i;
		dbindex = i-1;
	}
	void show_DB(void){
		for(int i=0; i<dbcount; i++)
			DB[i]->show_DB();
	}
	int set_timeshift(int n){
		if(n>dbindex){fprintf(fp, "SET_TIMESHIFT ERROR!"); timeshift=-1; return -1;}
		timeshift = n;
#ifdef DEBUG
		fprintf(fp, "setting timeshift : %d, index : %d\n", timeshift, get_index());
#endif
		return 0;
	}
	int run_timeshift(void){
		if(timeshift==0){fprintf(fp, "RUN_TIMESHIFT ERROR!"); timeshift=-1; return -1;}
		timeshift--;
		return 0;
	}
	void show_timeshift(void){
		fprintf(fp, "timeshift : %d, index : %d\n", timeshift, get_index());
	}
	int get_timeshift(void){
		return timeshift;
	}
	int get_dbindex(void){
		return dbindex;
	}
	int get_dbcount(void){
		return dbcount;
	}
	int get_index(int n){
		if(n+timeshift>dbindex) n=dbindex-timeshift;
		return DB[n+timeshift]->_index;
	}
	int get_index(void){
		return get_index(0);
	}
	void get(int * coinvalue, int n){
		if(n+timeshift>dbindex) n=dbindex-timeshift;
        memcpy(coinvalue, DB[n+timeshift]->coinvalue, sizeof(int)*(int)enum_Coinvalue::COIN_COUNT_NO_KRW);
	}

/*
	int get_index_old(int n){
		sprintf(query, "SELECT _index FROM db order by _index DESC limit %d, 1", n);
		res = call(query);
		row = mysql_fetch_row(res);
		if(row[0]==NULL)
			return -1;
		else
			return atoi(row[0]);
	}
	void get_old(int * coinvalue, int n){
		sprintf(query, "SELECT * FROM db order by _index DESC limit %d, 1", n);
		res = call(query);
		row = mysql_fetch_row(res);
		for(int i=0;i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW;i++)
			if(row[3+i]==NULL)
				coinvalue[i]=0;
			else
				coinvalue[i]=atoi(row[3+i]);
	}
	void get_old(int * coinvalue){
		res = call("SELECT * FROM db order by _index DESC limit 0, 1");
		row = mysql_fetch_row(res);
		for(int i=0;i<(int)enum_Coinvalue::COIN_COUNT_NO_KRW;i++)
			if(row[3+i]==NULL)
				coinvalue[i]=0;
			else
				coinvalue[i]=atoi(row[3+i]);
	}
*/
	int _Load_TestAccount_with_coin(_Account ** account){
		counts = 0;
		res = call("SELECT * FROM TEST_ACCOUNT");
		while((row = mysql_fetch_row(res))!=NULL){
			for(int i=0;i<(int)enum_Account::COIN_COUNT;i++)
				ACCOUNT_COIN[i]=atof(row[6+i]);
			account[counts] = new _Account(atoi(row[0]),atoi(row[1]),row[2],row[3],atoi(row[4]),atoi(row[5]));
			account[counts]->set_total_coin(ACCOUNT_COIN);
			account[counts]->set_available_coin(ACCOUNT_COIN);
			account[counts]->set_in_use_coin(ACCOUNT_COIN_ZERO);
			counts++;
		}
		return counts;
	}
	int _Load_Account(_Account ** account){
		counts = 0;
		res = call("SELECT * FROM ACCOUNT");
		while((row = mysql_fetch_row(res))!=NULL){
			account[counts++] = new _Account(atoi(row[0]),atoi(row[1]),row[2],row[3],atoi(row[4]),atoi(row[5]));
		}
		return counts;
	}
	void get_account_coin_from_db(_Account * account)
	{
		sprintf(query, "SELECT * FROM ACCOUNT WHERE _index = %d", account->_index);
		res = call(query);
		if( (row = mysql_fetch_row(res) ) !=NULL ){
			for(int i=0;i<(int)enum_Account::COIN_COUNT;i++)
				ACCOUNT_COIN[i]=atof(row[6+i]);
			account->set_total_coin(ACCOUNT_COIN);
			account->set_available_coin(ACCOUNT_COIN);
			account->set_in_use_coin(ACCOUNT_COIN_ZERO);
		}
		else
			account->set_all_coin_error();
	}
	void _Save_Account(_Account ** account, int counts){
		for(int i=0;i<counts;i++){
			sprintf(query, "UPDATE ACCOUNT SET KRW=%f", account[i]->total_coin[0]);
			for(int j=1;j<(int)enum_Account::COIN_COUNT;j++){
				strcat(query, ", ");
				get_name(name, j);
				strcat(query, name);
				sprintf(tmp,"=%f", account[i]->total_coin[j]);
				strcat(query, tmp);
			}
			sprintf(tmp," where _index=%d", account[i]->_index);
			strcat(query, tmp);
			send(query);
			sprintf(query, "UPDATE TRADESUM SET money = money + %f where _index = %d", account[i]->tradesum, account[i]->_index);
			send(query);
		}	
	}
};
#endif
