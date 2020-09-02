#include "dealhandler.h"
_Dealhandler::_Dealhandler(_Tradehandler * tradehandler):deal0(tradehandler),deal1(tradehandler),deal2(tradehandler){}
void _Dealhandler::init_deals_coinvalue(int * coinvalue)
{
	deal0.init_coinvalue(coinvalue);
	deal1.init_coinvalue(coinvalue);
	deal2.init_coinvalue(coinvalue);
}
void _Dealhandler::init_deals(double buylimit, double selllimit, double buylow)
{
	deal1.init(buylimit, selllimit, buylow);
	deal2.init(buylimit, selllimit, buylow);
}
void _Dealhandler::init_deal(int deal, double buylimit, double selllimit, double buylow)
{
	switch(deal){
		case 0: deal0.init(buylimit, selllimit, buylow); break;
		case 1: deal1.init(buylimit, selllimit, buylow); break;
		case 2: deal2.init(buylimit, selllimit, buylow); break;
		default: fprintf(fp, "_Dealhandler run error!\n");
	}
}
void _Dealhandler::run(_Account * account, double * rate)
{
	switch(account->deal){
		case 0: deal0.run(account, rate); break;
		case 1: deal1.run(account, rate); break;
		case 2: deal2.run(account, rate); break;
		default: fprintf(fp, "_Dealhandler run error!\n");
	}
}
