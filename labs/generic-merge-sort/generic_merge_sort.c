#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ELEN 128
#define STRSORT 0
#define NUMSORT 1

int sort;
int(*compare)(void *, void *);

int numcompare(char *a, char *b) {
	return (atoi(a)-atoi(b));
}

void merge(char arr[][ELEN], int l, int m, int r) {
	int i,j,k;
	int n1 = m - l + 1;
	int n2 = r - m;

	char L[n1][ELEN], R[n2][ELEN];

	for (i = 0; i < n1; i++)
		strcpy(L[i],arr[l+i]);
	for (j = 0; j < n2; j++)
		strcpy(R[j],arr[m+1+j]);

	i = 0;
	j = 0;
	k = l;

	while (i < n1 && j < n2) {
		if ((*compare)(L[i],R[j]) <= 0) {
			strcpy(arr[k],L[i]);
			i++;
		} else {
			strcpy(arr[k],R[j]);
			j++;
		}
		k++;
	}

	while (i < n1) {
		strcpy(arr[k],L[i]);
		i++;
		k++;
	}

	while (j < n2) {
		strcpy(arr[k],R[j]);
		j++;
		k++;
	}
}

void mergeSort(char arr[][ELEN], int l, int r) {
	if(l < r) {
		int m = l + (r-l)/2;

		mergeSort(arr,l,m);
		mergeSort(arr,m+1,r);
		merge(arr,l,m,r);
	}
}

int main(int argc, char **argv) {

	sort = ((argc > 1 && strcmp(argv[1],"-n")==0) ? NUMSORT : STRSORT);

	printf("%d\n",sort);
	compare = (sort ? (int(*)(void *, void *))(&numcompare) : (int(*)(void *, void *))(&strcmp));

	compare = (int(*)(void *, void *))&strcmp;

	printf("%d with b and a\n",(*compare)("b","a"));

	int n;
	char tempn[ELEN];

	printf("\nEnter the length of the array to be sorted (decimal values will be truncated): ");
	scanf("%s",tempn);
	printf("\n");

	n = atoi(tempn);

	if(n <= 0) {
		printf("Please enter a length of at least 1");
		return 1;
	}

	int i;
	char elements[n][ELEN];

	for(i = 0; i < n; i++){
		printf("Enter element number %d: ",i);
		scanf("%s",tempn);
		if(sort){
			int parsed;
			char *temp;

			parsed = (int)strtol(tempn,&temp,0);

			if(*temp != '\0'){
				printf("Only integers are accepted when sorting numbers\n");
				return 2;
			}
		}

		strcpy(elements[i],tempn);

	}

	printf("\nUnsorted array is {");
	for(i = 0; i < n-1; i++){
		printf("%s, ",elements[i]);
	}
	printf("%s}\n",elements[n-1]);

	mergeSort(elements,0,n-1);

	printf("\nSorted array is {");
	for(i = 0; i < n-1; i++){
		printf("%s, ",elements[i]);
	}
	printf("%s}\n\n",elements[n-1]);

    	return 0;
}
