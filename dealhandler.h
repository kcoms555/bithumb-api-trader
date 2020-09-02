#ifndef __DEAL_HANDLER__
#define __DEAL_HANDLER__

#include "enum.h"
#include "mysqlhandler.h"
#include "deal.h"

class _Dealhandler
{
public:
	_Deal0 deal0;
	_Deal1 deal1;
	_Deal2 deal2;

	_Dealhandler(_Tradehandler * tradehandler);
	void init_deals_coinvalue(int * coinvalue);
    void init_deals(double buylimit=0.5, double selllimit=1.0, double buylow=0.000);
    void init_deal(int deal, double buylimit=0.5, double selllimit=1.0, double buylow=0.000);
	void run(_Account * account, double * rate);
};
#endif
