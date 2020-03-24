#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define REPORT_FILE "packages_report.txt"
#define DATE_LEN 17
#define NAME_LEN 32
#define PACK_SIZE 1024
#define PINFO 64
#define LINE_LEN 512

void analizeLog(char *logFile, char *report);

struct package {
	char name[NAME_LEN];
	char installed[DATE_LEN];
	char upgraded[DATE_LEN];
	int upgrades;
	char removal[DATE_LEN];
};

struct package packages[PACK_SIZE];

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o pacman.log\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    	printf("Generating Report from: [%s] log file\n", logFile);

	int inputFileDesc = open(logFile,O_RDONLY);
	if(inputFileDesc < 0){
		printf("Error! Something went wrong with the log file.\n");
		return;
	}

	int fd = open(report, O_WRONLY|O_TRUNC|O_CREAT,0664);
	if(fd < 0){
		printf("Error! Something went wrong with the output file.\n");
		return;
	}

	char readC[1];
	char line[LINE_LEN];
	int i = 0;
	int end = 1;
	int totalInstalled = 0;
	int totalRemoved = 0;
	int totalUpgrades = 0;
	int currentInstalled = 0;

	while(end != 0){
		end = read(inputFileDesc,readC,1);
		while(readC[0] != '\n'){
			line[i] = readC[0];
			read(inputFileDesc,readC,1);
			i++;
		}
		line[i] = '\0';
		char * pch;
		pch = strtok(line,"[]");
		char *date = pch;
		pch = strtok(NULL," ");
		if(pch!=NULL){
			char *isALPM = pch;
			pch = strtok(NULL," ");
			char *action = pch;
			pch = strtok(NULL," ");
			char *pack = pch;

			if(strcmp(isALPM,"[ALPM]")==0){
				if(strcmp("installed",action)==0){
					struct package np;

					strcpy(np.name,pack);
					strcpy(np.installed,date);
					strcpy(np.upgraded,"-");
					np.upgrades = 0;
					strcpy(np.removal,"-");

					int j;
					int index = -1;
					for(j=0;j<totalInstalled;j++){
						if(strcmp(packages[j].name,pack)==0){
							index = j;
							break;
						}
					}

					if(index<0){
						packages[totalInstalled] = np;
						totalInstalled++;
					} else{
						if(packages[index].upgrades>0){
							totalUpgrades--;
						}
						packages[index] = np;
						totalRemoved--;
					}
					currentInstalled++;

				} else if(strcmp("upgraded",action)==0){
					int j;
					for(j=0;j<totalInstalled;j++){
						if(strcmp(packages[j].name,pack)==0){
							strcpy(packages[j].upgraded,date);
							if(packages[j].upgrades==0){
								totalUpgrades++;
							}
							packages[j].upgrades++;
							break;
						}
					}
				}else if(strcmp("removed",action)==0){
					int j;
					for(j=0;j<totalInstalled;j++){
						if(strcmp(packages[j].name,pack)==0){
							strcpy(packages[j].removal,date);
							break;
						}
					}
					totalRemoved++;
					currentInstalled--;
				}
			}
		}
		i = 0;
	}

	close(inputFileDesc);

	char *title;
	title = "Pacman Packages Report\n----------------------\n";
	write(fd,title,strlen(title));
	char genInfo[NAME_LEN];
	sprintf(genInfo,"- Installed packages\t: %d\n",totalInstalled);
	write(fd,genInfo,strlen(genInfo));
	sprintf(genInfo,"- Removed packages\t: %d\n",totalRemoved);
	write(fd,genInfo,strlen(genInfo));
	sprintf(genInfo,"- Upgraded packages\t: %d\n",totalUpgrades);
	write(fd,genInfo,strlen(genInfo));
	sprintf(genInfo,"- Current installed\t: %d\n",currentInstalled);
	write(fd,genInfo,strlen(genInfo));
	title="\nList of packages\n----------------\n";
	write(fd,title,strlen(title));

	char packInfo[PINFO];
	int j;
	for(j=0;j<totalInstalled;j++){
		struct package cp = packages[j];
		sprintf(packInfo,"- Package Name\t: %s\n",packages[j].name);
		write(fd,packInfo,strlen(packInfo));
		sprintf(packInfo,"  - Install date\t: %s\n",cp.installed);
		write(fd,packInfo,strlen(packInfo));
		sprintf(packInfo,"  - Last update date\t: %s\n",cp.upgraded);
		write(fd,packInfo,strlen(packInfo));
		sprintf(packInfo,"  - How many updates\t: %d\n",cp.upgrades);
		write(fd,packInfo,strlen(packInfo));
		sprintf(packInfo,"  - Removal date\t: %s\n",cp.removal);
		write(fd,packInfo,strlen(packInfo));
	}

	close(fd);

    	printf("Report was generated at: [%s]\n", report);
}
