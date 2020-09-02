#ifndef __FP__ 
#define fp stdout 
#endif 

#ifndef __API__ 
#define __API__ 
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <curl/curl.h>
#include <sys/time.h>
#include <limits.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include "account.h"
#include "enum.h"
#include "dread.h"

#define _API_HOST "https://api.bithumb.com"
#define clientTypeBuffer "api-client-type:1"

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
	struct dataset data;
	char ret[128];
	char parameter[256];
	char name[20];
public:
	_API();
	~_API();
	static const char base64_table[];
	void show_krw_deposit(_Account * account);
	void show_btc_address(_Account * account);
	void request(_Account * account, char * endpoint, char * post_data);
	void show_transactions(_Account * account);
	void get_account_coin_from_server(_Account * account);
	void perform(struct curl_slist * cslist);
	void init_coinvalue(const int * coinvalue);
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
void _API::request(_Account * account, char * endpoint, char * post_data)
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
void _API::show_transactions(_Account * account)
{
	data.size=0;
	data.data=(char*)malloc(1);
	request(account, (char *)"/info/user_transactions", (char *)"searchGb=0&currency=BTC");
	fprintf(fp, "%s\n", data.data);
	free(data.data);
}
void _API::perform(struct curl_slist * cslist)
{
	curl_easy_setopt( curl, CURLOPT_URL, url);
	curl_easy_setopt( curl, CURLOPT_HTTPHEADER, cslist );
	curl_easy_perform( curl );
}
#endif

/*comfile:
g++ -lcurl -lcrypto api.h
*/
