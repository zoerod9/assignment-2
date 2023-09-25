// Zoe Rodriguez
#include  <stdio.h>

int main (){
    puts("hello from main");
    return 0;
}

// randomly generates an array of double values with a given size (say, N=1000), 
// and divides it into two smaller arrays of equal size. 


// It then passes eachm sub-array to sorting_avg thread (creates two copies of 
// the same thread)


// and waits until these two sub-arrays are sorted and the average of the values 
// in each array are computed. 


// It then calls merging_avg thread by passing the already sorted two sub-arrays, 
// and the averages of the first and second sub-arrays as parameters.