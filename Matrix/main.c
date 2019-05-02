#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

int matrix1[50][50];
int matrix2[50][50];
int result[50][50] = {0};
int result2[50][50] = {0};

struct val{
    int i;
    int j;
    int b;
    int d;
};

 void *sum(void *param) {
    struct val *c = param;
    int sum=0;
    for (int n = 0; n <= c->b; n++) {
        sum += matrix1[c->i][n] * matrix2[n][c->j];
    }
    result[c->i][c->j] = sum;
 }

 void *row(void *param) {
    struct val *c = param;
    int sum=0;
    for (int x = 0; x <= c->d ;++x) {
        for (int n = 0; n <= c->b; n++) {
            sum += matrix1[c->i][n] * matrix2[n][x];
        }
        result2[c->i][x] = sum;
        sum =0;
    }

}

int main(void)
{
    FILE *ifp, *ofp;
    char outputFilename[] = "out.txt";
    int i,j,a,b,c,d,thread;
    struct timeval bef,aft,bef1,aft1;


    ifp = fopen("input.txt", "r");

    if (ifp == NULL)
    {
        fprintf(stderr, "Can't open input file in.list!\n");
        exit(1);
    }

    fscanf(ifp, "%d %d", &a, &b);  //getting size of matrix 1
    printf(" matrix size is : %d x %d\n",a,b);


    for(i=0; i<a; i++)
    {
        for(j=0; j<b; j++)
        {
            if(!feof(ifp))
                fscanf(ifp, "%d", &matrix1[i][j]); //getting values of matrix 1
            else
                break;
        }
    }
    for(i=0; i<a; i++)
    {
        for(j=0; j<b; j++)
        {
           printf("%d \t",matrix1[i][j]);
        }
        printf("\n");
    }


    fscanf(ifp, "%d %d", &c, &d); //getting size of matrix 2
    printf("\n \n matrix 2 size is : %d x %d\n",c,d);


    for(i=0; i<c; i++)
    {
        for(j=0; j<d; j++)
        {
            if(!feof(ifp))
                fscanf(ifp, "%d", &matrix2[i][j]);
            else
                break;
        }
    }
    fclose(ifp);

    for(i=0; i<c; i++)
    {
        for(j=0; j<d; j++)
        {
           printf("%d \t",matrix2[i][j]);
        }
        printf("\n");
    }

    if (b != c){
        printf("Matricies cannot be multiplied : incompatible size \n");
        exit(1);
    }


    ofp = fopen(outputFilename, "w");

    if (ofp == NULL)
    {
        fprintf(stderr, "Can't open output file %s!\n", outputFilename);
        exit(1);
    }


// computation of each element in a thread
gettimeofday(&bef,NULL);
    for (i = 0; i <= a;i++) {
        for ( j = 0; j <= d; ++j) {
            struct val *bynum = (struct val *)malloc(sizeof(struct val));
            bynum ->i = i;
            bynum ->j = j;
            bynum ->b = b;

                pthread_t t;
                thread= pthread_create(&t,NULL,sum,bynum);
                if (thread !=0){
                    printf("thread failed in sum of array");
                    exit(1);
                }
                pthread_join(t,NULL);

        }

    }
    gettimeofday(&aft,NULL);

    printf("\n Result : \n ");
    fprintf(ofp,"\n Result of element computation : \n");
    for(i=0; i<a; i++)
    {
        for(j=0; j<d; j++)
        {
            printf("%d \t",result[i][j]);
            fprintf(ofp,"%d \t",result[i][j]);
        }
        printf("\n");
        fprintf(ofp,"\n");
    }
    long time = (aft.tv_sec-bef.tv_sec)+(aft.tv_usec-bef.tv_usec);
    printf("The computuion of each element took %ld time \n",time);
    fprintf(ofp,"\n The computuion of each element took %ld time \n",time);


    /////Multiplication by row

    gettimeofday(&bef1,NULL);
    for (i = 0; i <= a;i++) {

            struct val *rowm = (struct val *)malloc(sizeof(struct val));
            rowm ->i = i;
            rowm ->b = b;
            rowm->d = d;
            pthread_t t;
            thread= pthread_create(&t,NULL,row,rowm);
            if (thread !=0){
                printf("thread failed in row mul of array");
                exit(1);
            }
            pthread_join(t,NULL);
    }
    gettimeofday(&aft1,NULL);

    printf("\n Result 2: \n ");
    fprintf(ofp,"\n Result of row computation : \n");
    for(i=0; i<a; i++)
    {
        for(j=0; j<d; j++)
        {
            printf("%d \t",result2[i][j]);
            fprintf(ofp,"%d \t",result2[i][j]);
        }
        printf("\n");
        fprintf(ofp,"\n");
    }
    long time1 = (aft1.tv_sec-bef1.tv_sec)+(aft1.tv_usec-bef1.tv_usec);
    printf("The computuion of each row took %ld time \n",time1);
    fprintf(ofp,"\n The computuion of each row took %ld time \n",time1);


    fclose(ofp);

    return 0;
}
