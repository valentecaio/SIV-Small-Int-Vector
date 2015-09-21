 /* Caio Valente 1313402 3WB */
 /* Ilana Segal 1510329 3WB */


#include "smallint.h"
#include <stdio.h>
#include <math.h>

VetSmallInt vs_new(int val[])
{
	
	int i, n;
	VetSmallInt siv[];
	
	for (n=24; n<28; n++)
	{
		v[n] = 0;
	}
	
	for (i = 0; i<4; i++)
	{
		if (v[i] < 2^5)
			{
				siv[31-i] = 0;
				siv[i] = v[i]
			}
		else
			{
				siv[31-i] = 1;
				//siv[i] = 
			}
	}
	
	return siv;
}
