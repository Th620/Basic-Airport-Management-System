#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Defining Structures

typedef struct Passenger Passenger;
typedef struct Passenger *ptrPassanger;
typedef struct Passenger *stack;
struct Passenger
{
    int id;
    char *name;
    ptrPassanger next;
};

typedef struct Queue
{
    ptrPassanger Head, Tail;
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

void enqueue(Queue *q, ptrPassanger p)
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

Passenger dequeue(Queue *q)
{
    Passenger p;
    if (isEmpty(*q))
    {
        return p;
    }
    p.id = (*q).Head->id;
    p.name = (*q).Head->name;
    ptrPassanger temp = (*q).Head;

    if ((*q).Head == (*q).Tail)
    {
        (*q).Head = NULL;
        (*q).Tail = NULL;
    }
    else
    {
        (*q).Head = (*q).Head->next;
    }

    free(temp);
    p.next = NULL;
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
        ptrPassanger temp = (*q).Head;
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
    stack temp = *s;
    *s = (*s)->next;
    free(temp);
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
        stack temp;
        temp = *s;
        *s = (*s)->next;
        free(temp);
    }
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

void addPassengerToFlightQueue(Flight *flight, Passenger passenger)
{

    enqueue(&flight->waitingQueue, &passenger);
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
        Passenger p = dequeue(&flight->waitingQueue);
        stack_passenger(&flight->boardedPassengers, &p);
        flight->passengerCount = flight->passengerCount + 1;
        printf("Passenger boarded into the flight #%d successfully\n", flight->id);
    }
}

void print_flight(Flight flight)
{
    printf("-- Flight #%d --\n", &flight.id);
    printf("ID: %d\n", flight.id);
    printf("Destination: %s\n", flight.destination);
    printf("Capacity: \n", &flight.capacity);
    printf("Number of pasengers: \n", &flight.passengerCount);
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

// Main funtion:

int main(int argc, char const *argv[])
{

    int flightsCount;
    int passengersCount;

    Flight *f;

    printf("--\tWelcome to Airport Management System\t--\n\n");

    // printf("- How many flights are available for Today: \t");
    // scanf("%d", &flightsCount);

    char *condFlight = "";

    do
    {
        printf("- How many flights are available for Today %s: \t", condFlight);
        scanf("%d", &flightsCount);
        condFlight = "(at least 3)";
    } while (flightsCount < 3);

    f = (Flight *)malloc(flightsCount * sizeof(Flight));

    for (int i = 0; i < flightsCount; i++)
    {
        char *dest;
        int capacity;

        printf("\n . Flight - %d:\n", i + 1);
        printf("   destination: ");
        scanf("%s", &dest);

        do
        {
            printf("   capacity (at least 5): ");
            scanf("%d", &capacity);
        } while (capacity < 5);

        f[i] = initFlight((i + 1) * 100 + capacity, dest, capacity);
    }

    printf("\n- How many passengers are available for Today: \t");
    scanf("%d", &passengersCount);

    if (passengersCount < 15)
    {
        do
        {
            printf("\n- How many passengers are available for Today (at least 15): \t");
            scanf("%d", &passengersCount);
        } while (passengersCount < 15);
    }

    Passenger *p;

    p = (Passenger *)malloc(passengersCount * sizeof(Passenger));

    for (int j = 0; j < passengersCount; j++)
    {
        char *name;
        printf("Paseenger - %d (name): ", j + 1);
        scanf("%s", &name);

        p[j] = initPassenger(j, name);
    }

    // Passenger p1 = initPassenger(1, "passsenger-1");
    // Passenger p2 = initPassenger(2, "passsenger-2");
    // Passenger p3 = initPassenger(3, "passsenger-3");
    // Passenger p4 = initPassenger(4, "passsenger-4");
    // Passenger p5 = initPassenger(5, "passsenger-5");
    // Passenger p6 = initPassenger(6, "passsenger-6");
    // Passenger p7 = initPassenger(7, "passsenger-7");
    // Passenger p8 = initPassenger(8, "passsenger-8");
    // Passenger p9 = initPassenger(9, "passsenger-9");
    // Passenger p10 = initPassenger(10, "passsenger-10");
    // Passenger p11 = initPassenger(11, "passsenger-11");
    // Passenger p12 = initPassenger(12, "passsenger-12");
    // Passenger p13 = initPassenger(13, "passsenger-13");
    // Passenger p14 = initPassenger(14, "passsenger-14");
    // Passenger p15 = initPassenger(15, "passsenger-15");

    // Passenger p = initPassenger(1, "test");
    // print_passenger(p);
    return 0;
}
