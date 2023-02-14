# CLI-Matrix-Editor
Simple CLI matrix editor written in C - supports modification, swapping and
deletion of matrix elements

Calugaritoiu Ion-Victor

Task 1:

- am alocat dinamic matricea hartii (functia read_map), numarul de coloane 
fiind diferit in functie de linie;
- pentru fiecare margine, am shiftat bitii corespunzatori pentru a aduna doar
 bytes-ii de pe margine (functia average_bytes);
- am impartit suma bytes-ilor la numarul acestora (ambele variabile fiind 
double) si am printat rezultatul cu precizia dorita;

Task 2:

- am implementat functia read_action pentru a citi fiecare actiune introdusa;

	MODIFY:
- am verificat in toate cele 3 cazuri (I, S, C) daca indexul introdus 
depaseste numarul de elemente de pe linie;
- daca nu depaseste, fac modificarile pe bytes in functie indexul introdus;
- daca depaseste, realoc linia matricei, astfel obtin mai multe elemente
pe linie, pe care pe zeroizez pana la intalnirea pozitiei dorite;

	DELETE:
- zeroizez bytes-ii potriviti, calculati diferit in fiecare caz;

	SWAP:
- pentru "I", interschimb toti bytes-ii intre ei (functia swap);
- pentru "S", interschimb doar 2 bytes, calculati in functie de index;
- pentru "C", functia nu are niciun efect;

