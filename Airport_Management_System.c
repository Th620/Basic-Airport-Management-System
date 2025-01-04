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
    printf(" #%d: Passenger name ' %s '\n", passenger.id, passenger.name);
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

Passenger *dequeue(Queue *q)
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
    else
    {
        Queue qALT;
        qALT.Head = q.Head->next;
        qALT.Tail = q.Tail;
        print_queue(qALT);
        print_passenger(*(q.Head));
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
    else
    {
        print_stack(s->next);
        print_passenger(*s);
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
        Passenger *p = dequeue(&flight->waitingQueue);
        stack_passenger(&flight->boardedPassengers, p);
        flight->passengerCount = flight->passengerCount + 1;
        printf("Check-in for passenger %d in flight %d\n", p->id, flight->id);
    }
}

void print_flight(Flight flight)
{
    printf("Flight #%d -------\n", flight.id);
    printf(" ID: %d\n", flight.id);
    printf(" Destination: %s\n", &flight.destination);
    printf(" Capacity: %d\n", flight.capacity);
    printf(" Number of pasengers: %d\n", flight.passengerCount);
    printf(" Passengers of the waiting queue: \n");
    if (isEmpty(flight.waitingQueue))
    {
        printf(" Empty\n");
    }
    else
    {
        print_queue(flight.waitingQueue);
    }

    printf(" Boarded Passengers: \n");
    if (isEmpty_stack(flight.boardedPassengers))
    {
        printf(" Empty\n");
    }
    else
    {
        print_stack(flight.boardedPassengers);
    }
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

    while (index < flightsCount)
    {
        if ((flights + index)->id == flightID)
        {
            return *(flights + index);
        }
        index++;
    }
    return f;
}

// Main funtion:

int main(int argc, char const *argv[])
{
    // Initialize Flights and Passengers

    int flightsCount;
    int passengersCount;

    Flight *f;

    printf(" --\tWelcome to Airport Management System\t--\n\n");

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
        printf("Allocation Failed!\n");
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
        printf("Allocation Failed!\n");
        exit(-1);
    }

    for (int j = 0; j < passengersCount; j++)
    {
        char *name;
        name = (char *)malloc(49 * sizeof(char));
        printf(" . Passenger - %d (name): ", j + 1);
        scanf("%s", name);

        p[j] = initPassenger(j, name);
    }

    // Add Passengers to Flights Queues

    for (int k = 0; k < flightsCount; k++)
    {
        printf("\n\n------------------------------------------------------\n\n");

        printf("Adding 5 passengers for the Flight #%d queue ... \n", f[k].id);
        for (int i = k * 5; i < 5 * (k + 1); i++)
        {
            addPassengerToFlightQueue(&f[k], p + i);
        }

        // Print Flights Information
        printf("\n");
        print_flight(f[k]);
        printf("\n");

        // Check in for each passenger

        printf("Finish 5 passengers chack-in for the Flight #%d ... \n", f[k].id);
        for (int o = k * 5; o < 5 * (k + 1); o++)
        {
            finish_passenger_check_in(&f[k]);
        }

        // Print Flights Information
        printf("\n");
        print_flight(f[k]);

        printf("\n\n------------------------------------------------------\n\n");
    }

    // Search for Passengers

    int searchCount;

    do
    {
        printf("\n- How many passengers you want to search for (at least 5): \t");
        scanf("%d", &searchCount);
        printf("\n\n");
    } while (searchCount < 5);

    for (int j = 0; j < searchCount; j++)
    {
        int flightID, passengerID;
        printf(" . Enter Passenger ID: ");
        scanf("%d", &passengerID);
        printf(" . Enter Flight ID: ");
        scanf("%d", &flightID);

        // Flight flight = find_flight(f, flightsCount, flightID);

        int index;
        int status = 10;

        Flight exitedFlight;

        // while (index < flightsCount && f[index].id != flightID)
        // {
        //     index++;
        // }
        // if (f[index].id == flightID)
        // {
        //     exitedFlight = f[index];
        //     status = find_passenger(exitedFlight, passengerID);
        // }

        for (int x = 0; x < flightsCount; x++)
        {
            if (f[x].id == flightID)
            {
                exitedFlight = f[x];
                status = find_passenger(exitedFlight, passengerID);
            }
        }

        if (status == 0)
        {
            printf("\n   -> Not in the Flight!\n\n");
        }
        else if (status == 1)
        {
            printf("\n   -> In the queue\n\n");
        }
        else if (status == 2)
        {
            printf("\n   -> In the stack\n\n");
        }
        else
        {
            printf("\n   -> Flight not found!\n\n");
        }

        printf("------------------------------------------------------\n\n");
    }

    // Free Queues, Stacks and Flights

    for (int w = 0; w < flightsCount; w++)
    {
        free_queue(&f[w].waitingQueue);
        free_stack(&f[w].boardedPassengers);
    }
    free(f);

    return 0;
}
