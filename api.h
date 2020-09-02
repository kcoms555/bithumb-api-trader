#ifndef __API__ 
#define __API__ 
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <curl/curl.h>
#include <sys/time.h>
#include <limits.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include "account.h"
#include "enum.h"
#include "dread.h"
#include <unistd.h>
#include "brainhandler.h"

#define _API_HOST "https://api.bithumb.com"
#define clientTypeBuffer "api-client-type:1"
#define FEE 0.0001
#define TOTAL_LIMIT 30000

#ifndef __FP__ 
#define fp stdout 
#endif 

#ifndef VERSION
#define VERSION 0.00
#endif

struct dataset{size_t size;char* data;};
size_t write_data( void *contents, size_t size, size_t nmemb, void * data );

class _API
{
	private:
		static const char base64_pad = '=';
		void base64_encode(const char *str, int length, char * base64_str);
		void url_encode(const char *str, char * strEncode);
		int hash_sha512(char *hash, const unsigned char *in, const char *secret);
		void updatetime(void);
		char url[128];
		char strPost[128];
		char strEncode[128];
		char strData[128];
		struct timeval time;
		char mtStr[14];
		char hash_str[(int)enum_API::HASH_STR_LEN+1];
		char base64_str[256];
		char keyBuffer[128];
		char resBuffer[256];
		char nonceBuffer[64];
		CURL *curl;
		CURLcode res;
		char ret[128];
		double available_coin[(int)enum_Account::COIN_COUNT];
		double coin[(int)enum_Account::COIN_COUNT];
		double in_use_coin[(int)enum_Account::COIN_COUNT];
		char parameter[256];
		char parameter2[256];
		char name[20];
		char type[10];
		struct dataset data;
		char d_BaseDir[100];
		int tradevaild;
		char message[256];
		char buffer[256];
	public:
		_API();
		~_API();
		static const char base64_table[];
		void trade(const _Account * account, const int COIN, const int price, const double volume)
		{
			if(tradevaild == -1){
				fprintf(fp, "tradevaild : -1, trade refused!\n");
				return;
			}

			const double fvolume = fabs(volume)-FEE;
			get_name_no_krw(name, COIN);
			if(volume>0)
				strcpy(type, "bid");
			else
				strcpy(type, "ask");

			if(account->deal!=0){
				if(account->total_money < TOTAL_LIMIT){
					updatetime();
					sprintf(message, "%.2f %d %d %.0f %5s %s %8.0f %8.4f %8d; total money < %d, trade refused!\n", VERSION, time.tv_sec, time.tv_sec/86400, account->total_money, name, type, fvolume * price, fvolume, price, TOTAL_LIMIT);
					account->fwrite_log(message);
					return;
				}
			}
			/*
			   if(account->get_stack() == 3){
			   fprintf(fp, "too many stack! trade refused!\n");
			   account->set_stack_zero();
			   return;
			   }
			   account->set_stack_inc();
			 */

			//sleep(1);

			data.size=0;
			data.data=(char*)malloc(1);
			sprintf(parameter, "order_currency=%s&Payment_currency=KRW&units=%.4f&price=%d&type=%s", name, fvolume, price, type);
			fprintf(fp, "API::trade::parameter : %s\n", parameter);
			request(account, (char *)"/trade/place", parameter);
			fprintf(fp, "API::trade::server return : %s\n", data.data);
			dread(ret, data.data, 1, "status");
			if(strcmp(ret, "0000") == 0)
			{
				int sleeptime = get_sleeptime(account->brain);
				dread(ret, data.data, 1, "order_id");
				sprintf(parameter2, "%s/cancel %d %d %s %s %s %s %s %s &", d_BaseDir, sleeptime, account->_index, account->apikey, account->secretkey, type, ret, name, parameter);
				system((char *)parameter2);
				sprintf(message, "%.2f %d %d %.0f %5s %s %8.0f %8.4f %8d;", VERSION, time.tv_sec, time.tv_sec/86400, account->total_money, name, type, fvolume * price, fvolume, price);
				account->fwrite_log(type, ret, message);
			}
			else
			{
				sprintf(message, "%.2f %d %d %.0f %5s %s %8.0f %8.4f %8d; %s\n", VERSION, time.tv_sec, time.tv_sec/86400, account->total_money, name, type, fvolume * price, fvolume, price, data.data);
				account->fwrite_log(message);
			}

			account->set_stack_zero();
			free(data.data);
		}
		void show_orders(const _Account * account, const char * type, int currency)
		{
			data.size=0;
			data.data=(char*)malloc(1);

			char name[20];
			get_name_no_krw(name, currency);
			updatetime();
			sprintf(parameter, "order_id=1&type=%s&after=%s&currency=%s", type, mtStr, name);
			//order_id, type, count, after, currency
			fprintf(fp, "orders :: parameter : %s\n", parameter);
			request(account, (char *)"/trade/orders", parameter);
			fprintf(fp, "orders :: request : %s\n", data.data);
			free(data.data);
		}
		void cancel(const _Account * account, const char * type, const char * order_id, const char * currency)
		{
			int ch;
			sprintf(buffer, "%s/../account/%d/pending/%s/%s", d_BaseDir, account->_index, type, order_id);
			FILE * fp=fopen(buffer, "r");
			sprintf(buffer, "%s/../account/%d/log", d_BaseDir, account->_index);
			FILE * account_fp = fopen(buffer, "a");
			while(1){
				ch = fgetc(fp);
				if(ch == EOF)
					break;
				fputc(ch, account_fp);
			}

			data.size=0;
			data.data=(char*)malloc(1);
			sprintf(parameter, "type=%s&order_id=%s&currency=%s", type, order_id, currency);
			request(account, (char *)"/trade/cancel", parameter);
			dread(ret, data.data, 1, "status");

			if(strcmp(ret, "0000") == 0){
				fputs(" canceled!;\n", account_fp);
				remove("/home/pi/brain2");
			}
			else
				fputs(" trade done!;\n", account_fp);

			fclose(fp);
			fclose(account_fp);

			sprintf(buffer, "%s/../account/%d/pending/%s/%s", d_BaseDir, account->_index, type, order_id);
			remove(buffer);
			free(data.data);
		}
		void show_krw_deposit(_Account * account);
		void show_btc_address(_Account * account);
		void request(const _Account * account, char * endpoint, char * post_data);
		void show_transactions(_Account * account, int currency);
		void get_account_coin_from_server(_Account * account);
		void perform(struct curl_slist * cslist);
		void account_record(_Account * account, const int message_type, const char * message);
		void set_d_BaseDir(const char * d_BaseDir);
		void trade_activation(void);
		void trade_disabled(void);
};

size_t write_data( void *contents, size_t size, size_t nmemb, void * data )
{
	size_t extsize = size * nmemb;
	struct dataset * mem = (struct dataset *)data;
	(*mem).data = (char *)realloc((*mem).data, (*mem).size + extsize + 1);
	memcpy(&mem->data[mem->size], contents, extsize);
	mem->size += extsize;
	mem->data[mem->size]=0;
	return extsize;
}

const char _API::base64_table[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
};
void _API::base64_encode(const char *str, int length, char * base64_str)
{
	const char *current = str;
	char *p;
	char *result;

	result = (char *) malloc(((length + 2) / 3) * (4 * sizeof(char)) + 1);
	p = result;

	while (length > 2) { /* keep going until we have less than 24 bits */
		*p++ = base64_table[current[0] >> 2];
		*p++ = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
		*p++ = base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
		*p++ = base64_table[current[2] & 0x3f];

		current += 3;
		length -= 3; /* we just handle 3 octets of data */
	}

	/* now deal with the tail end of things */
	if (length != 0) {
		*p++ = base64_table[current[0] >> 2];
		if (length > 1) {
			*p++ = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
			*p++ = base64_table[(current[1] & 0x0f) << 2];
			*p++ = base64_pad;
		} else {
			*p++ = base64_table[(current[0] & 0x03) << 4];
			*p++ = base64_pad;
			*p++ = base64_pad;
		}
	}


	*p = '\0';
	strcpy(base64_str, result);
	free(result);
}
void _API::url_encode(const char *str, char * strEncode)
{
	char *encstr, buf[2+1];
	unsigned char c;
	int i, j;

	encstr = (char *)malloc( (strlen(str) * 3) + 1);
	for(i = j = 0; str[i]; i++)
	{
		c = (unsigned char)str[i];
		if((c >= '0') && (c <= '9')) encstr[j++] = c;
		else if((c >= 'A') && (c <= 'Z')) encstr[j++] = c;
		else if((c >= 'a') && (c <= 'z')) encstr[j++] = c;
		else if((c == '@') || (c == '.') || (c == '=') || (c == '\\')
				|| (c == '-') || (c == '_') || (c == ':') || (c == '&') )
			encstr[j++] = c;
		else
		{
			sprintf(buf, "%02X", c);
			encstr[j++] = '%';
			encstr[j++] = buf[0];
			encstr[j++] = buf[1];
		}
	}
	encstr[j] = '\0';
	strcpy(strEncode, encstr);
	free(encstr);
}
int _API::hash_sha512(char *hash, const unsigned char *in, const char *secret)
{
	HMAC_CTX * ctx;
	ctx = HMAC_CTX_new();
	unsigned int len = 128;
	char out[len];

	// using sha512
	HMAC_Init_ex( ctx, secret, strlen(secret), EVP_sha512(), NULL );
	HMAC_Update( ctx, in, strlen((char*)in) );
	HMAC_Final( ctx, (unsigned char*)out, &len );
	HMAC_CTX_free(ctx);

	for(int i=0; i<len; i++)
		sprintf(&hash[i*2], "%02x", out[i]);
}
void _API::updatetime(void)
{
	gettimeofday(&time,NULL);
	sprintf(mtStr, "%d%03d", time.tv_sec,(int)(time.tv_usec/1000));
}
_API::_API()
{
	trade_disabled();
	set_d_BaseDir(".");
	curl = curl_easy_init();
	curl_easy_setopt( curl, CURLOPT_POST, 1 );
	curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0 );
	curl_easy_setopt( curl, CURLOPT_POSTFIELDS, strEncode );
	curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, &write_data );
	curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&data );
}
_API::~_API()
{
	curl_easy_cleanup(curl);
}
void _API::show_krw_deposit(_Account * account)
{
	data.size=0;
	data.data=(char*)malloc(1);
	request(account, (char *)"/trade/krw_deposit", (char *)"xcoin=1");
	fprintf(fp, "%s\n",data.data);
	free(data.data);
}
void _API::show_btc_address(_Account * account)
{
	data.size=0;
	data.data=(char*)malloc(1);
	request(account, (char *)"/info/wallet_address", (char *)"currency=btc");
	fprintf(fp, "%s\n",data.data);
	free(data.data);
}
void _API::request(const _Account * account, char * endpoint, char * post_data)
{
	sprintf(url, "%s%s", _API_HOST, endpoint);
	sprintf(strPost, "%s%s%s%s", "endpoint=",endpoint,"&",post_data);
	url_encode(strPost, strEncode);
	updatetime();

	sprintf(strData, "%s%c%s%c%s", endpoint, (char)1, strEncode, (char)1, mtStr);
	hash_sha512(hash_str, (unsigned char*)strData, account->secretkey);
	base64_encode(hash_str, strlen(hash_str), base64_str);

	sprintf(keyBuffer, "%s%s", "Api-Key: ", account->apikey);
	sprintf(resBuffer, "%s%s", "Api-Sign:", base64_str);
	sprintf(nonceBuffer, "%s%s", "Api-Nonce:", mtStr);

	struct curl_slist * cslist = NULL;
	cslist = curl_slist_append(cslist, keyBuffer);
	cslist = curl_slist_append(cslist, resBuffer);
	cslist = curl_slist_append(cslist, nonceBuffer);
	cslist = curl_slist_append(cslist, clientTypeBuffer);
	perform(cslist);
	curl_slist_free_all( cslist );
}
void _API::show_transactions(_Account * account, int currency)
{
	data.size=0;
	data.data=(char*)malloc(1);
	char name[20];
	get_name_no_krw(name, currency);
	sprintf(parameter, "searchGb=0&currency=%s", name);
	request(account, (char *)"/info/user_transactions", parameter);
	fprintf(fp, "%s\n", data.data);
	free(data.data);
}
void _API::get_account_coin_from_server(_Account * account)
{
	data.size=0;
	data.data=(char*)malloc(1);
	request(account, (char *)"/info/balance", (char *)"currency=all");
#ifdef API_DEBUG
	fprintf(fp, "%s\n", data.data);
#endif
	dread(ret, data.data, 1, "status");
	if(strcmp(ret, "0000") != 0)
	{
		fprintf(fp, "status error!, status : %s\n", ret);
		account->set_all_coin_error();
		return;
	}

	dread(ret, data.data, 2, "data", "total_krw");
	coin[0]=atof(ret);
	dread(ret, data.data, 2, "data", "available_krw");
	available_coin[0]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_krw");
	in_use_coin[0]=atof(ret);
	dread(ret, data.data, 2, "data", "total_btc");
	coin[1]=atof(ret);
	dread(ret, data.data, 2, "data", "available_btc");
	available_coin[1]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_btc");
	in_use_coin[1]=atof(ret);
	dread(ret, data.data, 2, "data", "total_eth");
	coin[2]=atof(ret);
	dread(ret, data.data, 2, "data", "available_eth");
	available_coin[2]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_eth");
	in_use_coin[2]=atof(ret);
	dread(ret, data.data, 2, "data", "total_dash");
	coin[3]=atof(ret);
	dread(ret, data.data, 2, "data", "available_dash");
	available_coin[3]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_dash");
	in_use_coin[3]=atof(ret);
	dread(ret, data.data, 2, "data", "total_ltc");
	coin[4]=atof(ret);
	dread(ret, data.data, 2, "data", "available_ltc");
	available_coin[4]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_ltc");
	in_use_coin[4]=atof(ret);
	dread(ret, data.data, 2, "data", "total_etc");
	coin[5]=atof(ret);
	dread(ret, data.data, 2, "data", "available_etc");
	available_coin[5]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_etc");
	in_use_coin[5]=atof(ret);
	dread(ret, data.data, 2, "data", "total_xrp");
	coin[6]=atof(ret);
	dread(ret, data.data, 2, "data", "available_xrp");
	available_coin[6]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_xrp");
	in_use_coin[6]=atof(ret);
	dread(ret, data.data, 2, "data", "total_bch");
	coin[7]=atof(ret);
	dread(ret, data.data, 2, "data", "available_bch");
	available_coin[7]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_bch");
	in_use_coin[7]=atof(ret);
	dread(ret, data.data, 2, "data", "total_xmr");
	coin[8]=atof(ret);
	dread(ret, data.data, 2, "data", "available_xmr");
	available_coin[8]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_xmr");
	in_use_coin[8]=atof(ret);
	dread(ret, data.data, 2, "data", "total_zec");
	coin[9]=atof(ret);
	dread(ret, data.data, 2, "data", "available_zec");
	available_coin[9]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_zec");
	in_use_coin[9]=atof(ret);
	dread(ret, data.data, 2, "data", "total_qtum");
	coin[10]=atof(ret);
	dread(ret, data.data, 2, "data", "available_qtum");
	available_coin[10]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_qtum");
	in_use_coin[10]=atof(ret);
	dread(ret, data.data, 2, "data", "total_btg");
	coin[11]=atof(ret);
	dread(ret, data.data, 2, "data", "available_btg");
	available_coin[11]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_btg");
	in_use_coin[11]=atof(ret);
	dread(ret, data.data, 2, "data", "total_eos");
	coin[12]=atof(ret);
	dread(ret, data.data, 2, "data", "available_eos");
	available_coin[12]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_eos");
	in_use_coin[12]=atof(ret);
	dread(ret, data.data, 2, "data", "total_icx");
	coin[13]=atof(ret);
	dread(ret, data.data, 2, "data", "available_icx");
	available_coin[13]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_icx");
	in_use_coin[13]=atof(ret);
	dread(ret, data.data, 2, "data", "total_ven");
	coin[14]=atof(ret);
	dread(ret, data.data, 2, "data", "available_ven");
	available_coin[14]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_ven");
	in_use_coin[14]=atof(ret);
	dread(ret, data.data, 2, "data", "total_trx");
	coin[15]=atof(ret);
	dread(ret, data.data, 2, "data", "available_trx");
	available_coin[15]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_trx");
	in_use_coin[15]=atof(ret);
	dread(ret, data.data, 2, "data", "total_mith");
	coin[16]=atof(ret);
	dread(ret, data.data, 2, "data", "available_mith");
	available_coin[16]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_mith");
	in_use_coin[16]=atof(ret);
	dread(ret, data.data, 2, "data", "total_mco");
	coin[17]=atof(ret);
	dread(ret, data.data, 2, "data", "available_mco");
	available_coin[17]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_mco");
	in_use_coin[17]=atof(ret);
	dread(ret, data.data, 2, "data", "total_omg");
	coin[18]=atof(ret);
	dread(ret, data.data, 2, "data", "available_omg");
	available_coin[18]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_omg");
	in_use_coin[18]=atof(ret);
	dread(ret, data.data, 2, "data", "total_knc");
	coin[19]=atof(ret);
	dread(ret, data.data, 2, "data", "available_knc");
	available_coin[19]=atof(ret);
	dread(ret, data.data, 2, "data", "in_use_knc");
	in_use_coin[19]=atof(ret);
	free(data.data);

	account->set_total_coin(coin);
	account->set_available_coin(available_coin);
	account->set_in_use_coin(in_use_coin);
}
void _API::perform(struct curl_slist * cslist)
{
	curl_easy_setopt( curl, CURLOPT_URL, url);
	curl_easy_setopt( curl, CURLOPT_HTTPHEADER, cslist );
	curl_easy_perform( curl );
}
void _API::account_record(_Account * account, const int message_type, const char * message)
{

}
void _API::set_d_BaseDir(const char * d_BaseDir)
{
	strcpy(this->d_BaseDir, d_BaseDir);
}
void _API::trade_activation(void)
{
	this->tradevaild = 1;
}
void _API::trade_disabled(void)
{
	this->tradevaild = -1;
}
#endif

/*comfile:
  g++ -lcurl -lcrypto api.h
 */
