#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Defining Structures

typedef struct Passenger Passenger;
typedef struct Passenger *ptrPassenger;
typedef struct Passenger *stack;
struct Passenger
{
    int id;
    char *name;
    ptrPassenger next;
};

typedef struct Queue
{
    ptrPassenger Head, Tail;
} Queue;

typedef struct Flight
{
    int id;
    char *destination;
    int capacity;
    int passengerCount;
    Queue waitingQueue;
    stack boardedPassengers;
} Flight;

// Implementing Passengers Queues and Stacks Operations

// 1- Passengers:

Passenger initPassenger(int id, char *name)
{
    Passenger p;
    p.id = id;
    p.name = name;
    return p;
}

void print_passenger(Passenger passenger)
{
    printf("#%d: Passenger name - %s -\n", passenger.id, passenger.name);
}

// 2- Queues:

Queue initQueue()
{
    Queue q;
    q.Head = NULL;
    q.Tail = NULL;
    return q;
}

bool isEmpty(Queue q)
{
    if (q.Head == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void enqueue(Queue *q, Passenger *p)
{
    p->next = NULL;

    if (q->Head == NULL)
    {
        q->Head = p;
        q->Tail = p;
    }
    else
    {
        q->Tail->next = p;
        q->Tail = p;
    }
}

Passenger * dequeue(Queue *q)
{
    Passenger *p = (Passenger *)malloc(sizeof(Passenger));
    if (isEmpty(*q))
    {
        return p;
    }
    p->id = (*q).Head->id;
    p->name = (*q).Head->name;
    if ((*q).Head == (*q).Tail)
    {
        (*q).Head = NULL;
        (*q).Tail = NULL;
    }
    else
    {
        (*q).Head = (*q).Head->next;
    }
    p->next = NULL;
    return p;
}

void print_queue(Queue q)
{
    if (q.Head == NULL)
    {
        return;
    }
    else if (q.Head == q.Tail)
    {
        printf("#%d: Passenger name - %s -\n", q.Head->id, q.Head->name);
    }
    else
    {
        q.Head = q.Head->next;
        print_queue(q);
    }
}

void free_queue(Queue *q)
{
    while (!isEmpty(*q))
    {
        ptrPassenger temp = (ptrPassenger)malloc(sizeof(Passenger));
        temp = (*q).Head;
        (*q).Head = (*q).Head->next;
        free(temp);
    }

    free((*q).Head);
}

bool search_queue(Queue q, int passenger_id)
{
    while (!isEmpty(q) && q.Head->id != passenger_id)
    {
        q.Head = q.Head->next;
    }
    return !isEmpty(q);
}

// 3- Stacks:

stack initStack()
{
    stack s = NULL;
    return s;
}

bool isEmpty_stack(stack s)
{
    if (s == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void stack_passenger(stack *s, Passenger *p)
{
    p->next = *s;
    *s = p;
}

Passenger unstack(stack *s)
{
    Passenger p;
    if (isEmpty_stack(*s))
        return p;
    p = **s;
    *s = (*s)->next;
    return p;
}

void print_stack(stack s)
{
    if (isEmpty_stack(s))
    {
        return;
    }
    else if (s->next == NULL)
    {
        printf("#%d: Passenger name - %s -\n", s->id, s->name);
    }
    else
    {
        s = s->next;
        print_stack(s);
    }
}

void free_stack(stack *s)
{
    while (!isEmpty_stack(*s))
    {
        Passenger *temp = (Passenger *)malloc(sizeof(Passenger));
        temp = (*s);
        *s = temp->next;
        free(temp);
    }
    free(*s);
}

bool search_stack(stack s, int passenger_id)
{
    while (!isEmpty_stack(s) && s->id != passenger_id)
    {
        s = s->next;
    }

    return !isEmpty_stack(s);
}
// Implementing Flights' Operations

Flight initFlight(int id, char *destination, int capacity)
{
    Flight f;
    f.id = id;
    f.destination = destination;
    f.capacity = capacity;
    f.passengerCount = 0;
    f.waitingQueue = initQueue();
    f.boardedPassengers = initStack();
    return f;
}

void addPassengerToFlightQueue(Flight *flight, Passenger *passenger)
{
    enqueue(&flight->waitingQueue, passenger);
    printf("Passenger added to the flight #%d waiting queue successfully\n", flight->id);
}

void finish_passenger_check_in(Flight *flight)
{
    if (flight->capacity == flight->passengerCount)
    {
        printf("The number of passengers of the flight #%d reached the limit. Please wait for the next flight!\n", flight->id);
    }
    else
    {
        Passenger * p = dequeue(&flight->waitingQueue);
        stack_passenger(&flight->boardedPassengers, p);
        flight->passengerCount = flight->passengerCount + 1;
        printf("Check-in for passenger %d in flight %d\n", p->id, flight->id);
    }
}

void print_flight(Flight flight)
{
    printf("-- Flight #%d --\n", &flight.id);
    printf("ID: %d\n", &flight.id);
    // printf("Destination: %s\n", &flight.destination);
    printf("Capacity: %d\n", &flight.capacity);
    printf("Number of pasengers: %d\n", &flight.passengerCount);
    printf("Passengers of the waiting queue: \n");
    print_queue(flight.waitingQueue);
    printf("Boarded Passengers: \n");
    print_stack(flight.boardedPassengers);
}

int find_passenger(Flight flight, int passengerID)
{
    if (search_queue(flight.waitingQueue, passengerID))
    {
        return 1;
    }
    else if (search_stack(flight.boardedPassengers, passengerID))
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

Flight find_flight(Flight *flights, int flightsCount, int flightID)
{
    int index = 0;

    Flight f;

    while (index < flightsCount && flights[index].id != flightID)
    {
        index++;
    }

    if (index = flightsCount)
    {
        printf("Flight Not Found!\n");
        return f;
    }
    else
    {
        return flights[index];
    }
}

// Main funtion:

int main(int argc, char const *argv[])
{
    // Initialize Flights and Passengers

    int flightsCount;
    int passengersCount;

    Flight *f;

    printf(" --\tWelcome to Airport Management System\t--\n\n");

    // printf("- How many flights are available for Today: \t");
    // scanf("%d", &flightsCount);

    char *condFlight = "";

    do
    {
        printf(" - How many flights are available for Today%s: \t", condFlight);
        scanf("%d", &flightsCount);
        condFlight = " (at least 3)";
    } while (flightsCount < 3);

    f = (Flight *)malloc(flightsCount * sizeof(Flight));
    if (!f)
    {
        printf("Allocation Failed!");
        exit(-1);
    }

    for (int i = 0; i < flightsCount; i++)
    {
        char *dest;
        int capacity;

        printf("\n . Flight - %d:\n", i + 1);
        printf("   destination: ");
        scanf("%s", &dest);

        char *condCapacity = "";

        do
        {
            printf("   capacity%s: ", condCapacity);
            scanf("%d", &capacity);
            condCapacity = " (at least 5)";
        } while (capacity < 5);

        f[i] = initFlight((i + 1) * 100 + capacity, dest, capacity);
    }

    printf("\n- How many passengers are available for Today: \t");
    scanf("%d", &passengersCount);

    if (passengersCount < (5 * flightsCount))
    {
        do
        {
            printf("\n- How many passengers for Today (at least %d): \t", (5 * flightsCount));
            scanf("%d", &passengersCount);

        } while (passengersCount < (5 * flightsCount));
    }

    Passenger *p;

    p = (Passenger *)malloc(passengersCount * sizeof(Passenger));

    if (!p)
    {
        printf("Allocation Failed!");
        exit(-1);
    }

    for (int j = 0; j < passengersCount; j++)
    {
        char *name;
        printf(" . Passenger - %d (name): ", j + 1);
        scanf("%s", &name);

        p[j] = initPassenger(j, name);
    }

    // Add Passengers to Flights Queues

    for (int k = 0; k < flightsCount; k++)
    {

        for (int i = k * 5; i < 5 * (k + 1); i++)
        {
            addPassengerToFlightQueue(&f[k], p + i);
        }

        // Print Flights Information

        // print_flight(f[k]);

        // Check in for each passenger

        for (int o = k * 5; o < 5 * (k + 1); o++)
        {
            finish_passenger_check_in(&f[k]);
        }

        // Print Flights Information

        // print_flight(f[k]);
    }

    // Search for Passengers

    int searchCount;

    do
    {
        printf("\n- How many passengers you want to search for (at least 5): \t");
        scanf("%d", &searchCount);
    } while (searchCount < 5);

    for (int j = 0; j < searchCount; j++)
    {
        int flightID, passengerID;
        printf(" . Enter Passenger ID: ");
        scanf("%d", &passengerID);
        printf(" . Enter Flight ID: ");
        scanf("%d", &flightID);

        Flight flight = find_flight(f, flightsCount, flightID);
        int status = find_passenger(flight, passengerID);

        if (status == 0)
        {
            printf("Not in the Flight!");
        }
        else if (status == 1)
        {
            printf("In the queue");
        }
        else
        {
            printf("In the stack");
        }
    }

    printf("\t |");
    printf("\t |");
    printf("\t |");
    printf("\t |");
    printf("\t |");
    printf("\t |");
    printf("\t |");
    printf("\t |");
    printf("\t |");
    printf("\t |");
    printf("\t |");

    // Free Queues, Stacks and Flights

    for (int w = 0; w < flightsCount; w++)
    {
        printf("----------------------------------------");
        free_queue(&f[w].waitingQueue);
        printf("--------------------                  6        --------------------");
        free_stack(&f[w].boardedPassengers);
        printf("------------------ 8 ----------------------");
    }
    free(f);

    printf("done");

    return 0;
}
