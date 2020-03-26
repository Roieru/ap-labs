#include <stdio.h>
#include <stdlib.h>

int mystrlen(char *str){
	char a[1];
	int v = 0;
	a[0] = str[0];
	while(a[0] != '\0'){
		v++;
		a[0] = str[v];
	}
	return v;
}

char *mystradd(char *origin, char *addition){
    	char *a;
	a = malloc(mystrlen(origin)+mystrlen(addition)+1);
	char b;
	int v = 0;
	int w = 0;
	b = origin[0];
	while(b != '\0'){
		a[v]=b;
		v++;
		b = origin[v];
	}
	b = addition[0];
	while(b != '\0'){
		a[v] = b;
		w++;
		v++;
		b = addition[w];
	}
	a[mystrlen(origin)] = *addition;

	return a;
}

int mystrfind(char *origin, char *substr){
	int i;
	int j = 0;
	int ret = 1;
	for(i=0;i<=mystrlen(origin);i++){
		char a = origin[i];
		char b = substr[j];
		if(b=='\0'){
			ret = 0;
			break;
		} else if(a == b){
			j++;
		} else{
			j = 0;
		}
	}
    	return ret;
}
