#include "brainhandler.h"
int get_sleeptime(int brain){
	switch(brain){
		case 1: return 290;
		case 2: return 290;
		case 3: return 290;
		case 4: return 1700;
		case 5: return 290;
		default: return 50;
	}
}
_Brainhandler::_Brainhandler(_Mysqlhandler * mysqlhandler):brain1(mysqlhandler),brain2(mysqlhandler), brain3(mysqlhandler),brain4(),brain5(mysqlhandler){}
void _Brainhandler::run_brains(void)
{
	brain0.run();
	brain1.run();
	brain2.run();
	brain3.run();
	brain4.run();
	brain5.run();
}
void _Brainhandler::get_rates(void)
{
	//각 brain들은 반드시 run행동이 실행되어야함
	brain0.get(&rate[0]);
	brain1.get(&rate[1]);
	brain2.get(&rate[2]);
	brain3.get(&rate[3]);
	brain4.get(&rate[4]);
	brain5.get(&rate[5]);
}
void _Brainhandler::init_brain(int brain, double buylow)
{
	switch(brain){
		case 1: brain1.set_buylow(buylow); break;
		case 2: brain2.set_buylow(buylow); break;
		case 3: brain3.set_buylow(buylow); break;
		case 4: brain4.set_buylow(buylow); break;
		default: fprintf(stderr, "init_brain() error!"); exit(1);
	}
}
