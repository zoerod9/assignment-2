// Zoe Rodriguez
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct ThreadIndexes
{
    double average;
    double *array;
    int length;
};

void swap(double *a, double *b)
{
    double temp = *a;
    *a = *b;
    *b = temp;
}

void selectionSort(double *input, int length)
{
    int i, j, minIndex;
    for (i = 0; i < length - 1; i++)
    {
        minIndex = i;
        for (j = i + 1; j < length; j++)
            if (input[j] < input[minIndex])
                minIndex = j;

        if (minIndex != i)
            swap(&input[minIndex], &input[i]);
    }
}

double getAverage(double *input, int inputLength)
{
    double result = 0;
    for (int i = 0; i < inputLength; i++)
    {
        result = result + input[i];
    }

    return result / inputLength;
}

// to be called in a thread
void *sortingAverage(void *input)
{

    selectionSort(((struct ThreadIndexes *)input)->array,
                  ((struct ThreadIndexes *)input)->length);
    // we can reassign the input, and that works
    ((struct ThreadIndexes *)input)->average = getAverage(((struct ThreadIndexes *)input)->array, ((struct ThreadIndexes *)input)->length);

    // we also return the input, which we can make work later
    pthread_exit(input);
}
// to be called in a thread
void *mergeArray(void *input)
{

    sleep(1);

    // should merge two arrays into one

    return NULL;
}

int counter;
double randomNumber()
{
    srand(time(NULL) + counter);
    counter += 876456479; // an actual random number to throw the seed off
    double div = RAND_MAX / 999;
    return 1 + (rand() / div);
}

int main(int argCount, char *arvg[])
{
    int desiredLength;
    sscanf(arvg[1], "%i", &desiredLength);

    // create array A ( n double values) and randomly generate these values
    // also create arrays B and C with the same size of A
    //                    Afirsthalf and AsecondHalf with the half size of A

    double *a = malloc(sizeof(double) * desiredLength);
    double *b = malloc(sizeof(double) * desiredLength);

    // for each item in array,
    for (int i = 0; i < desiredLength; i++)
    {
        // insert random number between 1 and 1000 into arrayToSort's index
        a[i] = randomNumber();
    }

    // copy a into b
    for (int j = 0; j < desiredLength; j++)
    {
        b[j] = a[j];
    }

    //-------------------- ONE THREAD CASE --------------------
    // create thB sortThread_avg to sort B and compute its average

    pthread_t sortingAvg;
    struct ThreadIndexes *input = malloc(sizeof(struct ThreadIndexes));
    input->array = b;
    input->length = desiredLength;
    input->average = 0;

    pthread_create(&sortingAvg, NULL, sortingAverage, input);

    pthread_join(sortingAvg, NULL);

    // for (int j = 0; j < desiredLength; j++)
    // {
    //     printf("%lf\n", b[j]);
    // }

    // //-------------------- TWO THREADS CASE --------------------
    double *aFirstHalf = malloc(sizeof(double) * (desiredLength / 2));
    double *aSecondHalf = malloc(sizeof(double) * (desiredLength / 2));
    double *mergedArray = malloc(sizeof(double) * desiredLength);

    // copy A into Afirsthalf  and AsecondHalf
    for (int j = 0; j < (desiredLength / 2); j++)
    {
        aFirstHalf[j] = a[j];
    }
    for (int j = 0; j < (desiredLength / 2); j++)
    {
        aSecondHalf[j] = a[j + desiredLength / 2];
    }

    pthread_t sortingAverageFirstHalf;
    struct ThreadIndexes *firstInput = malloc(sizeof(struct ThreadIndexes));
    firstInput->array = aFirstHalf;
    firstInput->length = desiredLength/2;
    firstInput->average = 0;
    pthread_create(&sortingAverageFirstHalf, NULL, sortingAverage, firstInput);

    pthread_t sortingAverageSecondHalf;
    struct ThreadIndexes *secondInput = malloc(sizeof(struct ThreadIndexes));
    secondInput->array = aSecondHalf;
    secondInput->length = desiredLength/2;
    secondInput->average = 0;
    pthread_create(&sortingAverageSecondHalf, NULL, sortingAverage, secondInput);

    pthread_join(sortingAverageFirstHalf, NULL);
    pthread_join(sortingAverageSecondHalf, NULL);


    // next steps:
    // create yet another thread to merge both arrays

    // add timing stuff (how long each thing took)

    // write all this in java
    return 0;
}
