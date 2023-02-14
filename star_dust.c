//Calugaritoiu Ion-Victor 312CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int **read_map(int n, int *col)
{
	int **map;

	map = (int **)malloc(n * sizeof(int *));	//alocam n linii pentru matrice

	if (!map) {	//verificam daca alocarea s-a efectuat cu succes
		free(map);
		return NULL;
	}

	for (int i = 0; i < n; i++) {
		scanf("%d", &col[i]);

		map[i] = calloc(col[i], sizeof(int *));
		//alocam i coloane in functie de linie
		if (!map[i]) {
			for (int j = 0; j < i; j++)
				free(map[j]);

			free(map);
			return NULL;
		}

		for (int j = 0; j < col[i]; j++)
			scanf("%x", &map[i][j]);
	}

	return map;
}

double average_bytes(int n, int **map, int *col)
{
	double s = 0, nr = 0;

	char *byte = calloc(4, sizeof(char));
	//alocam 4 bytes pentru a memora fiecare byte din elemntul prelucrat
	if (!byte)
		exit(-1);

	for (int j = 0; j < col[0]; j++) {	//shiftam bitii pentru a obtine bytes-ii
		byte[0] = ((int)map[0][j] >> 24) & 0xff;
		byte[1] = ((int)map[0][j] >> 16) & 0xff;
		byte[2] = ((int)map[0][j] >> 8) & 0xff;
		byte[3] = (int)map[0][j] & 0xff;

		for (int i = 0; i < 4; i++)
			s = s + byte[i];

		nr = nr + 4;
		//calculam suma bytes-ilor de pe marginea de sus a matricei
	}

	for (int j = 0; j < col[n - 1]; j++) {
		byte[0] = (map[n - 1][j] >> 24) & 0xff;
		byte[1] = (map[n - 1][j] >> 16) & 0xff;
		byte[2] = (map[n - 1][j] >> 8) & 0xff;
		byte[3] = map[n - 1][j] & 0xff;

		for (int i = 0; i < 4; i++)
			s = s + byte[i];

		nr = nr + 4;
		//calculam suma bytes-ilor de pe marginea de jos a matricei
	}

	for (int i = 1; i < n - 1; i++) {
		byte[1] =  map[i][0] & 0xff;
		s = s + byte[1];
		nr++;
		//adaugam la suma bytes-ii de pe marginea din stanga
		byte[1] =  (map[i][col[i] - 1] >> 24) & 0xff;
		s = s + byte[1];
		nr++;
		//adaugam la suma bytes-ii de pe marginea din dreapta
	}

	free(byte);	//eliberam memoria

	return s / nr;
	}

void modify(int **map, int *col, int type)
{
	int row, index, value;
	char *byte = calloc(4, sizeof(char));
	if (!byte) {
		free(byte);
		exit(-1);
	}
	scanf("%d %d %X", &row, &index, &value);
	if (type == 'I') {
		if (index < col[row]) {
			byte[3] = (value >> 24) & 0xFF;
			byte[2] = (value >> 16) & 0xFF;
			byte[1] = (value >> 8) & 0xFF;
			byte[0] = value & 0xFF;
			map[row][index - 1] = *(int *)byte;
		} else {
			map[row] = (int *)realloc(map[row], (index + 1) * sizeof(int *));
			//in cazul in care pozitia introdusa depaseste nr de coloane,
			//realocam memorie pentru a mari matricea
			for (int i = col[row]; i < index ; i++)
				map[row][i] = 0x00000000;	//zeroizam elementele nou alocate
			map[row][index - 1] = value;	//atribuim noua valoare
			col[row] = index;	//actualizam numarul de coloane
		}
	}
	if (type == 'S') {
		if (2 * col[row] > (index - 1) / 2 + 1) {
			byte[3] = (map[row][(index - 1) / 2] >> 24) & 0xFF;
			byte[2] = (map[row][(index - 1) / 2] >> 16) & 0xFF;
			byte[1] = (map[row][(index - 1) / 2] >> 8) & 0xFF;
			byte[0] = map[row][(index - 1) / 2] & 0xFF;
			if ((index - 1) % 2 == 0) {	//bytes-ii modificati difera
				byte[0] = value & 0xFF;	//in functie de paritatea indexului
				byte[1] = (value >> 8) & 0xFF;
			} else {
				byte[2] = value & 0xFF;
				byte[3] = (value >> 8) & 0xFF;
			}
			map[row][(index - 1) / 2] = *(int *)byte;
		} else {
			int new_length = index / 2;
			//repetam procedeul anterior peentru marirea matricei
			map[row] = (int *)realloc(map[row], new_length * sizeof(int *));
			//numarul de elemente nou adaugate este calculat diferit
			for (int i = col[row]; i < new_length + 1; i++)
				map[row][i] = 0;

			byte[3] = (value >> 24) & 0xFF;
			byte[2] = (value >> 16) & 0xFF;
			byte[1] = (value >> 8) & 0xFF;
			byte[0] = value & 0xFF;
			map[row][new_length] = *(int *)byte;
			col[row] = new_length + 1;
		}
	}
	if (type == 'C') {
		if (4 * (col[row] + 1) >= index + 1) {
			byte[3] = (map[row][(index - 1) / 4] >> 24) & 0xFF;
			byte[2] = (map[row][(index - 1) / 4] >> 16) & 0xFF;
			byte[1] = (map[row][(index - 1) / 4] >> 8) & 0xFF;
			byte[0] = map[row][(index - 1) / 4] & 0xFF;
			//modificam byte-ul calculat astfel:
			byte[(index - 1) % 4] = value & 0xFF;
			map[row][(index - 1) / 4] = *(int *)byte;
		} else {
			int new_length = index / 4;
			//repetam procedeul anterior peentru marirea matricei
			map[row] = (int *)realloc(map[row], new_length * sizeof(int *));
			//numarul de elemente nou adaugate este calculat diferit
			for (int i = col[row]; i < new_length + 1; i++)
				map[row][i] = 0;
			byte[3] = (value >> 24) & 0xFF;
			byte[2] = (value >> 16) & 0xFF;
			byte[1] = (value >> 8) & 0xFF;
			byte[0] = value & 0xFF;
			map[row][new_length] = *(int *)byte;
			col[row] = new_length + 1;
		}
	}
	free(byte);
}

void delete(int **map, int type)
{
	int row, index;
	char *byte = calloc(4, sizeof(char));

	if (!byte) {
		free(byte);
		exit(-1);
	}

	scanf("%d %d", &row, &index);

	if (type == 'I') {	//zeroizam elementul de pe coloana index - 1
		int slot = index - 1;	//deoarece delete porneste de la index = 1
		map[row][slot] = 0x00000000;
	}

	else if (type == 'S') {
		int slot = index / 2;

		byte[3] = (map[row][slot] >> 24) & 0xFF;
		byte[2] = (map[row][slot] >> 16) & 0xFF;
		byte[1] = (map[row][slot] >> 8) & 0xFF;
		byte[0] = map[row][slot] & 0xFF;
		//zeroizam bytes-ii in functie de paritatea indexului
		if (index % 2 != 0) {
			byte[0] = 0x00;
			byte[1] = 0x00;

			map[row][slot] = *(int *)byte;
		} else {
			byte[1] = (map[row][slot - 1] >> 8) & 0xFF;
			byte[0] = map[row][slot - 1] & 0xFF;
			byte[2] = 0x00;
			byte[3] = 0x00;

			map[row][slot - 1] = *(int *)byte;
		}
	}

	else if (type == 'C') {
		int slot = index - 1;

		byte[3] = (map[row][slot / 4] >> 24) & 0xFF;
		byte[2] = (map[row][slot / 4] >> 16) & 0xFF;
		byte[1] = (map[row][slot / 4] >> 8) & 0xFF;
		byte[0] = map[row][slot / 4] & 0xFF;
		//zeroizam byte-ul calculat astfel:
		byte[slot % 4] = 0x00;
		//impartim (index - 1) la 4 pentru a obtine coloana corecta
		map[row][slot / 4] = *(int *)byte;
	}

	free(byte);
}

void swap_bytes(char *byte1, char *byte2)
{
	int aux = *byte1;
	*byte1 = *byte2;
	*byte2 = aux;
}

void swap(int **map, int type)
{
	int row, index;
	char *byte = calloc(4, sizeof(char));

	if (!byte) {
		free(byte);
		exit(-1);
	}

	scanf("%d %d", &row, &index);

	if (type == 'I') {
		byte[3] = (map[row][index] >> 24) & 0xFF;
		byte[2] = (map[row][index] >> 16) & 0xFF;
		byte[1] = (map[row][index] >> 8) & 0xFF;
		byte[0] = map[row][index] & 0xFF;
		//interschimbam toti bytes-ii
		swap_bytes(&byte[0], &byte[3]);
		swap_bytes(&byte[1], &byte[2]);

		map[row][index] = *(int *)byte;
	} else if (type == 'S') {
		int slot = index / 2;

		byte[3] = (map[row][slot] >> 24) & 0xFF;
		byte[2] = (map[row][slot] >> 16) & 0xFF;
		byte[1] = (map[row][slot] >> 8) & 0xFF;
		byte[0] = map[row][slot] & 0xFF;
		//interschimbam bytes-ii calculati astfel:
		if (index % 2 == 0) {
			swap_bytes(&byte[1], &byte[0]);

			map[row][slot] = *(int *)byte;
		} else {
			swap_bytes(&byte[2], &byte[3]);

			map[row][slot] = *(int *)byte;
		}	//pentru tipul char, elementele nu se schimba
	}
	free(byte);
}

void read_action(int **map, int *v, int k)
{
	char action, type;
	//citim k actiuni asupra matricei
	while (k) {
		scanf("\n%c", &action);
		scanf("\n%c", &type);

		if (action == 'M')
			modify(map, v, type);

		if (action == 'D')
			delete(map, type);

		if (action == 'S')
			swap(map, type);
		k--;
	}
}

void print_map(int n, int **map, int *v)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < v[i]; j++)
			printf("%08X ", map[i][j]);

		printf("\n");
	}
}

int main(void)
{
	int **map;
	int *col;
	int n, k;
	double m_a;

	scanf("%d", &n);

	col = malloc(n * sizeof(int));
	//numar de coloane in functie de linie
	map = read_map(n, col);	//citesc matricea originala

	m_a = average_bytes(n, map, col);

	scanf("%d", &k);

	read_action(map, col, k);	//citesc actiunile

	printf("task 1\n");

	printf("%0.8lf\n", m_a);

	printf("task 2\n");

	print_map(n, map, col);	//afisez matricea modificata

	printf("task 3\n");

	free(col);
	//eliberam memoria alocata matricei in totalitate
	for (int i = 0; i < n; i++)
		free(map[i]);

	free(map);

	return 0;
}
