


#define AMAX 10			/* Maximum (square) array size */

#define CACHESIM 1		/* Set to 1 if simulating Cache */

#include <stdio.h>
#include <stdlib.h>

/*	memory management, code density, Cache emulation - statistics generation */
/*	Generated for CSC 315 Lab 5 */


/* This function gets called with each "read" reference to memory */



int ***cache;
int assoc, cache_size, hit, miss;
float read, write;

void cache_write(int *mp)
{
	int i, j;
	
	for (i = 0; i < assoc; i++) {
		for (j = 0; j < (cache_size/assoc); j++) {
			if (cache[i][j] == 0) {
				//printf("storring in branch: %d, index: %d\n", i, j);
				cache[i][j] = mp;

				return;
			}
		}
	}

	i = rand()%assoc;
	j = rand()%(cache_size/assoc);
	//printf("overwriting at branch: %d, index: %d\n", i, j);
	cache[i][j] = mp;
}

mem_read(int *mp)
{
	int i;
	int j;

	read++;
	//printf("Memory read from location %p\n", mp);

	for (i = 0; i < assoc; i++) {
		for (j = 0; j < (cache_size/assoc); j++) {
			if (cache[i][j] == mp) {
				//printf("HIT\n");
				hit++;

				return;
			}
		}
	}

	//printf("MISS\n");
	miss++;
	cache_write(mp);
}


/* This function gets called with each "write" reference to memory */

mem_write(int *mp)
{
	int i;
	int j;

	write++;
	//printf("Memory write to location %p\n", mp);

	for (i = 0; i < assoc; i++) {
		for (j = 0; j < (cache_size/assoc); j++) {
			if (cache[i][j] == mp) {
				//printf("HIT\n");
				hit++;

				return;
			}
		}
	}
	
	//printf("MISS\n");
	miss++;
	cache_write(mp);
}


/* Statically define the arrays a, b, and mult, where mult will become the cross product of a and b, i.e., a x b. */

static int a[AMAX][AMAX], b[AMAX][AMAX], mult[AMAX][AMAX];





void matmul( r1, c1, c2 )
   {
   int i,j,k;
   int *mp1, *mp2, *mp3;



/* Initializing elements of matrix mult to 0.*/
    for(i=0; i<r1; ++i)
     for(j=0; j<c2; ++j)
       {
       mult[i][j]=0;
       }

/* Multiplying matrix a and b and storing in array mult. */

    for(i=0; i<r1; ++i)
     for(j=0; j<c2; ++j)
      for(k=0; k<c1; ++k)
        {

#if CACHESIM		/* "Hooks" to measure memory references - enabled if CACHESIM  */

        mp1 = &mult[i][j];
	mp2 = &a[i][k];
	mp3 = &b[k][j];   
	mem_read(mp1);
 	mem_read(mp2);
	mem_read(mp3);
	mem_write(mp1); 
#endif

        mult[i][j]+=a[i][k]*b[k][j];

        }
   }





int main()
    {
    int r1, c1, r2, c2, i, j, k;
    int *mp1, *mp2, *mp3;
	for (r1=c1=r2=c2=2; r1 <= 10; r1+=8, c1+=8, r2+=8, c2+=8) {
		for (assoc = 1; assoc <=4; assoc = assoc * 2) {
			for (cache_size = 16; cache_size <= 256; cache_size = cache_size * 16) { 
	hit = 0;
	miss = 0;
	read = 0;
	write = 0;
	printf("matrix size: %d x %d\n", r1, c1);
	printf("associativity: %d\n", assoc);
	printf("cache size: %d\n", cache_size);
	/*
    printf("Size of pointer is: %d\n\n", sizeof(mp1));

    printf("Enter rows and column for first matrix: ");
    scanf("%d%d", &r1, &c1);
    printf("Enter rows and column for second matrix: ");
    scanf("%d%d",&r2, &c2);
	printf("Enter associativity (1, 2, or 4): ");
	scanf("%d", &assoc);
	printf("Enter cache size: ");
	scanf("%d", &cache_size);
	*/

	cache = calloc(assoc, sizeof(int **));
	for (i = 0; i < assoc; i++) {
		cache[i] = calloc(cache_size/assoc, sizeof(int *));
	}

/* If column of first matrix in not equal to row of second matrix, asking user to enter the size of matrix again. */
    /*while (c1 != r2)
        {
        printf("Error! column of first matrix not equal to row of second.\n");
        printf("Enter rows and column for first matrix: ");
        scanf("%d%d", &r1, &c1);
        printf("Enter rows and column for second matrix: ");
        scanf("%d%d",&r2, &c2);
        }
	*/
/* Storing elements of first matrix. */

    /*printf("\nEnter elements of matrix 1:\n");*/
    for(i=0; i<r1; ++i)
    for(j=0; j<c1; ++j)
          {
//        printf("Enter elements a%d%d: ",i+1,j+1);
//        scanf("%d",&a[i][j]);
	a[i][j] = i+j; // build sample data

          }

/* Storing elements of second matrix. */
    /*printf("\nEnter elements of matrix 2:\n");*/
    for(i=0; i<r2; ++i)
    for(j=0; j<c2; ++j)
    {
//        printf("Enter elements b%d%d: ",i+1,j+1);
//        scanf("%d",&b[i][j]);

         b[i][j] = 10 + i + j;
    }


   matmul(r1, c1, c2);  	/* Invoke matrix multiply function */	


/* Displaying the multiplication of two matrix. */
    /*printf("\nOutput Matrix:\n");
    for(i=0; i<r1; ++i)
    for(j=0; j<c2; ++j)
    {
        printf("%d  ",mult[i][j]);
        if(j==c2-1)
            printf("\n\n");
    }*/

	printf("read to write access ratio: %f\n", read/write);
	printf("hit rate: %d\n\n", (hit * 100)/(hit + miss));
			}
		}
	}
    return 0;
}

