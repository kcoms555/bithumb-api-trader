#include <cstdio>


#define __FP__
#define fp stdout

#include "enum.h"
#include <ctime>
#include "mysqlhandler.h"
#include "tradehandler.h"
#include "brainhandler.h"
#include "dealhandler.h"
#include "accounthandler.h"

int main(int argc, char * argv[]){
	FILE * result = fopen("result","w");
	time_t time_now;
        time(&time_now);
        fprintf(fp,"server time : %d\n", time_now);

        int coinvalue[(int)enum_Coinvalue::COIN_COUNT_NO_KRW];
        _Mysqlhandler mysqlhandler;
        _API api;
        _Brainhandler brainhandler(&mysqlhandler);
        _Tradehandler tradehandler(&mysqlhandler, &api);
        _Dealhandler dealhandler(&tradehandler);
        _Accounthandler accounthandler(&mysqlhandler, &api);

        accounthandler.init_run_member(&brainhandler, &dealhandler);
        accounthandler.init_coinvalue(coinvalue);
        dealhandler.init_deals();
        dealhandler.init_deals_coinvalue(coinvalue);
        tradehandler.init_coinvalue(coinvalue);

        int num;
        mysqlhandler.update_DB(BUFFER);
        printf("GETINIT timeshift ( dbcount 보다 작을것 dbcount : %d, dbindex : %d )\n", mysqlhandler.get_dbcount(), mysqlhandler.get_dbindex());
        scanf("%d", &num);
        int brain = atoi(argv[1]);
        int deal = atoi(argv[2]);
        double buylow;
        if(argc>3)
            buylow = atof(argv[3]);
        else
            buylow = 0.001;

for(int i=1;i<34;i++){
printf("%d\n",i);
for(int j=1;j<34;j++){
	accounthandler._Create_Account(brain,deal,1000000);
	accounthandler.update_all_account_money();
	mysqlhandler.set_timeshift(num);

	dealhandler.deal1.init(0.03*i,0.03*j,buylow);
	dealhandler.deal2.init(0.03*i,0.03*j,buylow);
	
	while(1)
        {
                mysqlhandler.get(coinvalue, (int)enum_Mysqlhandler::NOW);
                brainhandler.run_brains();
                brainhandler.get_rates();
                accounthandler.run_all_account();
                accounthandler.update_all_account_money();
                if(mysqlhandler.get_timeshift()==0)
                        break;
                else
                        mysqlhandler.run_timeshift();
        }
	accounthandler.fshow_all_account_rate(result, 1000000);
	fprintf(result, ",");
	accounthandler._Remove_All_Account();
}
	fprintf(result, "\n");
}
	fprintf(fp, "DONE\n");
	fclose(fp);
	fclose(result);
	return 0;
}

#include "enum.cpp"
#include "account.cpp"
#include "brainhandler.cpp"
#include "accounthandler.cpp"
#include "dealhandler.cpp"

/* compile :
 * g++ -I/usr/include/mariadb -lmariadb -lcrypto -lcurl -DTESTER -o getinit getinit.cpp
 *
 */
