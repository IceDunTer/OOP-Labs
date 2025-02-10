#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <math.h>
#include <locale>
#define Pi 3.1415926535
using namespace std;

int main() {
	setlocale(LC_ALL, "RUS");
	double a, b;

	cout << "Введите число А:";
	scanf("%lg", &a);
	cout << "Введите число B:";
	scanf("%lg", &b);

	double chisl = atan((sin(a + Pi))/(cos(b+2.87)));
	double znam = pow(a + pow(cos(b), 2), 0.5);
	double ans = chisl / znam;
	printf("Сумма чисел А и В равна %.4lg", ans);
}
