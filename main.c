// Zoe Rodriguez
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct ThreadIndexes
{
    int first;
    int last;
};

void *mergeArray(void *input)
{

    sleep(1);
    printf("first is %d, %d \n", ((struct ThreadIndexes *)input)->first, ((struct ThreadIndexes *)input)->last);

    // should merge two arrays into one

    return NULL;
}

void *sortingAverage(void *vargp)
{
    sleep(1);
    printf("sorting average \n");

    /* implement selection or insertion sort O(n^2) */
    /* return sorted array and overallAvg */
    return NULL;
}

double randomNumber()
{
    double range = (1000 - 1);
    double div = RAND_MAX / range;
    return 1 + (rand() / div);
}

int main(int argCount, char *arvg[])
{
    puts("hello from main");

    // get desired length from user from argv
    int desiredLength = 10;

    // create array A ( n double values) and randomly generate these values
    // also create arrays B and C with the same size of A
    //                    Afirsthalf and AsecondHalf with the half size of A

    double a[desiredLength];
    double b[desiredLength];
    double c[desiredLength];

    double aFirstHalf[desiredLength / 2];
    double aSecondHalf[desiredLength / 2];

    // for each item in array,
    for (int i = 0; i <= desiredLength - 1; i++)
    {
        // insert random number between 1 and 1000 into arrayToSort's index
        a[i] = randomNumber();
    }

    // copy a into b
    for (int j = 0; j <= desiredLength - 1; j++)
    {
        b[j] = a[j];
    }
    //-------------------- ONE THREAD CASE --------------------
    // create thB sortThread_avg to sort B and compute its average

    pthread_t sortingAvg;
    printf("Before Thread\n");
    struct ThreadIndexes *input = malloc(sizeof(struct ThreadIndexes));
    input->first = 0;
    input->last = 9;
    pthread_create(&sortingAvg, NULL, sortingAverage, input); // firgure out how to pass array

    pthread_join(sortingAvg, NULL);
    printf("After Thread\n");

    //-------------------- TWO THREADS CASE --------------------
    // copy A into Afirsthalf  and AsecondHalf
    for (int j = 0; j <= (desiredLength / 2) - 1; j++)
    {
        aFirstHalf[j] = a[j];
    }
    for (int j = (desiredLength / 2); j <= desiredLength - 1; j++)
    {
        aSecondHalf[j] = a[j];
    }

    pthread_t sortingAverageFirstHalf;
    printf("Before Thread\n");
    struct ThreadIndexes *firstInput = malloc(sizeof(struct ThreadIndexes));
    firstInput->first = 0;
    firstInput->last = 4;
    pthread_create(&sortingAverageFirstHalf, NULL, sortingAverage, firstInput);

    pthread_t sortingAverageSecondHalf;
    printf("Before Thread\n");
    struct ThreadIndexes *secondInput = malloc(sizeof(struct ThreadIndexes));
    secondInput->first = 5;
    secondInput->last = 9;
    pthread_create(&sortingAverageSecondHalf, NULL, sortingAverage, secondInput);

    pthread_join(sortingAverageFirstHalf, NULL);
    pthread_join(sortingAverageSecondHalf, NULL);
    printf("After Thread\n");

    pthread_t mergingThread;
    printf("Before Thread\n");
    // we don't need to pass this object to this thread, because its job is different
    struct ThreadIndexes *mergingInput = malloc(sizeof(struct ThreadIndexes));
    mergingInput->first = 5;
    mergingInput->last = 9;
    pthread_create(&mergingThread, NULL, mergeArray, mergingInput);

    return 0;
}
