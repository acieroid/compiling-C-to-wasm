/* MDH WCET BENCHMARK SUITE. File version $Id: fac.c 7801 2018-01-19 13:33:32Z lkg02 $ */

/*
 * Changes: CS 2006/05/19: Changed loop bound from constant to variable.
 */

int fac (int n)
{
  if (n == 0)
     return 1;
  else
     return (n * fac (n-1));
}

int main (void)
{
  int i;
  int s = 0;
  volatile int n;

  n = 5;
  for (i = 0;  i <= n; i++)
      s += fac (i);

  printf("result: %d\n", s);
  return 0;
}

