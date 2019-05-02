#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

struct Sort {
    int *data;
    int low;
    int high;
};


void merge(int arr[],int l,int m,int h)
{
    int arr1[50],arr2[50];  // Two temporary arrays to hold the two arrays to be merged
    int n1,n2,i,j,k;
    n1=m-l+1;
    n2=h-m;

    for(i=0; i<n1; i++)
        arr1[i]=arr[l+i];
    for(j=0; j<n2; j++)
        arr2[j]=arr[m+j+1];

    arr1[i]=9999;  // To mark the end of each temporary array
    arr2[j]=9999;

    i=0;
    j=0;
    for(k=l; k<=h; k++) { //process of combining two sorted arrays
        if(arr1[i]<=arr2[j])
            arr[k]=arr1[i++];
        else
            arr[k]=arr2[j++];
    }

}

static void* mergeSort(void* args)
{

    struct Sort*  newargs = args;
    int *data = newargs->data;
    int r = newargs->high;
    int l = newargs->low;

    pthread_t thread1 , thread2;
    int ans1, ans2;

    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = (r+l)/2;
        struct Sort newArgs1[3] = {data, l, m};
        struct Sort newArgs2[3] = {data, m+1, r};
        ans1 = pthread_create ( &thread1 , NULL , mergeSort ,(void*)newArgs1);
        ans2 = pthread_create ( &thread2 , NULL , mergeSort ,(void*)newArgs2);
        if (ans1 !=0 || ans2!=0)
            exit(1);
        pthread_join(thread1,NULL);
        pthread_join(thread2,NULL);

        merge(data, l, m, r);

    }

}

int main(void)
{
    FILE *ifp, *ofp;
    int i,a;


    ifp = fopen("input.txt", "r");

    if (ifp == NULL)
    {
        fprintf(stderr, "Can't open input file !\n");
        exit(1);
    }

    fscanf(ifp, "%d", &a);  //getting size of array
    if(a<=1) {
        printf("array is already sorted");
        exit(1);
    }
    printf(" array size is : %d \n",a);
    int input[a];

    for(i=0; i<a; i++)
    {

        fscanf(ifp, "%d", &input[i]);

    }
    for (int i = 0; i <a ; i++) {
        printf(" %d \t",input[i]);
    }
    fclose(ifp);

    int ret;
    struct Sort args[3] ={input,0,a-1};
    pthread_t t1;
    ret =pthread_create(&t1,NULL,mergeSort,(void*)args);// sort the array
    if (ret ==0)
        pthread_join(t1,NULL);
    else
        printf("failed in Main thread");

    ofp = fopen("out.txt", "w");

    if (ofp == NULL)
    {
        fprintf(stderr, "Can't open output file \n");
        exit(1);
    }

    printf("\n Sorted array: \n");  // print sorted array
    fprintf(ofp,"Sorted array: \n");
    for(i=0; i<a; i++) {
        fprintf(ofp,"%d \t",input[i]);
        printf("%d \t", input[i]);
    }
    printf("\n");
    fclose(ofp);
    return 0;
}