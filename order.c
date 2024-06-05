#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "p3220028-p3220031.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Diloseis conditions
pthread_mutex_t lock1;
pthread_mutex_t lock2;
pthread_mutex_t lock3;
pthread_mutex_t lock4;
pthread_mutex_t lock5;
pthread_mutex_t lock6;
pthread_mutex_t lock7;
pthread_cond_t cond_tel;
pthread_cond_t cond_prep;
pthread_cond_t cond_oven;
pthread_cond_t cond_deliv;

#define Ncust 10

struct ThreadData {
    int id;
    unsigned int seed;
    int numPizzas;
    int chargeTime;
    double chargeStatus;
	time_t startTime;
    time_t endTime;
    time_t startCoolingTime;
    time_t endCoolingTime;
};

enum PizzaType { Margherita, Pepperoni, Special };
enum OrderStatus { Charged, Failed };

int ovens = Noven; //fournoi
int telephones = Ntel; //sto tilefono
int deliverers = Ndeliverer; //delivery
int walterwhite= Ncook; //paraskevastes

int totalIncome = 0;
int totalPizzas[3] = {0, 0, 0}; // Margherita, Pepperoni, Special
int totalOrders[2] = {0, 0}; // Successful, Failed
double totalServiceTime = 0;
double maxServiceTime = 0;
double totalCoolingTime = 0;
double maxCoolingTime = 0;

void *order(void *x);

int main(int argc, char *argv[]) {    

    if (argc < 3) {
        printf("Usage: %s <number of customers> <seed>\n", argv[0]);
        return -1;
    }

    unsigned int seed = atoi(argv[2]);
    int customers = atoi(argv[1]);

    int rc;
    pthread_t threads[customers];
    struct ThreadData data[customers];
   
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);
    pthread_mutex_init(&lock3, NULL);
    pthread_mutex_init(&lock4, NULL);
    pthread_mutex_init(&lock5, NULL);
    pthread_mutex_init(&lock6, NULL);
    pthread_mutex_init(&lock7, NULL);
    pthread_cond_init(&cond_tel, NULL);
    pthread_cond_init(&cond_prep, NULL);
    pthread_cond_init(&cond_oven, NULL);
    pthread_cond_init(&cond_deliv, NULL);
   
    for (int i = 0; i < customers; i++) {
        data[i].id = i + 1;
        data[i].seed = seed;
        data[i].numPizzas = Norderlow + rand_r(&seed) % (Norderhigh - Norderlow + 1);
        data[i].chargeTime = Tpaymentlow + rand_r(&seed) % (Tpaymenthigh - Tpaymentlow + 1);
        data[i].chargeStatus = ((double)rand_r(&seed) / RAND_MAX) * 100;
        printf("(%d)Main: creating thread\n", i + 1);
        rc = pthread_create(&threads[i], NULL, order, &data[i]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            return -1;
        }
        int waiting = Torderlow + rand_r(&seed) % (Torderhigh - Torderlow + 1);
        sleep(waiting);
    }

    for (int i = 0; i < customers; i++) {
        pthread_join(threads[i], NULL);
    }
   
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    pthread_mutex_destroy(&lock3);
    pthread_mutex_destroy(&lock4);
    pthread_mutex_destroy(&lock5);
    pthread_mutex_destroy(&lock6);
    pthread_mutex_destroy(&lock7);
    pthread_cond_destroy(&cond_oven);
    pthread_cond_destroy(&cond_prep);
    pthread_cond_destroy(&cond_tel);
    pthread_cond_destroy(&cond_deliv);
    
    printf("\n------------------STATS------------------\n");

	printf("Total income: %d\n", totalIncome);
    printf("Total pizzas sold: Margherita: %d, Pepperoni: %d, Special: %d\n", totalPizzas[Margherita], totalPizzas[Pepperoni], totalPizzas[Special]);
    printf("Total orders: Successful: %d, Failed: %d\n", totalOrders[Charged], totalOrders[Failed]);
    printf("Average service time: %.2f minutes, Maximum service time: %.2f minutes\n", totalServiceTime / totalOrders[Charged], maxServiceTime);
    printf("Average cooling time: %.2f minutes, Maximum cooling time: %.2f minutes\n", totalCoolingTime / totalOrders[Charged], maxCoolingTime);
   
   	printf("\n------------------GOODBYE------------------\n");
   
    return 0;
}

void *order(void *x) {

    struct ThreadData *data = (struct ThreadData *)x;
    struct timespec tpstart;
    clock_gettime(CLOCK_REALTIME, &tpstart);
    data->startTime = tpstart.tv_sec;
    int id = data->id;
    unsigned int seed = data->seed;
    int numPizzas = data->numPizzas;
    int chargeTime = data->chargeTime;
    double chargeStatus = data->chargeStatus;
    int rc;

printf("*********************************************\n");
printf("(%d)Telephone for thread\n", id);
rc = pthread_mutex_lock(&lock1);
	
	struct timespec tporder_start;
    clock_gettime(CLOCK_REALTIME, &tporder_start);
    time_t order_start = tporder_start.tv_sec;
	
	if (rc!= 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}

	while (telephones == 0) {
		printf("Waiting in line because thread %d did not find available caller...\n", id);
		rc = pthread_cond_wait(&cond_tel, &lock1);
	}
	
	telephones--;
	
rc = pthread_mutex_unlock(&lock1);
	
	numPizzas = Norderlow + rand_r(&seed) % (Norderhigh - Norderlow + 1);
	printf("(%d)Telephone for thread went through\n", id);
	enum PizzaType orderedPizzas[numPizzas];
	printf("(%d)Numbers of pizzas: %d\n", id,numPizzas);
	
	for (int i = 0; i < numPizzas; i++) {
		double r = (double)rand_r(&seed) / RAND_MAX * 100;
		enum PizzaType type;
		if (r < Pm) {
			type = Margherita;
		}
		else if (r < Pm + Pp) {
			type = Pepperoni;
		}
		else {
			type = Special;
		}
		
		orderedPizzas[i] = type; // Store the type of pizza ordered
		
		switch (type) {
			case Margherita: printf("Margherita\n"); totalPizzas[Margherita]++; break;
			case Pepperoni: printf("Pepperoni\n"); totalPizzas[Pepperoni]++; break;
			case Special: printf("Special\n"); totalPizzas[Special]++; break;
		}
	}
	
	chargeTime = Tpaymentlow + rand_r(&seed) % (Tpaymenthigh - Tpaymentlow + 1);
	printf("(%d)Time to charge the card: %d seconds\n",id,chargeTime);
	double r = ((double)rand_r(&seed) / RAND_MAX) * 100;
	sleep(chargeTime);
	
	enum OrderStatus status;
	if (r < Pfail) {
		status = Failed;} 
	else {
		status = Charged;
	}
	
	switch (status) {
		case Failed:
			printf("(%d)The charge failed. The order is cancelled.\n",id);
			rc = pthread_mutex_lock(&lock2);
				if (rc!= 0) {	
					printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
					pthread_exit(&rc);
				}
				telephones++;
				rc = pthread_cond_signal(&cond_tel);
			rc = pthread_mutex_unlock(&lock2);
			totalOrders[Failed]++;
			pthread_exit((void *)-1);
		case Charged:
			printf("(%d)The charge succeeded. The order is placed.\n",id);
			totalOrders[Charged]++;
			// Add the price of each pizza to totalIncome
			for (int i = 0; i < numPizzas; i++) {
				switch (orderedPizzas[i]) {
					case Margherita: 
						totalIncome += Cm; break;
					case Pepperoni: 
						totalIncome += Cp; break;
					case Special: 
						totalIncome += Cs; break;
					}
			}
	}
	
rc = pthread_mutex_lock(&lock3);
	if (rc!= 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
	}	
	printf("(%d)Telephone for order was successfully arrived!\n", id);
	telephones++;
	rc = pthread_cond_signal(&cond_tel);
rc = pthread_mutex_unlock(&lock3);

printf("*********************************************\n");
printf("Order %d is getting ready by the preparers(aka walter & jessie)...\n", id);
rc = pthread_mutex_lock(&lock4);
	
	if (rc!= 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	while (walterwhite == 0) {
		printf("(%d)Order waiting for a preparer...\n", id);
		rc = pthread_cond_wait(&cond_prep, &lock4);
	}
	printf("(%d)Order found a preparer!\n", id);
	
	walterwhite--;
rc = pthread_mutex_unlock(&lock4);

sleep(numPizzas*Tprep);

rc = pthread_mutex_lock(&lock5);

	if (rc!= 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}

	while (ovens < numPizzas) {
		printf("(%d)Order waiting for %d available ovens...\n", id, numPizzas);
		rc = pthread_cond_wait(&cond_oven, &lock5);
	}

	printf("(%d)Preparer for order finished!\n", id);
	walterwhite++;
	ovens -= numPizzas;    
	rc = pthread_cond_signal(&cond_prep);
rc = pthread_mutex_unlock(&lock5);

sleep(Tbake);
struct timespec tpstartcooling;
clock_gettime(CLOCK_REALTIME, &tpstartcooling);
data->startCoolingTime = tpstartcooling.tv_sec;


rc = pthread_mutex_lock(&lock6);
	
	if (rc!= 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}

	while (deliverers==0) {
		printf("(%d)Order waiting for a deliverer...\n", id);
		rc = pthread_cond_wait(&cond_deliv, &lock6);
	}
	
	printf("(%d)Order baked and given to deliverers!\n", id);
	ovens += numPizzas;  
	
	rc = pthread_cond_broadcast(&cond_oven);
	deliverers--;
rc = pthread_mutex_unlock(&lock6);

sleep(numPizzas*Tpack);

struct timespec tporder_end;
clock_gettime(CLOCK_REALTIME, &tporder_end);
time_t order_end = tporder_end.tv_sec;

printf("(%d)Time for the order to be prepared: %.2f minutes\n",id,difftime(order_end, order_start));

int timefordelivery = Tdellow + rand_r(&seed) % (Tdelhigh - Tdellow + 1);
printf("(%d)Time for the deliverer to deliver the pizzas and come back is %d minutes\n", id,2* timefordelivery);
sleep(timefordelivery);

struct timespec tpendcooling;
clock_gettime(CLOCK_REALTIME, &tpendcooling);
data->endCoolingTime = tpendcooling.tv_sec;

struct timespec tporder_delivered;
clock_gettime(CLOCK_REALTIME, &tporder_delivered);
time_t order_delivered = tporder_delivered.tv_sec;

printf("(%d)Total time for the order to be delivered: %.2f minutes\n",id,difftime(order_delivered, order_start));

sleep(timefordelivery);

rc = pthread_mutex_lock(&lock7);

	if (rc!= 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	printf("*********************************************\n");
	printf("(%d)Order finished!\n", id);
	deliverers++;
	
	rc = pthread_cond_signal(&cond_deliv);
rc = pthread_mutex_unlock(&lock7);
printf("*********************************************\n");

struct timespec tpend;
clock_gettime(CLOCK_REALTIME, &tpend);
data->endTime = tpend.tv_sec;

totalServiceTime += difftime(data->endTime, data->startTime);
maxServiceTime = fmax(maxServiceTime, difftime(data->endTime, data->startTime));

totalCoolingTime +=difftime(data->endCoolingTime, data->startCoolingTime);
maxCoolingTime = fmax(maxCoolingTime, difftime(data->endCoolingTime, data->startCoolingTime));

pthread_exit(NULL);
}
