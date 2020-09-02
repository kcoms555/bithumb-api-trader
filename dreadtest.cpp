#include "dread.h"
#include <cstdlib>
#include <cstdio>

int main(void){
	FILE * fp = fopen("init_member", "r");

	if(fp){
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		char str[] = ": \n";
		char * buffer = (char *)malloc(sizeof(char)*size + 1);
		memset(buffer, 0, size + 1);

		fseek(fp, 0, SEEK_SET);
		fread(buffer, sizeof(char), size, fp);
		printf("%s\n", buffer);

		char * token;
		int o1;
		double o2;

		double d[3];

		token = strtok(buffer, str);
		while(1){
			if(!strcmp(token, "brain")){
				 o1 = atoi(strtok(NULL, str));
				 o2 = atof(strtok(NULL, str));
				 printf("READ : brain, %d %f\n", o1, o2);
			}
			if(!strcmp(token, "deal")){
				 d[0] = atof(strtok(NULL, str));
				 d[1] = atof(strtok(NULL, str));
				 d[2] = atof(strtok(NULL, str));
				 printf("READ : deal, %f %f %f\n", d[0], d[1], d[2]); 
			}
			token = strtok(NULL, str);
			if(!token) break;
		}

		free(buffer);
		fclose(fp);
	}

	return 0;
}
