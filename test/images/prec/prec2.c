#include <armsdk/syscalls.h>
#include <armsdk/string.h>

#define PRINT_INT_ARRAY(a,len,i) \
	do {\
		for (i = 0; i < len - 1; i++) {\
			writei (stderr, a[i]);\
            writes (stderr, " ");\
        }\
		writei (stderr, a[i]);\
	} while (0)

int faktorial (int i) {
	int ret = 1;
	while (i > 0) {
		ret *= i;
		i--;
	}
	return ret;
}

int permutace_na_poradi (int * permutace, int len)
{
	int i, j, compared;
	int poradi = 0;
	int faktor = faktorial (len);
	for (i = 0; i < len; i++) {
		faktor /= (len - i);
		compared = permutace[i];
		for (j = 0; j < i; j++) {
			if (permutace[i] > permutace[j])
				compared--;
		}
		compared--;
		poradi += compared * faktor;
	}
	poradi++;
	return poradi;
}

int poradi_na_permutace (int poradi, int * permutace, int len)
{
	int current;
	int i, j;
	/* visited[i] == 1, jestli uz se vyskytlo cislo (i+1) v permutaci */
	static int * visited = (int *)0x80000000;
	int faktor = faktorial (len);
    for (i = 0; i < len; ++i)
        visited[i] = 0;

	if (poradi > faktor)/* permutace delsi nez pozadovana delka */
		return 0;
	for (i = 0; i < len; i++) {
		faktor /= (len - i);
		current = ((poradi - 1) / faktor) + 1;
		poradi = ((poradi - 1) % faktor) + 1;
		permutace[i] = current;
		/**
		 * Najdeme (current)-te dosud nepouzite cislo
		 */
		for (j = 0;; j++) {
			if (visited[j] == 0)
				current--;
			if (current == 0)
				break;
		}
		j++; /* protoze cislujeme od jednicky, ale visited je cislovane od 0 (0 znamena 1, 1 znamena 2 atd.) */
		permutace[i] = j;
		visited[j-1] = 1;/* (j-1) - viz poznamka u deklarace visited nahore */
	}
	return 1;
}

int main (int argc, char *argv[]) {
	int poradi, poradi_zpetne;
	int i;

	int * permutace;

	int digits = 0;
	int power_of_ten = 1;

	int permutace_len;
	int permutace_count;
	char * tail;

	if (argc == 1)
		permutace_len = 9;
	else if (argc == 2)
		permutace_len = (int) strtou32 (argv[1]);
	else {
		writes (stderr, "Pouziti: prec <delka permutace>\n");
        exit();
		return 1;
	}

	permutace_count = faktorial (permutace_len);

	/**
	 * Spocteme pocet cislic v desitkovem zapisu poctu permutaci,
	 * abychom mohli zarovnat vypis
	 */
	//while (permutace_count > power_of_ten) {
	//	power_of_ten *= 10;
	//	digits++;
	//}

	/**
	 * Test konzistence indexovani permutaci hrubou silou. 
	 *
	 * Projdeme vsechna poradi, dekodujeme permutaci, vytiskneme permutaci,
	 * permutaci zpet zakodujeme na poradi a porovname toto poradi
	 * s puvodnim. Jestli se lisi, tiskneme na konci radku "!!!FAIL!!!",
	 * jestli ne, tak "OK".
	 */
    permutace = (int *) 0x90000000;
	for (poradi = 1; poradi <= permutace_count; poradi++) {
		writes (stderr, "Permutace ");
        writei (stderr, poradi);
		writes (stderr, ": ");

  	    poradi_na_permutace (poradi, permutace, permutace_len);
		PRINT_INT_ARRAY (permutace, permutace_len, i);

		poradi_zpetne = permutace_na_poradi (permutace, permutace_len);
		if (poradi_zpetne == poradi)
			writes (stderr, " OK");
		else {
			writes (stderr, " FAILED, permutace_na_poradi vratila ");
            writei (stderr, poradi_zpetne);
        }
		writes (stderr, "\n");
	}
	
    exit();
	return 0;
}

