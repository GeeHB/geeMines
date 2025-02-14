#include "shared/casioCalcs.h"
#include "consts.h"

#ifdef DEST_CASIO_CALC
#include "shared/keys.h"
#include <math.h>
#endif // #ifdef DEST_CASIO_CALC

int main(void)
{
#ifdef DEST_CASIO_CALC
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
#endif // #ifdef DEST_CASIO_CALC
	return 1;
}
