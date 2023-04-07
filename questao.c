#include <stdio.h>
#include <locale.h>
#include <omp.h>
#include <stdlib.h>

// Exercício 3: Aproximação do PI
// OpenMP: Redução (Reduction)
#define STEPS 100000000

double pi_sequencial() {
    int i;
    double step, x, pi, sum = 0.0;
    double start, end, time;
    
    printf("Iniciando método sequencial...\n");
    start = omp_get_wtime();
    step = 1.0/(double)STEPS;

    for(i = 0; i < STEPS; i++) {
        x = i * step;
        sum = sum + 4.0/(1.0 + x * x);
    }
    pi = step * sum;
    
    end = omp_get_wtime();
    time = end - start;
    printf("Pi: %f\n", pi);
    printf("Tempo Sequencial: %f\n\n", time);
    return time;
}

double pi_paralelo() {
    int i;
    double step, pi = 0.0;
    double start, end, time;
    
    printf("Iniciando método paralelo...\n");
    start = omp_get_wtime();
    step = 1.0/(double)STEPS;

    #pragma omp parallel num_threads(4)
    {
        double x,sum=0.0;
        int i, id, nthrds;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();

        for(i = id, sum = 0.0; i < STEPS; i = i+nthrds) {
            x = i * step;
            sum = sum + 4.0/(1.0 + x * x);
        }
        
        #pragma omp critical
            pi += step * sum;
    }
    
    end = omp_get_wtime();
    time = end - start;
    printf("Pi: %f\n", pi);
    printf("Tempo Paralelo: %f\n\n", time);
    return time;
}

int main() {
    printf("Calculando o valor de pi...\n\n");
    double time = pi_sequencial()/pi_paralelo();
    printf("Speedup: %f\n", time);
    printf("Eficiência: %f\n", time/4);
}
