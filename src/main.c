#include <gint/display.h>
#include <gint/keyboard.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
	int i;
	dclear(C_WHITE);
	dtext(1, 1, C_BLACK, "Sample fxSDK add-in.");
	dupdate();

	srand(1000);
	i = rand() % 20;
	i++;

	float a = 18.3;
	float b = floor(a);
	b+=1.0;

	getkey();
	return 1;
}
