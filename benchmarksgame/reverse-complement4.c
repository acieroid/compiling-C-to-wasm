/* The Computer Language Benchmarks Game
 * https://salsa.debian.org/benchmarksgame-team/benchmarksgame/
 *
 * contributed by Bob W 
 */

/*
#include <stdio.h>
#include <stdlib.h>
*/
extern void *stdin;
extern void *stdout;
extern void *stderr;
#define stdin stdin
#define stdout stdout
#define stderr stderr
typedef unsigned int size_t;

#define JBFSIZE 82
#define QBFSIZE 5200
#define Z16     "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
#define V32     "\0TVGH\0\0CD\0\0M\0KN\0\0\0YSA\0BW\0R\0\0\0\0\0\0"
#define VALL    Z16 Z16 Z16 Z16 V32 V32 Z16 Z16 Z16 Z16 Z16 Z16 Z16 Z16

int errex(char *s, int n) {
  fprintf(stderr,"\n*** Error: %s [%d]!\n", s, n);
  return 1;
}

int main () {                    
  char *pj, *pq, *pr;            
  char *jjj = malloc(JBFSIZE);   
  char *qqq = malloc(QBFSIZE);   
  char *pqstop = qqq+QBFSIZE;    
  char xtab[256] = VALL;         

  if (!jjj || !qqq)
    return errex("Buffer allocation", !jjj + !qqq);
  pj = fgets(jjj,JBFSIZE,stdin);         
  if (!pj)
    return errex("No input data",0);
  if (*jjj != '>')
    return errex("1st char not '>'", 0);

  while (pj) {                           
    fputs(jjj, stdout);                  

    for (pq=qqq+1, pr=pqstop; ; pq++) {  
      pj = fgets(jjj, JBFSIZE, stdin);   
      if (!pj || (*jjj=='>'))  break;    
      if (pr <= (pq+61)) {               
        char *newstop = pqstop + 12777888;
        char *newptr  = realloc(qqq, newstop-qqq);
        if (!newptr)
          return errex("Out of memory", 0);
        if (newptr != qqq) {             
          size_t x = newptr-qqq;        
          pq+=x;  pr+=x;  qqq+=x;
          newstop+=x;  pqstop+=x;
        }
        pr = __builtin_memmove(newstop-(pqstop-pr), pr, pqstop-pr);
        pqstop = newstop;                
      }
      while (*pj) {                      
        char c = xtab[(unsigned char)(*pj++)];
        if (c)                           
          *(--pr) = c;
      }
    }

    for (pq = qqq; pr<pqstop; ) {        
      size_t x = (pqstop-pr)<60 ? pqstop-pr : 60;
      __builtin_memmove(pq,pr,x);        
      pr+=x;  pq+=x;  *(pq++) = 0xA;     
    }
    fwrite(qqq, 1, pq-qqq, stdout);      
  }
  return 0;
}
