    /*
* Soubor: proj1.c
* Datum: 2012/10/27
* Autor: Radek Pistelak, xpiste04@stud.fit.vutbr.cz
* Projekt: Tvorba histogramu, projekt c.1. pro predmet IZP
* Popis: Program slouzi pro zjisteni poctu cetnosti znaku na std. vstupu a jejich vypisu,
*        dle zadani
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h> 

// vycet stavy programu 
enum states { 
    CHELP,  // program bude pouze tisknout napovedu
    CNORMAL,// program bude pracovat bez parametru 
    CWITHN, // program bude pracovat s parametrem N
};

// vycet chybovych kodu
enum ecode {
    EOK = 0, 
    ECLWRONG,
    EUNKNOWN,
};

// retezce ktere se budou tisknout pri chybovem kodu
const char *EMSG[] = {
    "Vse v poradku!\n", 
    "Chybne parametry!\n",
    "Neznama chyba!\n",
    };

// text napovedy
const char *HELPMSG =
    "\n"
    "Projekt c.1 Tvorba histogramu\n"
    "-----------------------------\n"
    "Autor: Radek Pistelak, xpiste04 2012\n"
    "\n"
    "Funkce\n"
    "./main -h\t\tVytiskne napovedu\n"
    "./main --help\t\tVytiskne napovedu\n"
    "\n"
    "./main\t\t\tVytiskne vsechny relevantni radky histogramu\n"
    "\t\t\tv poradi danem ordinalnimi hodnotami znaku\n"
    "\n"
    "./main N\t\tkde N je ciselny parametr\n"
    "\t\t\tvytiskne prvnich N radku histogramu v poradi danem cetnosti\n"
    "\t\t\tznaku od nejcetnejsiho znaku.\n"
    "\n"
;

// struktura a novy typ TParams pro praci s parametry  
typedef struct {
    int ecode; 
    int state;
    int n; 
} TParams; 


/////////////////////////////////////////////////////////
void printECode(int ecode);

TParams get_params(int argc, char *argv[]);
int load_n (char *str, int *n);

void load_lines(unsigned char array[]);

void print (unsigned char a, int index);
void print_all (unsigned char a[]);
void sort_and_print (unsigned char a[], int sort_lines);
/////////////////////////////////////////////////////////



/** ------------------ PRACE S PARAMETRY ---------------*/

/*
* Funkce vyhodnoti parametry navratovym typem je TParams 
*/

TParams get_params(int argc, char *argv[]) {
    
    TParams result = { .ecode = EOK, .state = CHELP, .n = 0 };

    if (argc == 1) {
        result.state = CNORMAL;
    }
        else if (argc == 2) {
        if ((strcmp("-h", argv[1]) == 0) || 
            (strcmp ("--help", argv[1]) == 0)) { 
            result.state = CHELP;     
        }
        else if (load_n(argv[1], &result.n) == 1) {
            result.state = CWITHN;
        }
        else {
            result.ecode = ECLWRONG;
        }
    }
   else {
        result.ecode = ECLWRONG;
    }

    return result; 
}

/*
* Funkce prevadi retezec na cislo a zaroven kontroluje zda vyhovuje zadani
* 
* Funkce vraci int ale zrovna tak by mohla vracet typ bool.
*
* char *str -> ukazatel na prevadeny retezec
* int *n    -> ukazatel na promenou int do ktere prevedeno cislo ulozime
*/
int load_n (char *str, int *n)
{
   while (*str != '\0')  
   {   // budeme prevadet dokud nebudeme na konci retezce
       if (*str >= '0' && *str <= '9') 
       { // pokud je prevadany znak cislo pokracujeme jinak vratime -1  
           if (*n == 0 && *str == '0') 
           { // nuly na zacatku nebudume ukladat
               ++str; 
               continue;
           }
           *n = *n * 10 + (*str - '0');
           if (*n > UCHAR_MAX)
           {
               return -1; // cislo vetsi nez UCHAR_MAX nema cenu prevadet 
                          // -> nevyhovuje zadani 
           }
           ++str; // posuneme ukazatel na dalsi znak
       }
       else 
           return -1;
   }
   return *n > 0 ? 1 : -1; // pokud jsme se dostali az sem cislo jsme nacetli 
                           // ale jeste musime overit jestli nahodou to neni nula 
                           // -> nevyhovovala by zadani
}


/* ------------ VSTUP --------- */

/*
* Funkce slouzi pro nacteni "radku". 
* Tedy spise poctu zobrazenych znaku...
*
* unsigned char array[] -> predame ji pole do ktereho bude nacitat hodnoty
*/ 
void load_lines(unsigned char array[]) 
{
    int c; // promena typu int pro nacitani znaku -- kvuli EOF

    while ((c = getchar()) != EOF) 
    { // nacitame znaky dokud nebude konec vstupu
        if (array[c] > UCHAR_MAX - 1) 
        { // pokud je pocet znaku vetsi nebo roven UCHAR_MAX -1 nebudeme 
          // ho jiz navysovat... 
            array[c] = UCHAR_MAX;
        }
        else 
            array[c]++;
    }
}

/* ------------ VYSTUP --------- */

/*
* Pomocna funkce pro tisk 
*
* unsigned char a -> reprezentuje pocet zobrazeni znaku na indexu int index
* int index
*/


void print (unsigned char a, int index)
{
    if (a > 0) // pokud je cetnost vetsi nez 0 
    {
        if (a == UCHAR_MAX) 
        { // pokud je cetnnost rovna UCHAR_MAX resp. UCHAR.MAX - 1 
          // vypisujeme misto ni pouze NNN 
                if (isprint(index)) {
                     printf ("%i \'%c\': NNN\n", index, index);
                }
                else {
                     printf ("%i: NNN\n", index);
                }   
        }
        else 
        { // jinak vypisujeme "normalni" vystup
                if (isprint(index)) {
                     printf ("%i \'%c\': %d\n", index, index, a);
                }
                else {
                     printf ("%i: %d\n", index, a);
                }
        }   
    }
}


/*
* Funkce projde cele pole a pro kazdy prvek zavola funkci print
* ktera rozhodne co se vytiskne
*/
void print_all (unsigned char a[])
{
    for (int i = 0; i <= UCHAR_MAX; i++)
        print (a[i], i);
}

/*
* Funkce ktera vyhleda hleda dokola maximum a pro nej vola print
* ktera ho vytiskne
*
* unsigned char a[] -> pole ktere se ma tisknout
* int sort_lines    -> pocet radku ktere se budou tisknout/radit
*/

void sort_and_print (unsigned char a[], int sort_lines)
{
    for (int i = 0; i < sort_lines; i++)
    {
        int max = 0;
        for (int j = 0; j <= UCHAR_MAX; j++) { 
            if (a[max] < a[j]) {
                max = j; // do max priradime index prvku pole na ktere se
                         // nachazi nejvysi hodnota
            }
        }    
        print (a[max], max); 
        a[max] = 0; // prvek o indexu max vyunulujeme at ut uz se nam neplete
                    // do vysledku
    }
}

/*
* Funkce slouzi pro vypis na std. err, dle zadaneho chyboveho kodu
*/

void printECode(int ecode)
{
  if (ecode < EOK || ecode > EUNKNOWN) { 
      ecode = EUNKNOWN; 
  }
  fprintf(stderr, "%s", EMSG[ecode]);
}

/*
*  Hlavni cast programu, volajici ostatni podprogramy
*/
int main (int argc, char *argv[]) 
{   

    TParams params = get_params(argc, argv);

    if (params.ecode != EOK)
    {
        printECode(ECLWRONG);
        return EXIT_FAILURE;
    }

    if (params.state == CHELP) {
        printf ("%s", HELPMSG);
        return EXIT_SUCCESS;
    }

    unsigned char array[256] = {0};
    
    load_lines(array);

    switch (params.state) {
        case CWITHN:
            sort_and_print (array, params.n);
            break;
        case CNORMAL:
            print_all(array);
            break;
    }
    
    return EXIT_SUCCESS; 
} 


