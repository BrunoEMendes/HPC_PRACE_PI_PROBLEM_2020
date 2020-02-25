#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <limits.h>
#include <pthread.h>
#define NUM_THREADS 4


// função de tempo implementada pela Professora Margarida Moura em SPD
// https://tutoria.ualg.pt/2019/pluginfile.php/132032/mod_resource/content/0/Guia1-Exerc%C3%ADcios%20Iniciais.pdf
double get_function_run_time()
{
    struct timeval time;
    gettimeofday(&time,NULL);
    return ( (double)(time.tv_sec*1000000+time.tv_usec)/1000000 );
}

double solve_pi(int n)
{
    double result = 4.0 / n;
    double sum = 0.0;
    for(int i = 1; i <= n; ++i)
        sum += 1 / (1 + pow((i - 1 /2.0) / n , 2));
    return result * sum;
}

int n_min(double error)
{
    int n = 1;
    while(fabs(solve_pi(n) - M_PI) > error)
        n++;
    return n;
}


void problem_B()
{
    double input_number;
    scanf("%lf", &input_number);
    double time = get_function_run_time();
    int res = n_min(input_number);
    printf("Elapsed time %.12lf seconds\n", get_function_run_time() - time);
    printf("Nmin for error %.10lf is %d\n", input_number, res);

}


void problem_A()
{
    int N;
    scanf("%d", &N);
    double time = get_function_run_time();
    double res = solve_pi(N);
    printf("Elapsed time %.12lf seconds\n", get_function_run_time() - time);
    printf("Pi value for %d is %.24lf with an error of %.24lf\n", N, res, fabs(res - M_PI));
}

typedef struct pthreadarg
{
    int pthread_number;
    int jump;
    double error;
}PthreadArg;

PthreadArg pthreadArg(int pthread_number, int jump, double error)
{
    PthreadArg result;
    result.pthread_number = pthread_number;
    result.jump = jump;
    result.error = error;
    return result;
}
void *n_min_thread(void*  pa)
{
    PthreadArg *pArg = (PthreadArg *)pa;
    int n =  pArg->pthread_number;
    double error = pArg->error;
    int jump = pArg->jump;
    while(fabs(solve_pi(n) - M_PI) > error)
        n += jump;
    pthread_exit((void *) n);
}

void problem_C()
{

    double error;
    scanf("%lf", &error);

    void * size_array_thread[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    PthreadArg pa[NUM_THREADS];
    double time = get_function_run_time();

    for(int i = 0; i < NUM_THREADS; ++i)
        pa[i] = pthreadArg(i + 1, NUM_THREADS, error);

    for(int i = 0; i < NUM_THREADS; ++i)
        pthread_create(&threads[i], NULL, n_min_thread, (void *) &pa[i]);

    for(int i = 0; i < NUM_THREADS; ++i)
        pthread_join(threads[i], &size_array_thread[i]);

    int min = -1;
    for(int i = 0; i < NUM_THREADS; i++)
        if(min == -1)
            min = (int)size_array_thread[i];
        else if(min > (int)size_array_thread[i])
            min = (int)size_array_thread[i];


    printf("Elapsed time %.12lf seconds\n", get_function_run_time() - time);
    printf("Nmin for error %.12lf is %d\n", error, min);
}


int main(int argc, char ** argv)
{

    // problem_A();
    // problem_B();
    // problem_C();
    return 0;
}