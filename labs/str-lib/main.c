#include <stdio.h>

int main(int argc, char **argv) {

	int mystrlen(char *str);
	char *mystradd(char *origin, char *addition);
	int mystrfind(char *origin, char *substr);

	if(argc < 4){
		printf("Usage: ./main initialString additionalString substring\n");
		return 1;
	} else{
		int len = mystrlen(argv[1]);
		char *res = mystradd(argv[1],argv[2]);
		int sub = mystrfind(res,argv[3]);

		printf("Initial Length\t\t: %d\n",len);
		printf("New String\t\t: %s\n",res);
		if(mystrfind(res,argv[3]) == 0){
			printf("SubString was found\t: yes\n");
		} else{
			printf("SubString was found\t: no\n");
		}
	}

	return 0;
}
