#ifndef __BRAIN_HANDELR__
#define __BRAIN_HANDELR__

#include "enum.h"
#include "mysqlhandler.h"
#include "brain.h"

int get_sleeptime(int brain);
class _Brainhandler
{
private:
	_Brain0 brain0;
	_Brain1 brain1;
    _Brain2 brain2;
	_Brain3 brain3;
	_Brain4 brain4;
    _Brain5 brain5;
public:
	double * rate[(int)enum_Brainhandler::BRAINNUM];
	_Brainhandler(_Mysqlhandler * mysqlhandler);
	void run_brains(void);
	void get_rates(void);
	void init_brain(int brain, double buylow);
	void init_brains(void);
};
#endif
