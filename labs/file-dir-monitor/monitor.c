#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>
#include "logger.h"
#include <sys/inotify.h>

#define BUFFER_LEN sizeof(struct inotify_event) * 1024

int inotifyFd, wd;
char *root;

int add_subd(const char *filename, const struct stat *statptr, int fileflags, struct FTW *pfwt){
	if(fileflags == FTW_D){
		wd = inotify_add_watch(inotifyFd, filename, IN_ALL_EVENTS);
		if(wd == -1){
			errorf("Could not add to inotify instance\n");
			return 2;
		}
		infof("%s was added to inotify instance\n",filename);
	}

	return 0;

}

int init_root(){
	infof("\n\n");
	inotifyFd = inotify_init();
	if(inotifyFd == -1){
		errorf("Could not initialize inotify instance\n");
		return 1;
	}

	wd = inotify_add_watch(inotifyFd, root, IN_MODIFY | IN_CREATE | IN_DELETE);
	if(wd == -1){
		errorf("Could not add to inotify instance\n");
		return 2;
	}

	nftw(root, add_subd, 5, 0);

	infof("\n\n");
}

int displayInotifyEvent(struct inotify_event *i){
	if(i->mask & IN_MODIFY)		infof("%s was modified\n", i->name);
	if(i->mask & IN_CREATE){
		close(inotifyFd);
		init_root();
		infof("%s was created\n",i->name);
		infof("Inotify instance was updated\n");
	}
	if(i->mask & IN_DELETE){
		close(inotifyFd);
		init_root();
		infof("%s was deleted\n",i->name);
		infof("Inotify instance was updated\n");
	}
}

int main(int argc, char** argv){

	initLogger("stdout");

	char* buf = (char*)malloc(BUFFER_LEN);
	ssize_t numRead;
	char *p;
	struct inotify_event *event;

	if(argc < 2){
		errorf("Error. Correct usage: ./monitor <path>");
		return -1;
	}

	root = argv[1];

	init_root();

	for(;;){
		numRead = read(inotifyFd, buf, BUFFER_LEN);
		if(numRead == 0){
			panicf("Could not read from inotify instance\n");
			return 3;
		}
		if(numRead == 1){
			errorf("Could not read from inotify instance\n");
			return 4;
		}

		for(p = buf; p < buf + numRead;){
			event = (struct inotify_event *)p;
			displayInotifyEvent(event);

			p += sizeof(struct inotify_event) + event->len;
		}

	}

	return 0;
}
