// find the ORBS printf and split the code into 
//   prefix; function with ORBS printf; postfix

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define MAX_DIST 100

static void die(char *msg)
{
  perror(msg);
  exit(-1);
}


char *suck_in(char *filename)
{
  FILE *fin = fopen(filename, "r");
  if (fin == NULL) die("fopen");
  if (fseek(fin, 0, SEEK_END) < 0) die("fseek");
  int length = ftell(fin);
  if (length < 0) die("ftell");
  if (fseek(fin, 0, SEEK_SET) < 0) die("fseek");
  char *buf = (char*) malloc(length + 1);
  if (fread(buf, 1, length, fin) < 0) die("fread");
  buf[length] = '\0';
  return buf;
}


// find the offset from the between of rodata to 'ORBS:%'
static int find_offset(char *rodata, char *orbs)
{
  char *cp = orbs;
  int cnt = 0;

  // in theory rodata check is unnecessary :)
  while (rodata < cp && *cp != '"')
    cp--;

  for(cp++; cp != orbs; )
  {
    cnt++;
    if (*cp++ == '\\')
      cp += 2;
    // if(cnt > 40) break;
  }

  return cnt;
}


static void unique_check(char *first_occurrence, char *instr)
{
  char *cp = strstr(first_occurrence, instr);
  if (cp != NULL)
    printf("** Warning multiple copies of %s\n", instr);
}


/* expected pattern is
 *  i32.const 1034
 *  local.get <n>
 *  i32.const <n>
 *  i32.add
 *  call $printf
 */
static char *find_func(char *src, char *instr)
{
  char *cp = strstr(src, instr);
  if (cp == NULL) 
  {
    printf("failed to find %s ... bye\n", instr);
    exit(-1);
  }

  char *print = strstr(cp, "call $printf");
  if (cp == NULL) 
  {
    printf("failed to find printf following %s ... bye\n", instr);
    exit(-1);
  }

  unique_check(print, instr);

  if (print - cp > MAX_DIST)
    printf("** Warning might want to update find_func distance = %ld > %d == MAX\n", print-cp, MAX_DIST);

   // printf("%p %p %ld\n", cp, print, print - cp);
   // printf("%100.100s", cp);

  return print;
}


static char *func_start(char *src, char *orbs_print)
{
  char *cp;
  for(cp = orbs_print; src < cp; cp--)
    if (*cp == '(' && strncmp("(func", cp, 5) == 0)
      break;

  if (cp == src)
    printf("** Warning failed to find start of function :(\n");

  return cp;
}


static char *func_end(char *fs)
{
  char *cp = fs;
  int cnt = 0;
  
  for(cp = fs; *cp != '\0'; cp++)
  {
    if (*cp == '(')
      cnt++;
    else if (*cp == ')')
      cnt--;
    if (cnt == 0)
      return cp;
  }

  printf("** Warning failed to find end of function :(\n");
  return fs;

}



static void write_src(char *name, char *part, char *start, char *end)
{
  char buff[1024];
  sprintf(buff, "%s%s", name, part);
  FILE *fout = fopen(buff, "w");
  if (fout == NULL) die("fopen");

  if (end == NULL)
    fputs(start, fout);
  else
    fwrite(start, 1, end-start, fout);

  fclose(fout);
}



int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s filename\n", argv[0]);
    exit(-1);
  }


  char *src = suck_in(argv[1]);

  char *rodata = strstr(src, ".rodata");
  if (rodata == NULL) die("can't find .rodata");
  char *orbs = strstr(rodata, "\\0aORBS:%");
  if (orbs == NULL) die("can't find ORBS:%%");
  int offset = 1024 + find_offset(rodata, orbs);

  printf("offset = %d\n", offset);

  char instr[100];
  sprintf(instr, "i32.const %d", offset);

  printf("instr = %s\n", instr);

  char *orbs_print = find_func(src, instr);

  char *fs = func_start(src, orbs_print);
  char *fe = func_end(fs);

  // printf("fs = %20.20s\n", fs);
  // printf("fe = %20.20s\n", fe-18);
  // printf("lengths %ld %ld %ld\n", fs-src, fe-fs, strlen(fe));

  write_src(argv[1], ".prefix", src, fs);
  write_src(argv[1], "", fs, fe+1);
  write_src(argv[1], ".postfix", fe+1, NULL);
}
