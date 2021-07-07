#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#ifdef NAN
#endif
#define F1F2L 1.0
#define F1F2R 3.0
#define F1F3L 0.1
#define F1F3R 1.0
#define F2F3L 1.0
#define F2F3R 2.0
#define EPS 1e-4
int coi;
int target_n;
extern double f1(double);
extern double df1(double);
extern double f2(double);
extern double df2(double);
extern double f3(double);
extern double df3(double);


// 3 3 2

double root ( double (*f)(double), double (*g)(double), double (*df)(double), double (*dg)(double), double a, double b, double eps){
	int flm = 0;
	int fls = 0;
	if (((*df)(b) - (*dg)(b)) - ((*df)(a) - (*dg)(a)) > 0)
		flm = 1;
	else
		flm = -1;
	if ((*df)(a) - (*dg)(a) > 0)
		fls = 1;
	else
		fls = -1;
	double x0;
	int flag = 0;
	if ((flm == 1 && fls == 1) || (flm == -1 && fls == -1) ){
		x0 = b;
		flag = 1;
	}
	else{
		x0 = a;
		flag = -1;
	}
	// x_n+1 = x_n - F(x_n)/F'(x_n)
	double x1 = x0 - ((*f)(x0) - (*g)(x0))/((*df)(x0) - (*dg)(x0));
	coi = 1;

	while (((*f)(x1 - flag * eps) - (*g)(x1  - flag * eps)) * ((*f)(x1) - (*g)(x1)) > 0 ){
		x0 = x1;
		x1 = x1 - ((*f)(x1) - (*g)(x1))/((*df)(x1) - (*dg)(x1));
		coi++;
    if (x1 > b){
      printf("No solutions on [%f;%f] \n", a, b);
      return NAN;
    }
	}

	return x1;
}

double _trapezIntegral(double a, double b, int n, double (*f)(double)){
    const double width = (b-a)/n;

    double trapez_integral = 0;
    for(int step = 0; step < n; step++) {
        const double x1 = a + step*width;
        const double x2 = a + (step+1)*width;
        trapez_integral += 0.5*(x2-x1)*((*f)(x1) + (*f)(x2));
    }

    return trapez_integral;

}

/*
// def trapezoid_rule(func, a, b, rtol = 1e-8, nseg0 = 1):
//     """Правило трапеций
//        rtol - желаемая относительная точность вычислений
//        nseg0 - начальное число отрезков разбиения"""
//     nseg = nseg0
//     old_ans = 0.0
//     dx = 1.0 * (b - a) / nseg
//     ans = 0.5 * (func(a) + func(b))
//     for i in range(1, nseg):
//         ans += func(a + i * dx)
//
//     ans *= dx
//     err_est = max(1, abs(ans))
//     while (err_est > abs(rtol * ans)):
//         old_ans = ans
//         ans = 0.5 * (ans + midpoint_rectangle_rule(func, a, b, nseg)) # новые точки для уточнения интеграла
//                                                                       # добавляются ровно в середины предыдущих отрезков
//         nseg *= 2
//         err_est = abs(ans - old_ans)
//
//     return ans
 */

double integral(double (*f)(double),double a, double b, double eps){
  int nseg = 1;
  target_n = 1;
  double old_ans = 0.0;
  double dx = 1.0 * (b-a) / nseg;
  double ans  = 0.5 * ((*f)(a) + (*f)(b));
  ans *= dx;
  double err_est = max(1.0, fabs(ans));
  while (err_est > fabs(eps * ans)){
    const double old_ans = ans;
    nseg *= 2;
    ans = 0.5 * (_trapezIntegral(a,b,nseg,f) + ans);

    err_est = fabs(ans-old_ans);
  }

  target_n = nseg;

  return ans;
}







int main(int argc, char **argv) {
  // double p = 2.0;
  // printf("f3(x) : = %f \t f3'(x) : = %f\n", (*f3)(p), (*df3)(p));
  //printf("x = %f \t ", root(f3,f2,df3,df2,1,10,0.0001));
  coi = 0;
  target_n = 0;
  if (!strcmp (argv[1], "-h") || !strcmp (argv[1], "--help")){
    printf("\t--all-roots/-r  [--count-of-iterations/-c Отобразить число итераций для вычисления корней] Отобразить абсциссы пеерсечения заданных функций\n \
            \t--solve/-s [f1, f2, a,b, eps] Решить уравнение f1=f2 на отрезке [a;b] с заданной точностью eps\n\
            \t--integral/-i [f,a,b,eps] Найти определённый интеграл функции f на отрезке [a;b] с точностью eps методом трапеций\n\
            \t--list-of-functions/-lf Список используемы функций f(x)");
  }
  if (!strcmp (argv[1], "--all-roots") || !strcmp (argv[1], "-r")){
    printf("Abscissas of intersections f1 and f2:\t%f\n", root(f1,f2,df1,df2,F1F2L,F1F2R, EPS));
    if (!strcmp (argv[2], "--count-of-iterations") || !strcmp (argv[2], "-c")){
      printf("Number of iterations for f1 and f2: \t %d \n", coi);
    }
    printf("\nAbscissas of intersections f1 and f3:\t%f\n", root(f1,f3,df1,df3,F1F3L,F1F3R, EPS));
    if (!strcmp (argv[2], "--count-of-iterations") || !strcmp (argv[2], "-c")){
      printf("Number of iterations for f1 and f3: \t %d \n", coi);
    }
    printf("\nAbscissas of intersections f2 and f3:\t%f\n", root(f2,f3,df2,df3,F2F3L,F2F3R, EPS));
    if (!strcmp (argv[2], "--count-of-iterations") || !strcmp (argv[2], "-c")){
      printf("Number of iterations for f2 and f3: \t %d \n", coi);
    }
  }
  if (!strcmp (argv[1], "--solve") || !strcmp (argv[1], "-s")){
    double (*f)(double);
		double (*df)(double);
		double (*g)(double);
		double (*dg)(double);
    if (!strcmp (argv[2], "1")){
			f = f1;
			df = df1;
		}
		else if (!strcmp (argv[2], "2")){
			f = f2;
			df = df2;
		}
		else if (!strcmp (argv[2], "3")){
			f = f3;
			df = df3;
		}
    if (!strcmp (argv[3], "1")){
			g = f1;
			dg = df1;
		}
		else if (!strcmp (argv[3], "2")){
			g = f2;
			dg = df2;
		}
		else if (!strcmp (argv[3], "3")){
			g = f3;
			dg = df3;
		}
    printf("Abscissa of intersection of functions on cut [%s;%s] with eps = %s: \t %.8f \n", argv[4], argv[5], argv[6], root(f,g,df,dg,atof(argv[4]),atof(argv[5]), atof(argv[6])));
  }
  if (!strcmp (argv[1], "--integral") || !strcmp (argv[1], "-i")){
    double (*f)(double);

    if (!strcmp (argv[2], "1")){
			f = f1;

		}
		else if (!strcmp (argv[2], "2")){
			f = f2;

		}
		else if (!strcmp (argv[2], "3")){
			f = f3;
		}
    printf("\nIntegral of this function on cut [%s:%s] with eps = %s is\t %f \t number of needed segments is: ", argv[3], argv[4], argv[5], integral(f,atof(argv[3]), atof(argv[4]), atof(argv[5])));
    printf("%d\n", target_n);

  }
  if (!strcmp (argv[1], "--list-of-functions") || !strcmp (argv[1], "-lf")){
    printf("List of avilable functions: \n 1. \t f(x) = exp(-x)+3  \n 2. \t f(x) = 2x-2 \n 3. \t f(x) = 1/x\n");

  }
  if (!strcmp (argv[1], "--find-area") || !strcmp (argv[1], "-a")){
    const double x1 = root(f1,f2,df1,df2,F1F2L,F1F2R, EPS);
    const double x2 = root(f1,f3,df1,df3,F1F3L,F1F3R, EPS);
    const double x3 = root(f2,f3,df2,df3,F2F3L,F2F3R, EPS);
    const double s1 = integral(f1, x2, x1, EPS);
    const double s2 = integral(f3, x2,x3, EPS);
    const double s3 = integral(f2, x3, x1, EPS);
    printf("S1 = %f \t S2 = %f \t S3 = %f \n \tS = %f \n EPS = %f\n", s1,s2,s3,s1-s2-s3, EPS);


  }
  return 0;
}
