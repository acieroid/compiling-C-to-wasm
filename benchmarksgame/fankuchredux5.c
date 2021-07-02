/*
// The Computer Language Benchmarks Game
// https://salsa.debian.org/benchmarksgame-team/benchmarksgame/
//
// Contributed by Jeremy Zerfas
// Based on the Ada program by Jonathan Parker and Georg Bauhaus which in turn
// was based on code by Dave Fladebo, Eckehard Berns, Heiner Marxen, Hongwei Xi,
// and The Anh Tran and also the Java program by Oleg Mazurov.

// This value controls how many blocks the workload is broken up into (as long
// as the value is less than or equal to the factorial of the argument to this
// program) in order to allow the blocks to be processed in parallel if
// possible. PREFERRED_NUMBER_OF_BLOCKS_TO_USE should be some number which
// divides evenly into all factorials larger than it. It should also be around
// 2-8 times the amount of threads you want to use in order to create enough
// blocks to more evenly distribute the workload amongst the threads.
// */
#define PREFERRED_NUMBER_OF_BLOCKS_TO_USE 12

/* #include <stdint.h>
#include <stdlib.h>
#include <stdio.h> */
typedef int intptr_t;
typedef long intmax_t;
typedef signed char int8_t;

/* intptr_t should be the native integer type on most sane systems. */
typedef intptr_t intnative_t;


int main(int argc, char ** argv){
   const intnative_t n=atoi(argv[1]);

   intnative_t factorial_Lookup_Table[n+1];
   factorial_Lookup_Table[0]=1;
   intnative_t i;
   for(i=0; ++i<=n;)
      factorial_Lookup_Table[i]=i*factorial_Lookup_Table[i-1];

   const intnative_t block_Size=factorial_Lookup_Table[n]/
     (factorial_Lookup_Table[n]<PREFERRED_NUMBER_OF_BLOCKS_TO_USE ?
     1 : PREFERRED_NUMBER_OF_BLOCKS_TO_USE);

   intnative_t maximum_Flip_Count=0, checksum=0;

   #pragma omp parallel for \
     reduction(max:maximum_Flip_Count) reduction(+:checksum)
   intnative_t initial_Permutation_Index_For_Block;
   for(initial_Permutation_Index_For_Block=0;
     initial_Permutation_Index_For_Block<factorial_Lookup_Table[n];
     initial_Permutation_Index_For_Block+=block_Size){

      intnative_t count[n];
      int8_t temp_Permutation[n], current_Permutation[n];


      count[0]=0;
      intnative_t i;
      for(i=0; i<n; ++i)
         current_Permutation[i]=i;
      intnative_t permutation_Index;
      for(i=n-1,
        permutation_Index=initial_Permutation_Index_For_Block; i>0; --i){
         const intnative_t d=permutation_Index/factorial_Lookup_Table[i];
         permutation_Index=permutation_Index%factorial_Lookup_Table[i];
         count[i]=d;

         intnative_t j;
         for(j=0; j<n; ++j)
            temp_Permutation[j]=current_Permutation[j];
         for(j=0; j<=i; ++j)
            current_Permutation[j]= j+d<=i ?
              temp_Permutation[j+d] : temp_Permutation[j+d-i-1];
      }


      const intnative_t last_Permutation_Index_In_Block=
        initial_Permutation_Index_For_Block+block_Size-1;
      for(permutation_Index=initial_Permutation_Index_For_Block; ;
        ++permutation_Index){

         if(current_Permutation[0]>0){

            intnative_t i;
            for(i=0; ++i<n;)
               temp_Permutation[i]=current_Permutation[i];

            intnative_t flip_Count=1;

            intnative_t first_Value;
            for(first_Value=current_Permutation[0];
              temp_Permutation[first_Value]>0; ++flip_Count){

               const int8_t new_First_Value=temp_Permutation[first_Value];
               temp_Permutation[first_Value]=first_Value;

               if(first_Value>2){
                  intnative_t low_Index=1, high_Index=first_Value-1;
                  do{
                     const int8_t temp=temp_Permutation[high_Index];
                     temp_Permutation[high_Index]=
                       temp_Permutation[low_Index];
                     temp_Permutation[low_Index]=temp;
                  }while(low_Index+++3<=high_Index-- && low_Index<16);
               }

               first_Value=new_First_Value;
            }


            if(permutation_Index%2==0)
               checksum+=flip_Count;
            else
               checksum-=flip_Count;

            if(flip_Count>maximum_Flip_Count)
               maximum_Flip_Count=flip_Count;
         }


         if(permutation_Index>=last_Permutation_Index_In_Block)
            break;

         int8_t first_Value=current_Permutation[1];
         current_Permutation[1]=current_Permutation[0];
         current_Permutation[0]=first_Value;
         intnative_t i;
         for(i=1; ++count[i]>i;){
            count[i++]=0;
            const int8_t new_First_Value=current_Permutation[0]=
              current_Permutation[1];

            intnative_t j;
            for(j=0; ++j<i;)
               current_Permutation[j]=current_Permutation[j+1];

            current_Permutation[i]=first_Value;
            first_Value=new_First_Value;
         }
      }
   }


   printf("%jd\nPfannkuchen(%jd) = %jd\n", (intmax_t)checksum, (intmax_t)n,
     (intmax_t)maximum_Flip_Count);

   return 0;
}
