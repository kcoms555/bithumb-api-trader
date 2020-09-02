#ifndef __INITOR__
#define __INITOR__
#ifndef fp
#define fp stdout
#endif
#include "brainhandler.h"
#include "dealhandler.h"
#include <cstdlib>
#include <cstdio>

int initor(const char * const address, _Brainhandler * brainhandler, _Dealhandler * dealhandler){
	FILE * file = fopen(address, "r");

	if(fp){
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		char str[] = ": \n";
		char * buffer = (char *)malloc(sizeof(char)*size + 1);
		memset(buffer, 0, size + 1);

		fseek(file, 0, SEEK_SET);
		fread(buffer, sizeof(char), size, file);

		char * token;
		int o1;
		double o2;
		int d1;
		double d[3];

		token = strtok(buffer, str);
		while(1){
			if(!strcmp(token, "brain")){
				o1 = atoi(strtok(NULL, str));
				o2 = atof(strtok(NULL, str));
				brainhandler->init_brain(o1, o2);
				fprintf(fp, "initor : brain init_brain(%d, %f)\n", o1, o2);
			}
			if(!strcmp(token, "deal")){
				d1 = atoi(strtok(NULL, str));
				d[0] = atof(strtok(NULL, str));
				d[1] = atof(strtok(NULL, str));
				d[2] = atof(strtok(NULL, str));
				dealhandler->init_deal(d1, d[0],d[1],d[2]);
				fprintf(fp, "initor : deal init_deal(%d, %f, %f, %f)\n", d1, d[0], d[1], d[2]);
			}
			token = strtok(NULL, str);
			if(!token) break;
		}
		free(buffer);
		fclose(file);
	}
	else{
		fprintf(fp, "There is no initalizer file!\n");
	}

	return 0;
}
#endif
