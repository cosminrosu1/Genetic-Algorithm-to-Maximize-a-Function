#define _USE_MATH_DEFINES

#include<iostream>
#include<cmath>

using namespace std;

float evaluareFitness(int v[][50], int poz, float x1, float x2, int nr_biti_x1, int nr_biti_x2, float interval_stanga_x1, float interval_stanga_x2) {
	//transformare D1 din binar in decimal
	int decimal_D1 = 0;
	int putere = 0;
	for (int i = nr_biti_x1 - 1; i >= 0; i--) {
		if (v[poz][i] == 1) {
			decimal_D1 = decimal_D1 + pow(2, putere);
		}
		putere++;
	}

	//transformare D2 din binar in decimal
	int decimal_D2 = 0;
	putere = 0;
	for (int i = nr_biti_x1 + nr_biti_x2 - 1; i >= nr_biti_x1; i--) {
		if (v[poz][i] == 1) {
			decimal_D2 = decimal_D2 + pow(2, putere);
		}
		putere++;
	}

	x1 = interval_stanga_x1 + (decimal_D1 * (x1 / (pow(2, nr_biti_x1) - 1)));
	x2 = interval_stanga_x2 + (decimal_D2 * (x2 / (pow(2, nr_biti_x2) - 1)));

	float function;
	function = 21.5 + x1 * sin(4 * M_PI * x1) + x2 * sin(20 * M_PI * x2);

	return function;
}

void fitness_probabilitati(int n, float eval[], float fitness_total, float prob_selectie[], float prob_cumulate[]) {
	cout << "Evaluarea fitness-ului:" << endl;
	for (int i = 0; i < n; i++) {
		cout << "eval(v" << i + 1 << ") = " << eval[i] << endl;
	}

	for (int i = 0; i < n; i++) {
		fitness_total = fitness_total + eval[i];
	}

	cout << endl << "Fitness total: " << fitness_total << endl;

	for (int i = 0; i < n; i++) {
		prob_selectie[i] = eval[i] / fitness_total;
	}

	cout << endl << "Probabilitatile de selectie:" << endl;
	for (int i = 0; i < n; i++) {
		cout << "p" << i + 1 << " = " << prob_selectie[i] << endl;
	}

	prob_cumulate[0] = 0;
	for (int i = 1; i < n + 1; i++) {
		prob_cumulate[i] = prob_cumulate[i - 1] + prob_selectie[i - 1];
	}

	cout << endl << "Probabilitatile cumulate:" << endl;
	for (int i = 1; i < n + 1; i++) {
		cout << "q" << i << " = " << prob_cumulate[i] << endl;
	}
}

void ruleta(int v[][50], int n, int l, float prob_cumulate[], int v_prim[][50]) {
	//generare numere random si selectare de cromozomi
	float random;
	int rezultate_ruleta[20];

	cout << endl << "Numere generate random cuprinse intre intervalul [0,1] pentru metoda ruletei:";

	for (int i = 0; i < n; i++) {
		random = rand() % 100;
		random = random / 100;
		cout << endl << "a = " << random;
		for (int j = 0; j < n; j++) {
			if (random >= prob_cumulate[j] && random <= prob_cumulate[j + 1]) {
				cout << " -> v" << i + 1 << "' = " << "v" << j + 1;
				rezultate_ruleta[i] = j + 1;
			}
		}
	}

	cout << endl;

	//formarea noii populatii
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < l; j++) {
			v_prim[i][j] = v[rezultate_ruleta[i] - 1][j];
		}
	}

	cout << endl << "Noua populatie:" << endl;

	for (int i = 0; i < n; i++) {
		cout << "v" << i + 1 << "': ";
		for (int j = 0; j < l; j++) {
			cout << v_prim[i][j];
		}
		cout << endl;
	}
}

void incrucisare(int v_prim[][50], int n, int l, float pc) {
	cout << endl << "Probabilitatea de incrucisare: " << pc << endl << endl;

	//generare numere random si selectare perechi de indivizi
	float random[50];
	int indivizi_selectati[50];
	int num = 0;

	cout << "Numere generate random cuprinse intre intervalul [0,1] pentru incrucisare:" << endl;

	for (int i = 0; i < n; i++) {
		random[i] = rand() % 101;
		random[i] = random[i] / 100;
		cout << "a = " << random[i];
		if (random[i] < pc) {
			cout << " -> Este selectat individul v" << i + 1 << "'" << endl;
			indivizi_selectati[num] = i;
			num++;
		}
		else {
			cout << endl;
		}
	}

	if (num % 2 == 1) {
		num--;
	}

	int k = 0;
	int pos;
	int ok;
	int aux;

	//incrucisare intre indivizii perechilor
	for (int i = 0; i < num / 2; i++) {
		ok = 0;
		pos = rand() % (l - 1) + 1;

		cout << endl << "Incrucisare pentru perechea " << indivizi_selectati[k] + 1 << ", " << indivizi_selectati[k + 1] + 1 << endl;

		cout << "pos = " << pos << endl;

		cout << "v" << indivizi_selectati[k] + 1 << "': ";

		for (int j = 0; j < l; j++) {
			cout << v_prim[indivizi_selectati[k]][j];
			if (j == pos - 1) {
				cout << "|";
			}
		}

		cout << endl << "v" << indivizi_selectati[k + 1] + 1 << "': ";

		for (int j = 0; j < l; j++) {
			cout << v_prim[indivizi_selectati[k + 1]][j];
			if (j == pos - 1) {
				cout << "|";
			}
		}

		for (int v = 0; v < l; v++) {
			if (v == pos) {
				ok = 1;
			}
			if (ok == 1) {
				aux = v_prim[indivizi_selectati[k]][v];
				v_prim[indivizi_selectati[k]][v] = v_prim[indivizi_selectati[k + 1]][v];
				v_prim[indivizi_selectati[k + 1]][v] = aux;

			}
		}

		cout << endl << "Descendentii rezultati:" << endl;

		cout << "v" << indivizi_selectati[k] + 1 << "'': ";

		for (int v = 0; v < l; v++) {
			cout << v_prim[indivizi_selectati[k]][v];
		}

		cout << endl << "v" << indivizi_selectati[k + 1] + 1 << "'': ";

		for (int v = 0; v < l; v++) {
			cout << v_prim[indivizi_selectati[k + 1]][v];
		}

		cout << endl;

		k = k + 2;
	}

	cout << endl << "Populatia curenta este: ";
	for (int i = 0; i < n; i++) {
		cout << "v" << i + 1 << "'";
		for (int j = 0; j < num; j++) {
			if (indivizi_selectati[j] == i) {
				cout << "'";
			}
		}
		if (i != n - 1) {
			cout << ", ";
		}
	}
}

void mutatie(int v_prim[][50], int n, int l, float pm) {
	cout << endl << endl << "Probabilitatea de mutatie: " << pm << endl << endl;

	float random;
	int nr_individ, nr_bit;
	//generare numere random si mutatie
	for (int i = 0; i < n * l; i++) {
		random = rand() % 1001;
		random = random / 1000;
		if (random < pm) {
			cout << "Numar random: " << random << endl;
			cout << "Pozitia bit-ului: " << i + 1 << endl;
			nr_individ = ceil(float((i + 1)) / float(l));
			cout << "Numar individ: " << nr_individ << endl;
			nr_bit = (i + 1) - ((nr_individ - 1) * l);
			cout << "Numar bit in individ: " << nr_bit << endl;

			if (v_prim[nr_individ - 1][nr_bit - 1] == 1) {
				v_prim[nr_individ - 1][nr_bit - 1] = 0;
			}
			else {
				v_prim[nr_individ - 1][nr_bit - 1] = 1;
			}

			for (int j = 0; j < l; j++) {
				if (j == nr_bit - 1) {
					cout << "|" << v_prim[nr_individ - 1][j] << "|";
				}
				else {
					cout << v_prim[nr_individ - 1][j];
				}
			}
			cout << endl << endl;
		}
	}

	cout << "Populatia finala rezultata: ";
	for (int i = 0; i < n; i++) {
		cout << endl << "v" << i + 1 << ": ";
		for (int j = 0; j < l; j++) {
			cout << v_prim[i][j];
		}
	}
}

void main() {
	cout << "Functia este: f(x1,x2) = 21.5 + x1 * sin(4 * M_PI * x1) + x2 * sin(20 * M_PI * x2), unde:" << endl;

	float interval_stanga_x1 = -3;
	float interval_dreapta_x1 = 12.1;
	float interval_stanga_x2 = 4.1;
	float interval_dreapta_x2 = 5.8;

	cout << endl << interval_stanga_x1 << " <= x1 <= " << interval_dreapta_x1 << endl;
	cout << interval_stanga_x2 << " <= x2 <= " << interval_dreapta_x2 << endl;

	int precizia = 4;
	float x1 = interval_dreapta_x1 - interval_stanga_x1;
	float x2 = interval_dreapta_x2 - interval_stanga_x2;

	cout << endl << "Domeniul lui x1: " << x1 << endl;
	cout << "Domeniul lui x2: " << x2 << endl;

	float D1 = x1 * pow(10, precizia);
	float D2 = x2 * pow(10, precizia);

	cout << endl << "Subintervale in care se divide D1: " << D1 << endl;
	cout << "Subintervale in care se divide D2: " << D2 << endl;

	int k = 0;
	int nr_biti_x1 = 0;
	while (D1 >= pow(2, k)) {
		k++;
	}
	nr_biti_x1 = k;

	cout << endl << "2^" << k - 1 << " < " << D1 << " < " << "2^" << k << endl;

	k = 0;
	int nr_biti_x2 = 0;
	while (D2 >= pow(2, k)) {
		k++;
	}
	nr_biti_x2 = k;

	cout << "2^" << k - 1 << " < " << D2 << " < " << "2^" << k << endl;

	cout << endl << "Numarul de biti pentru x1: " << nr_biti_x1 << endl;
	cout << "Numarul de biti pentru x2: " << nr_biti_x2 << endl;

	int n = 20;
	int l = nr_biti_x1 + nr_biti_x2;

	cout << endl << "Numarul de cromozomi: " << n;
	cout << endl << "Lungimea cromozomului: " << l << endl;

	srand(time(NULL));

	int v[50][50];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < l; j++) {
			v[i][j] = rand() % 2;
		}
	}

	float eval[50], fitness_total = 0, prob_selectie[50], prob_cumulate[51];
	float pc = 0.25, pm = 0.01;
	int v_prim[50][50];
	int first = 0;

	float maxFitness = 0;
	int pozFitness;
	int conditie = 2;
	while (conditie) {
		if (first == 0) {
			cout << endl << "Initializarea populatiei:";
			first++;
		}
		else {
			cout << endl << "Populatie curenta:";
		}

		for (int i = 0; i < n; i++) {
			cout << endl << "v" << i + 1 << ": ";
			for (int j = 0; j < l; j++) {
				cout << v[i][j];
			}
		}

		cout << endl << endl;

		for (int i = 0; i < n; i++) {
			eval[i] = evaluareFitness(v, i, x1, x2, nr_biti_x1, nr_biti_x2, interval_stanga_x1, interval_stanga_x2);
		}

		fitness_probabilitati(n, eval, fitness_total, prob_selectie, prob_cumulate);
		ruleta(v, n, l, prob_cumulate, v_prim);
		incrucisare(v_prim, n, l, pc);
		mutatie(v_prim, n, l, pm);

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < l; j++) {
				v[i][j] = v_prim[i][j];
			}
		}

		conditie--;
		if (conditie != 0) {
			cout << endl << endl << "--------------------------------------" << endl;
		}
		else {
			for (int i = 0; i < n; i++) {
				if (eval[i] > maxFitness) {
					maxFitness = eval[i];
					pozFitness = i;
				}
			}
			cout << endl << endl << "Individul cu cel mai mare fitness este individul " << pozFitness + 1 << " cu fitness-ul " << maxFitness << ".";
		}
	}
}