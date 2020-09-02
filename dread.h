#ifndef __DREAD__
#define __DREAD__
#include <cstring>
#include <cstdarg>
#include <cstdlib>
#include <cstdio>

int dread(char ret[], const char * const data, int n, ...){
	char * rdata = (char *)malloc(sizeof(char)*strlen(data)+1);
	char * dataout = rdata;
	const char str1[] = ":,\"";
	char * token;
	strcpy(rdata, data);
	int i;

	va_list ap;
	va_start(ap, n);
	for(i=0;i<n;i++){
		if((dataout=strstr(dataout, va_arg(ap,char*)))==NULL){
			free(rdata);
			va_end(ap);
			return -1;
		}
	}
	va_end(ap);

	token = strtok(dataout,str1);
	token = strtok(NULL,str1);

	if(token==NULL){free(rdata); return -1;}
	strcpy(ret,token);

	free(rdata);
	return 0;
}
#endif
