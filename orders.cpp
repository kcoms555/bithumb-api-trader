#include <cstdio>

#define __FP__
#define fp stdout

#include <ctime>
#include "mysqlhandler.h"
#include "accounthandler.h"
#include "api.h"

int main(void){
	time_t time_now;
	time(&time_now);
	fprintf(fp,"server time : %d\n", time_now);

	_Mysqlhandler mysqlhandler;
	_API api;
	_Accounthandler accounthandler(&mysqlhandler, &api);

	accounthandler._Load_Account_From_db();
	accounthandler.show_all_orders_account();

	return 0;
}

#include "accounthandler.cpp"
/* compile :
g++ -I/usr/include/mariadb -lmariadb -lcurl -lcrypto -o orders orders.cpp
*/
