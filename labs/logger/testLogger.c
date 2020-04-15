#include <stdio.h>

int infof(char *format, ...);
int warnf(char *format, ...);
int errorf(char *format, ...);
int panicf(char *format, ...);
int printer(char *format, ...);

int main() {

	infof("INFO: String: %s Exponential floating-point number: %e Floating-point number: %f\n","Hello World!", 3.2e-5f, 3.1415);
	warnf("WARNING: Decimal base 10: %d Integer base 10: %i Octal number: %o\n", 10, 100, 8);
	errorf("ERROR: Percent sign: %% Unsigned decimal number: %u\n", 10u);
	panicf("PANIC: Character: %c Hexadecimal: %x\n", 'A', 16);
    	return 0;
}
