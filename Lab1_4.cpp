#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <math.h>
#include <locale>
#define Pi 3.1415926535
using namespace std;

int main() {
	setlocale(LC_ALL, "RUS");
	double start = 0;
	double end = 5;
	double num;
	cout << "Введите число для проверки:\n";
	scanf("%lg", &num);

	if ((num > start) && (num <= end)) printf("Число %lg находится в диапазоне от 0 до 5", num);
	else printf("Число %lg НЕ находится в диапазоне от 0 до 5", num);
}
