/*
 *  OpenMP lecture exercises
 *  Copyright (C) 2011 by Christian Terboven <terboven@rz.rwth-aachen.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <pthread.h>
 #include <sys/time.h>
 
 
 // Estructura para pasar argumentos a cada hilo
 typedef struct {
     int thread_id;
     int num_threads;
     int n;
     double partial_sum;
 } ThreadData;
 
 
 double CalcPi(int n, int num_threads);
 
 int main(int argc, char **argv) 
 {
     int n = 2000000000;
     const double fPi25DT = 3.141592653589793238462643;
     double fPi;
     double fTimeStart, fTimeEnd;
 
     int num_threads = 4; // default
 
     if (argc >= 2) n = atoi(argv[1]);
     if (argc >= 3) num_threads = atoi(argv[2]);
 
     if (n <= 0 || n > 2147483647 || num_threads <= 0) {
         printf("Valores invalidos. n debe estar entre 1 y 2147483647, num_threads > 0\n");
         return 1;
     }
 
     // get initial time 
     fTimeStart = (double)clock() / CLOCKS_PER_SEC;
 
     fPi = CalcPi(n, num_threads);
 
     //get final fime
     fTimeEnd = (double)clock() / CLOCKS_PER_SEC;
 
     printf("\npi is approximately = %.20f \nError               = %.20f\n",
                         fPi, fabs(fPi - fPi25DT));
 
     // report time
     printf("Average execution time: %.6f seconds/thread\n", (fTimeEnd - fTimeStart) / num_threads);
 
     return 0;
 }
 
 
 double f(double a) {
     return 4.0 / (1.0 + a * a);
 }
 
 
 // Función que ejecuta cada hilo
 void* thread_func(void* arg) {
     ThreadData* data = (ThreadData*) arg;
     int n = data->n;
     int T = data->num_threads;
     int id = data->thread_id;
 
     double fH = 1.0 / (double)n;
     double local_sum = 0.0;
 
     // Cada hilo calcula su rango de iteraciones
     int start = id * (n / T);
     int end = (id == T - 1) ? n : start + (n / T); // último hilo toma el resto
 
     for (int i = start; i < end; i++) {
         double x = fH * ((double)i + 0.5);
         local_sum += f(x);
     }
 
     data->partial_sum = local_sum;
     pthread_exit(NULL);
 }
 
 
 double CalcPi(int n, int num_threads) {
     pthread_t threads[num_threads];
     ThreadData thread_data[num_threads];
 
     // Crear hilos
     for (int t = 0; t < num_threads; t++) {
         thread_data[t].thread_id = t;
         thread_data[t].num_threads = num_threads;
         thread_data[t].n = n;
         thread_data[t].partial_sum = 0.0;
         pthread_create(&threads[t], NULL, thread_func, (void*)&thread_data[t]);
     }
 
     // Esperar hilos y sumar resultados
     double total_sum = 0.0;
     for (int t = 0; t < num_threads; t++) {
         pthread_join(threads[t], NULL);
         total_sum += thread_data[t].partial_sum;
     }
 
     return (1.0 / (double)n) * total_sum;
 }