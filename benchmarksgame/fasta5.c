#define MAXIMUM_LINE_WIDTH   60

/*#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>*/

typedef int intptr_t;
typedef unsigned int uint32_t;
#define NULL 0
extern void *stdout;
#define stdout stdout

typedef intptr_t intnative_t;

typedef struct{
   char letter;
   float probability;
} nucleotide_info;


static void repeat_And_Wrap_String(const char string_To_Repeat[],
  const intnative_t number_Of_Characters_To_Create){
   const intnative_t string_To_Repeat_Length=strlen(string_To_Repeat);

   char extended_String_To_Repeat[string_To_Repeat_Length+MAXIMUM_LINE_WIDTH];
   intnative_t column;
   for(column=0; column<string_To_Repeat_Length+MAXIMUM_LINE_WIDTH;
     column++)
      extended_String_To_Repeat[column]=
        string_To_Repeat[column%string_To_Repeat_Length];
   intnative_t offset=0;

   char line[MAXIMUM_LINE_WIDTH+1];
   line[MAXIMUM_LINE_WIDTH]='\n';

   intnative_t current_Number_Of_Characters_To_Create;
   for(current_Number_Of_Characters_To_Create=
     number_Of_Characters_To_Create;
     current_Number_Of_Characters_To_Create>0;){
      intnative_t line_Length=MAXIMUM_LINE_WIDTH;
      if(current_Number_Of_Characters_To_Create<MAXIMUM_LINE_WIDTH){
         line_Length=current_Number_Of_Characters_To_Create;
         line[line_Length]='\n';
      }

      memcpy(line, extended_String_To_Repeat+offset, line_Length);

      offset+=line_Length;
      if(offset>string_To_Repeat_Length)
         offset-=string_To_Repeat_Length;

      fwrite(line, line_Length+1, 1, stdout);
      current_Number_Of_Characters_To_Create-=line_Length;
   }
}


#define IM      139968
#define IA      3877
#define IC      29573
#define SEED   42
static float get_LCG_Pseudorandom_Number(const float max){
   static uint32_t seed=SEED;
   seed=(seed*IA + IC)%IM;
   return max/IM*seed;
}


static void generate_And_Wrap_Pseudorandom_DNA_Sequence(
  const nucleotide_info nucleotides_Information[],
  const intnative_t number_Of_Nucleotides,
  const intnative_t number_Of_Characters_To_Create){

   float cumulative_Probabilities[number_Of_Nucleotides],
     cumulative_Probability=0.0;
   intnative_t i;
   for(i=0; i<number_Of_Nucleotides; i++){
      cumulative_Probability+=nucleotides_Information[i].probability;
      cumulative_Probabilities[i]=cumulative_Probability*IM;
   }

   char line[MAXIMUM_LINE_WIDTH+1];
   line[MAXIMUM_LINE_WIDTH]='\n';
   intnative_t current_Number_Of_Characters_To_Create;
   for(current_Number_Of_Characters_To_Create=
     number_Of_Characters_To_Create;
     current_Number_Of_Characters_To_Create>0;){
      intnative_t line_Length=MAXIMUM_LINE_WIDTH;
      if(current_Number_Of_Characters_To_Create<MAXIMUM_LINE_WIDTH){
         line_Length=current_Number_Of_Characters_To_Create;
         line[line_Length]='\n';
      }

      intnative_t column;
      for(column=0; column<line_Length; column++){
         const float r=get_LCG_Pseudorandom_Number(IM);

         intnative_t count=0;
         intnative_t i;
         for(i=0; i<number_Of_Nucleotides; i++)
            if(cumulative_Probabilities[i]<=r)
               count++;

         line[column]=nucleotides_Information[count].letter;
      }

      fwrite(line, line_Length+1, 1, stdout);
      current_Number_Of_Characters_To_Create-=line_Length;
   }
}


int main(int argc, char ** argv){
   const intnative_t n=atoi(argv[1]);

   fputs(">ONE Homo sapiens alu\n", stdout);
   const char homo_Sapiens_Alu[]=
     "GGCCGGGCGCGGTGGCTCACGCCTGTAATCCCAGCACTTTGGGAGGCCGAGGCGGGCGGATCACCTGAGGTC"
     "AGGAGTTCGAGACCAGCCTGGCCAACATGGTGAAACCCCGTCTCTACTAAAAATACAAAAATTAGCCGGGCG"
     "TGGTGGCGCGCGCCTGTAATCCCAGCTACTCGGGAGGCTGAGGCAGGAGAATCGCTTGAACCCGGGAGGCGG"
     "AGGTTGCAGTGAGCCGAGATCGCGCCACTGCACTCCAGCCTGGGCGACAGAGCGAGACTCCGTCTCAAAAA";
   repeat_And_Wrap_String(homo_Sapiens_Alu, 2*n);

   fputs(">TWO IUB ambiguity codes\n", stdout);
   nucleotide_info iub_Nucleotides_Information[]={
     {'a', 0.27}, {'c', 0.12}, {'g', 0.12}, {'t', 0.27}, {'B', 0.02},
     {'D', 0.02}, {'H', 0.02}, {'K', 0.02}, {'M', 0.02}, {'N', 0.02},
     {'R', 0.02}, {'S', 0.02}, {'V', 0.02}, {'W', 0.02}, {'Y', 0.02}};
   generate_And_Wrap_Pseudorandom_DNA_Sequence(iub_Nucleotides_Information,
     sizeof(iub_Nucleotides_Information)/sizeof(nucleotide_info), 3*n);

   fputs(">THREE Homo sapiens frequency\n", stdout);
   nucleotide_info homo_Sapien_Nucleotides_Information[]={
     {'a', 0.3029549426680}, {'c', 0.1979883004921},
     {'g', 0.1975473066391}, {'t', 0.3015094502008}};
   generate_And_Wrap_Pseudorandom_DNA_Sequence(
     homo_Sapien_Nucleotides_Information,
     sizeof(homo_Sapien_Nucleotides_Information)/sizeof(nucleotide_info), 5*n);

   return 0;
}
