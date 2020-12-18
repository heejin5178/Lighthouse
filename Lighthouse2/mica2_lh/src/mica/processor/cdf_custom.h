#include <stdio.h>
#include <stdlib.h>

#define MAX 500

#define MAGIC 0.5

bool min_(int X, int Y) {
  return (((X) < (Y)) ? (X) : (Y));
}
int comp (const void * elem1, const void * elem2) 
{
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int binarySearch(int arr[], int l, int r, int x)
{
    if (r >= l)
    {
        int mid = l + (r - l)/2;
 
        // If the element is present at the middle
        // itself
        if (arr[mid] == x)
            return mid;
 
        // If element is smaller than mid, then it
        // can only be present n left subarray
        if (arr[mid] > x)
            return binarySearch(arr, l, mid-1, x);
 
        // Else the element can only be present
        // in right subarray
        return binarySearch(arr, mid+1, r, x);
    }
 
    // We reach here when element is not present
    // in array
    return -1;
}

int exponentialSearch(int arr[], int n, int x)
{
    // If x is present at firt location itself
    if (arr[0] == x)
        return 0;
 
    // Find range for binary search by
    // repeated doubling
    int i = 1;
    while (i < n && arr[i] <= x)
        i = i*2;
 
    //  Call binary search for the found range.
    return binarySearch(arr, i/2, 
                            min_(i, n-1), x);
}
static double CDF_update(int* dataset, int cur_idx, int x)
{
  double res = 0.0;
  int count = 0;
  /*
  for(int i = 0; i < cur_idx; ++i) {
    if(dataset[i] <= x) {
      count++;
    }
  }
  if(dataset[cur_idx] != 0) {
    printf("dataset ptr is %p\n", dataset);
    printf("it has error..\n");
    exit(1);
  }
  else {
    dataset[cur_idx] = x;
  }
  if(cur_idx != 0){
    res = (double)count / cur_idx;
  }
  */
  count = exponentialSearch(dataset, cur_idx, x);
  
  if(cur_idx != 0){
    res = (double)count / cur_idx;
  }
  return res;
}

static int is_large(double cdf)
{
  return MAGIC <= cdf;
}
