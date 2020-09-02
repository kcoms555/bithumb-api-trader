#include "enum.h"
char * get_name_no_krw(char * name, int n){
switch(n)
{
case 0: strcpy(name, "BTC"); break;
case 1: strcpy(name, "ETH"); break;
case 2: strcpy(name, "DASH"); break;
case 3: strcpy(name, "LTC"); break;
case 4: strcpy(name, "ETC"); break;
case 5: strcpy(name, "XRP"); break;
case 6: strcpy(name, "BCH"); break;
case 7: strcpy(name, "XMR"); break;
case 8: strcpy(name, "ZEC"); break;
case 9: strcpy(name, "QTUM"); break;
case 10: strcpy(name, "BTG"); break;
case 11: strcpy(name, "EOS"); break;
case 12: strcpy(name, "ICX"); break;
case 13: strcpy(name, "VEN"); break;
case 14: strcpy(name, "TRX"); break;
case 15: strcpy(name, "MITH"); break;
case 16: strcpy(name, "MCO"); break;
case 17: strcpy(name, "OMG"); break;
case 18: strcpy(name, "KNC"); break;
default: strcpy(name, "UNKNOWN COIN"); 
}
return name;
}
char * get_name(char * name, int n){
switch(n)
{
case 0: strcpy(name, "KRW"); break;
case 1: strcpy(name, "BTC"); break;
case 2: strcpy(name, "ETH"); break;
case 3: strcpy(name, "DASH"); break;
case 4: strcpy(name, "LTC"); break;
case 5: strcpy(name, "ETC"); break;
case 6: strcpy(name, "XRP"); break;
case 7: strcpy(name, "BCH"); break;
case 8: strcpy(name, "XMR"); break;
case 9: strcpy(name, "ZEC"); break;
case 10: strcpy(name, "QTUM"); break;
case 11: strcpy(name, "BTG"); break;
case 12: strcpy(name, "EOS"); break;
case 13: strcpy(name, "ICX"); break;
case 14: strcpy(name, "VEN"); break;
case 15: strcpy(name, "TRX"); break;
case 16: strcpy(name, "MITH"); break;
case 17: strcpy(name, "MCO"); break;
case 18: strcpy(name, "OMG"); break;
case 19: strcpy(name, "KNC"); break;
default: strcpy(name, "UNKNOWN COIN"); 
}
return name;
}
