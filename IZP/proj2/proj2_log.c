/*
 *    Autor:  Radek Pistelak, xpiste04@stud.fit.vutbr.cz
 *    Rocnik: 2012/2013
 *    Soubor: proj2_log.c // obsahuje fce pro vypocet logaritmu. 
 *    Popis:  Program pomoci iteracnich vypoctu pocita zvolenou funkci
 *            viz. napoveda a dokumentace...
 */


#include <math.h>

#define LN_2 0.693147180559945309417
#define E_DIGIT 2.718281828459045235360


/** 
* Taylorova rada pro vypocet ln (x)
*
* Fce vraci typ. 
* Jako parametry potrebuje 
* x ---> vstupni hodnota
* eps -> pozadovana presnost 
*/

double my_log_series (double x, double eps)
{
	double sum; 
	double x_old;
	double x_new;
	int k;
	
	sum = x-1;
	k = 1;
	x_old = x_new = x-1;

	while (fabs(x_new) > eps)
	{ 
		k += 1; 
		x_old *= -(x-1);
		x_new = x_old / k;
		sum += x_new;
	}
    
	return sum;
}


/** 
* Fce slouzi ke zuzeni def. oboru, tak aby byl vhodny 
* pro mou zvolenou nekonecnou radu. 
*
* Vraci double. 
* Jako parametry potrebuje vstupni hodnotu x, a prensnost eps.
*
*/

double my_log (double x, double eps)
{
	if (x < 0.0)	
		return NAN;
	if (x == 0.0) 
		return -INFINITY;
	if (x == 1.0)
		return 0;
	if (isinf(x))
		return INFINITY;
	if (x > 1.0 && x < 1.2) {
		x /= E_DIGIT;
		eps /= 100;  // zvyseni presnoti kvuli deleni cisla x 
		return 1 + my_log_series(x, eps);	 
	}
	int exp; 
	
	x = frexp (x, &exp); 
	x = my_log_series (x, eps);

	return x + exp * LN_2;
}
