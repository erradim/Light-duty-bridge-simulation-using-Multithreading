#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "time.h"
#include "pthread.h"
#include "string.h"

static void oneVehicle(int vehicle_id, bool dir, bool time_to_cross);
static void arriveBridge(bool dir);
static void crossBridge(int vehicle_id, bool dir, bool time_to_cross);
static void exitBridge(int vehicle_id, bool dir);
static int crossing(double sec);
static int get_random_direction();

#define NB_VEHICLES 20
#define BRIDGE_LIMIT 3

static int timer = 5.0;
static int on_the_bridge = 0;
static bool crossing_dir = -1;
int departure_index = 1;

typedef struct
{
    int vehicle_id;
    bool dir;
    bool time_to_cross;

    pthread_t thread_vehicle[NB_VEHICLES];
} vehicle_t;

static vehicle_t vehicle;

static void *fn_vehicle(void *p_data)
{
    vehicle.vehicle_id = (int)p_data;
    vehicle.dir = get_random_direction();
    vehicle.time_to_cross = 1;

    while (1)
    {
        printf("Vehicle %d has arrived at the bridge\n",
               vehicle.vehicle_id);
        oneVehicle(vehicle.vehicle_id, vehicle.dir, vehicle.time_to_cross);
        pthread_exit(vehicle.thread_vehicle[vehicle.vehicle_id]);
    }
    return NULL;
}

static void oneVehicle(int vehicle_id, bool dir, bool time_to_cross)
{
    arriveBridge(dir);
    crossBridge(vehicle_id, dir, time_to_cross);
    exitBridge(vehicle_id, dir);
}

static void arriveBridge(bool dir)
{
    while (1)
    {
        if (on_the_bridge == 0)
        {
            crossing_dir = dir;
            return;
        }
        if (on_the_bridge < BRIDGE_LIMIT && crossing_dir == dir)
        {
            return;
        }
    }
}

static void crossBridge(int vehicle_id, bool dir, bool time_to_cross)
{
    on_the_bridge++;
    printf("Vehicle %d is crossing the bridge in direction %d\n", vehicle_id, dir);
    while (time_to_cross)
    {
        crossing(timer);
        time_to_cross = 0;
    }
    return;
}

static void exitBridge(int vehicle_id, bool dir)
{
    on_the_bridge--;
    if (dir == 0)
    {
        printf("Vehicle %d has exited the bridge in direction %d. Departure index: %d\n",
               vehicle_id, dir, departure_index);
        departure_index++;
    }
    if (dir == 1)
    {
        printf("Vehicle %d has exited the bridge in direction %d. Departure index: %d\n",
               vehicle_id, dir, departure_index);
        departure_index++;
    }
    return;
}

int main()
{
    int i, ret, choice;

    printf("Please choose the vehicle arrival schedule\n \
           Enter 1 for: 5 : DELAY(10) : 5 : DELAY(10) : 5 : DELAY(10) : 5\n \
           Enter 2 for: 10 : DELAY(10) : 10\n \
           Enter 3 for: 20\n \
           choice:");
    scanf("%d", &choice);

    printf("Creation of the Vehicles\n");
    for (i = 0; i < NB_VEHICLES; i++)
    {
        ret = pthread_create(&vehicle.thread_vehicle[i], NULL,
                             fn_vehicle, (void *)i);
        if ((choice == 1 && i == 4) || (choice == 1 && i == 9) || (choice == 1 && i == 14))
            crossing(10);
        if (choice == 2 && i == 9)
            crossing(10);
        if (ret)
            return EXIT_FAILURE;
    }

    i = 0;
    for (i = 0; i < NB_VEHICLES; i++)
    {
        pthread_join(vehicle.thread_vehicle[i], NULL);
    }

    return EXIT_SUCCESS;
}

static int crossing(double sec)
{
    clock_t begin;
    double time_spent;
    unsigned int i;
    begin = clock();
    for (i = 0; 1; i++)
    {
        time_spent = (double)(clock() - begin) / CLOCKS_PER_SEC;
        if (time_spent >= sec)
            break;
    }
    return 0;
}

static int get_random_direction()
{
    double val;
    int max = 2;

    val = (double)max * rand();
    val = val / (RAND_MAX + 1.0);

    return ((int)val);
}