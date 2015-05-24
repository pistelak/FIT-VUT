/*
 *    Autor:  Radek Pistelak, xpiste04@stud.fit.vutbr.cz
 *    Rocnik  2012/2013
 *    Popis:  Program pomoci iteracnich vypoctu pocita zvolenou funkci
 *            viz. napoveda a dokumentace...
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include <math.h>

#include "proj2_arctg.h"
#include "proj2_powxa.h"
#include "proj2_argsinh.h"

#define SIGDIG_MAX 11 
#define SIGDIG_MIN 1

// stavove kody programu
enum states {
    CHELP,
    CPOWXA,
    CARCTG,
    CARGSINH,
};


// kody programu
enum ecode {
    EOK = 0,
    ECLWRONG,
    EUNKNOWN,
};


// chybova hlaseni odpovidajici chybovym kodum
const char *EMSG[] = {
    "Vse v poradku!\n",
    "Chybne parametry!\n",
    "Neznama chyba!\n",
};


// text napovedy
const char *HELPMSG = {
    "\n"
    "Projekt c.2 - Iteracni vypocty\n"
    "------------------------------\n"
    "Autor: Radek Pistelak, xpiste04\n"
    "\n"
    "Volby:\n"
    "------\n"
    "./proj2 -h\t\t\tnapoveda\n"
    "./proj2 --powxa sigdig a\n"
    "./proj2 --arctg sigdig\n"
    "./proj2 --argsinh sigdig\n"
    "\n"
    "--powxa sigdig a\t\tspocitani mocninne funkce s exponentem a\n"
    "\t\t\t\to presnosti sigdig\n"
    "--arctg sigdig\t\t\tspocitani argus tangentu o presnosti sigdig\n"
    "--argsinh sigdig\t\tspocitani argumentu hyperbolickeho sinu\n"
    "\t\t\t\to presnosti sigdig\n"
    "\n"
    "Kde a je libolne realne cislo a sigdig je kladne cele cislo v intervalu 1-10.\n"
    "    -                           ------\n"
    "\n"
};

void printECode(int ecode)
{
    if (ecode < EOK || ecode > EUNKNOWN)
    { ecode = EUNKNOWN; }
    fprintf(stderr, "%s", EMSG[ecode]);
}

typedef struct {
    double sigdig; //presnost
    double a;      //exponent
    int ecode;
    int state;
} TParams;

bool load_a (char *str, double *a);
bool load_sigdig (char *str, double *sigdig);

TParams get_params(int argc, char *argv[]) {
    
    TParams result = {
        .ecode = EOK,
        .state = CHELP,
        .sigdig = 0,
        .a = 0,
    };
    
    if (argc == 2 && (strcmp("-h", argv[1]) == 0))
    {
        result.state = CHELP;
    }
    else if (argc == 3)
    {
        if (strcmp("--arctg", argv[1]) == 0) {
            result.state = CARCTG;
        }
        else if (strcmp("--argsinh", argv[1]) == 0) {
            result.state = CARGSINH;
        }
    }
    else if (argc == 4 && (strcmp("--powxa", argv[1]) == 0))
    {
        result.state = CPOWXA;
    }
    else
        result.ecode = ECLWRONG;
    
    
    if (argc == 3 || argc == 4)
    { 
        if ((load_sigdig (argv[2], &result.sigdig)) == false)
        {
            result.ecode = ECLWRONG;
            return result;
        }
    }
    if (argc == 4)
    {
        if ((load_a(argv[3], &result.a)) == false)
        {
            result.ecode = ECLWRONG;
            return result;
            
        }
    }
    
    return result;
}


/**
*
* Fce pro prevod retezce na cislo... 
*
*/ 
bool load_a (char *str, double *a)
{
	char *ptr = NULL; 

	*a = strtod (str, &ptr);

	if (*ptr != '\0')
		return false; 

	return true;
}

/**
*
* Fce pro prevod retezce na cislo... 
*
* Rovnou kontroluje horni a spodni hranici sigdig.
*
*/ 
bool load_sigdig (char *str, double *sigdig)
{
    
    while (*str != '\0') {
        if (*str >= '0' && *str <= '9')
        {
            *sigdig = *sigdig * 10 + (*str - '0');
            if (*sigdig > SIGDIG_MAX)
            {
                return false;
            }
            ++str;
        }
        else
            return false;
    }
    
    return *sigdig < SIGDIG_MIN ? false : true;
}


/**
*
* Fce pro cisla na tvar 0.1^sigdig 
*
*/ 

double sigdig_to_eps(double sigdig)
{
	double eps = 0.1;

	for (int i = 0; i < sigdig; i++) {
		eps *= 0.1; 
	}
	return eps; 
}


/**
* Hlavni fce ktera rozhodne kterou fci budeme volat...
* Prve zavolame fci, ktera nacte parametry do struktury TParams.
* A pote se bud vypise napoveda nebo se zacne nacitat ze vstupu. 
*/

int main (int argc, char *argv[])
{
    
    TParams params = get_params(argc, argv);
    
    if (params.ecode != EOK)
    {
        printf("Spatne zadane parametry\n");
        return EXIT_FAILURE;
    }
    
    if (params.state == CHELP)
    {
        printf ("%s", HELPMSG);
        return EXIT_SUCCESS;
    }
    
	double eps;
	double vysledek;
	double x;

	int status;

	eps = sigdig_to_eps (params.sigdig);
	
	while ((status = fscanf (stdin, "%lf", &x)) != EOF) 
	{  // dokud neni konec vstupu budu nacitat dalsi x 
		if (status != 1)
		{ // pokud se nepovede nacist cislo je tam chyba 
          // kterou precteme, ale neulozime "%*s" 
			fscanf (stdin, "%*s");
			x = NAN;
			printf ("%.10e\n", x);
			continue;
		}
		switch (params.state) 
		{ // podle parametru zvolime co se bude pocitat a danou fci zavolame 
			case CARCTG:
				vysledek = my_arctg (x, eps);
				printf ("%.10e\n", vysledek);
				break;
			case CPOWXA:
				vysledek = my_powxa (x, params.a, eps);
				printf ("%.10e\n", vysledek);
				break;
			case CARGSINH:
				vysledek = my_argsinh (x, eps);
				printf ("%.10e\n", vysledek);
				break;
		}
	}
	
    return EXIT_SUCCESS; 
} 

