// Zoe Rodriguez
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct SortingAvgInput
{
    double average;
    double *array;
    int length;
} SortingAvgInput;

typedef struct MergingAvgInput
{
    double *array1;
    double *array2;
    double *resultArray;
    int length;
    double average1;
    double average2;
    double combinedAvg;
} MergingAvgInput;

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

    selectionSort(((SortingAvgInput *)input)->array,
                  ((SortingAvgInput *)input)->length);
    // we can reassign the input, and that works
    ((SortingAvgInput *)input)->average = getAverage(((SortingAvgInput *)input)->array, ((SortingAvgInput *)input)->length);

    // we also return the input, which we can make work later
    pthread_exit(input);
}
// to be called in a thread
void *mergeArray(void *input)
{
    ((MergingAvgInput *)input)->combinedAvg = (((MergingAvgInput *)input)->average1 + ((MergingAvgInput *)input)->average2) / 2;

    int subsectionLength = ((MergingAvgInput *)input)->length;
    int array1Index = 0, array2Index = 0, combinedArrayIndex = 0;

    while (array1Index < subsectionLength && array2Index < subsectionLength) // this loop will run till a or b is completely traversed
    {
        if (((MergingAvgInput *)input)->array1[array1Index] < ((MergingAvgInput *)input)->array2[array2Index])
            ((MergingAvgInput *)input)->resultArray[combinedArrayIndex++] = ((MergingAvgInput *)input)->array1[array1Index++];
        // here, as soon as we copy an element in c, we increment the iterator so that the next element is copied at next index.
        // When we copy an element from a to c, we increment i also because now we will compare with the next element of a.
        else
            ((MergingAvgInput *)input)->resultArray[combinedArrayIndex++] = ((MergingAvgInput *)input)->array2[array2Index++];
    }

    while (array1Index < subsectionLength) // copying the leftover elements of a, if any
        ((MergingAvgInput *)input)->resultArray[combinedArrayIndex++] = ((MergingAvgInput *)input)->array1[array1Index++];

    while (array2Index < subsectionLength) // copying the leftover elements of b, if any
        ((MergingAvgInput *)input)->resultArray[combinedArrayIndex++] = ((MergingAvgInput *)input)->array2[array2Index++];

    return input;
}

int counter;
double randomNumber()
{
    srand(time(NULL) + counter);
    counter += 87564659;
    double div = RAND_MAX / 999;
    return 1 + (rand() / div);
}

int main(int argCount, char *arvg[])
{

    // time measuring devices
    struct timespec ts_begin, ts_end;
    double elapsed;

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

    puts("-----Unsorted--Input-------");
    for (int j = 0; j < desiredLength; j++)
    {
        printf("%lf\n", a[j]);
    }
    puts("END--Unsorted--Input----END");

    // copy a into b
    for (int j = 0; j < desiredLength; j++)
    {
        b[j] = a[j];
    }

    //-------------------- ONE THREAD CASE --------------------
    // create thB sortThread_avg to sort B and compute its average

    clock_gettime(CLOCK_MONOTONIC, &ts_begin);
    pthread_t sortingAvg;
    SortingAvgInput *input = malloc(sizeof(SortingAvgInput));
    input->array = b;
    input->length = desiredLength;
    input->average = 0;

    pthread_create(&sortingAvg, NULL, sortingAverage, input);

    pthread_join(sortingAvg, NULL);

    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    elapsed = ts_end.tv_sec - ts_begin.tv_sec;
    elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000000.0;

    printf("Sorting by ONE thread is done in: %f ms\n", elapsed * 1000);
    for (int j = 0; j < 10; j++)
    {
        printf("Value %d %lf\n", j + 1, b[j]);
    }

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

    clock_gettime(CLOCK_MONOTONIC, &ts_begin);
    pthread_t sortingAverageFirstHalf;
    SortingAvgInput *firstInput = malloc(sizeof(SortingAvgInput));
    firstInput->array = aFirstHalf;
    firstInput->length = desiredLength / 2;
    firstInput->average = 0;
    pthread_create(&sortingAverageFirstHalf, NULL, sortingAverage, firstInput);

    pthread_t sortingAverageSecondHalf;
    SortingAvgInput *secondInput = malloc(sizeof(SortingAvgInput));
    secondInput->array = aSecondHalf;
    secondInput->length = desiredLength / 2;
    secondInput->average = 0;
    pthread_create(&sortingAverageSecondHalf, NULL, sortingAverage, secondInput);

    pthread_join(sortingAverageFirstHalf, NULL);
    pthread_join(sortingAverageSecondHalf, NULL);

    pthread_t mergingAverage;
    MergingAvgInput *mergingAverageInput = malloc(sizeof(MergingAvgInput));
    mergingAverageInput->array1 = firstInput->array;
    mergingAverageInput->array2 = secondInput->array;
    mergingAverageInput->average1 = firstInput->average;
    mergingAverageInput->average2 = secondInput->average;
    mergingAverageInput->length = desiredLength / 2;
    mergingAverageInput->combinedAvg = 0;
    mergingAverageInput->resultArray = mergedArray;

    pthread_create(&mergingAverage, NULL, mergeArray, mergingAverageInput);

    pthread_join(mergingAverage, NULL);

    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    elapsed = ts_end.tv_sec - ts_begin.tv_sec;
    elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000000.0;

    printf("Sorting by TWO threads is done in: %f ms\n", elapsed * 1000);
    for (int j = 0; j < 10; j++)
    {
        printf("Value %d %lf\n", j + 1, mergedArray[j]);
    }

    return 0;
}
