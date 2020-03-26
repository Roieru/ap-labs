#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/* filecopy:  copy file ifp to file ofp */
void filecopy(int ifpd, int ofpd)
{

    	int end = 1;
	char readC[1];

	end = read(ifpd,readC,1);
	//printf("%d\n",ifpd);

	while (end > 0){
		//end = read(ifpd,readC,1);
		//printf("%d\n",end);
		write(ofpd,readC,1);
		end = read(ifpd,readC,1);

	}

}

/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[])
{
    int fp;
    void filecopy(int, int);
    char *prog = argv[0];   /* program name for errors */

    if (argc == 1){  /* no args; copy standard input */
        filecopy(0, 1);
    }else{
        while (--argc > 0){
		//printf("%s\n",argv[v]);
		int fp = open(*++argv,O_RDONLY);
            if (fp < 0) {
                fprintf(stderr, "%s: can′t open %s\n", prog, *argv);
                return 1;
            } else {
		//printf("%d\n",fp);
                filecopy(fp, 1);
                close(fp);
            }

		}
	}

    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        return 2;
    }

    return 0;
}
