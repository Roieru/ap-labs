#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

	if (argc > 1) {
		char *string = argv[1];
		int charCount[256];
		char counting = 1;
		int startingIndex = 0;
		int currentIndex;
		int currentLength;
		int maxLength = 1;

		int i;

		while (startingIndex < strlen(string)) {
			if (starting) {
				for (i = 0; i < 256; i++) {
					charCount[i] = 0;
				}

				currentLength = 0;
				counting = 0;
				currentIndex = startingIndex;
			}

			if (charCount[(int) string[currentIndex]] == 0) {
				charCount[(int) string[currentIndex]] = 1;
				currentIndex++;
				currentLength++;
				if (currentLength > maxLength) {
					maxLength = currentLength;
				}
				if (currentIndex >= strlen(string)) {
					break;
				}
			}
			else {
				counting = 1;
				startingIndex++;
			}
		}

		printf("%d\n",maxLength);

	}
	
	return 0;

}
