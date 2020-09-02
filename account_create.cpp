#include "mysqlhandler.h"

int main(void){
	_Mysqlhandler mysqlhandler;
	int selection, count, owner, brain, deal;
	double KRW;
	char query[1024];
	printf("select action : 1(CREATE TEST_ACCOUNT); 2(DELETE TEST_ACCOUNT); 3(CREATE ACCOUNT); 4(DELETE ACCOUNT);\n");
	scanf("%d", &selection);
	fflush(stdin);


	switch(selection){
	case 1 :
		printf("INSERT number of count, owner, brain, deal, KRW\n");
		scanf("%d %d %d %d %lf", &count, &owner, &brain, &deal, &KRW);
		fflush(stdin);
		for(int i=0;i<count;i++){
			sprintf(query, "insert into TEST_ACCOUNT(owner, brain, deal, KRW) value(%d, %d, %d, %f)", owner, brain, deal, KRW);
			mysqlhandler.send(query);
		}
		break;
	case 2 :
		mysqlhandler.send("delete from TEST_ACCOUNT");
		break;
	case 3 : 
		printf("INSERT number of count, owner, brain, deal, KRW\n");
		scanf("%d %d %d %d %lf", &count, &owner, &brain, &deal, &KRW);
		fflush(stdin);
		for(int i=0;i<count;i++){
			sprintf(query, "insert into ACCOUNT(owner, brain, deal, KRW) value(%d, %d, %d, %f)", owner, brain, deal, KRW);
			mysqlhandler.send(query);
		}
		break;
	case 4 :
		mysqlhandler.send("delete from ACCOUNT");
		break;
	default :
		printf("default!\n");

	}
	return 0;
}
/* compile :
g++ -I/usr/include/mariadb -lmariadb -o account_create account_create.cpp
*/
