#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <math.h>
#include <locale>
#define Pi 3.1415926535
using namespace std;

double f(double x, int n, const double e) {
	double ans = 0;
	for (int m = 1; m <= n; m++) {
		double temp = 0;
		double num = 1;
		for (int i = 0; i < m; i++) {
			num *= x;
			if (fabs(num - temp) <= e) break;
			temp = num;
		}
		if (m % 2 == 1) {
			ans += (num / (double)m);
		}
		else {
			ans += -1 * (num / (double)m);
		}
	}
	return ans;
}

int main() {
	setlocale(LC_ALL, "RUS");
	double func, x1, x2, x3;
	const double e = 0.001;
	int n = 5;
	x1 = 0.1;
	x2 = 0.8;
	x3 = -0.62;

	printf("Для первого Х значение будет %lg\n", f(x1, n, e));
	printf("Для второго Х значение будет %lg\n", f(x2, n, e));
	printf("Для третьего Х значение будет %lg\n", f(x3, n, e));
}
