/* MDH WCET BENCHMARK SUITE. File version $Id: ns.c 7800 2018-01-19 12:43:12Z lkg02 $ */

/* Test of deeply nested loops and non-local exits */


/*-------------------------------------------------- *
 * LOG:
 *  $Log: ns.c,v $
 *  Revision 1.3  2005/11/11 10:31:47  ael01
 *  updated
 *
 *  Revision 1.2  2005/04/04 11:34:58  csg
 *  again
 *
 *  Revision 1.1  2005/03/29 11:28:43  jgn
 *  New file.
 *
 *  Revision 1.8  2001/05/07 10:05:37  ijae
 *  no message
 *
 *  Revision 1.7  2001/04/25 12:48:15  ijae
 *  Corrected trace names.
 *
 *  Revision 1.6  2001/04/25 12:17:47  ijae
 *  no message
 *
 *  Revision 1.5  2001/04/25 12:11:31  ijae
 *  Compilable for V850
 *
 *  Revision 1.4  2001/04/25 12:09:55  ijae
 *  Now in target mode.
 *
 *  Revision 1.3  2001/04/25 12:06:36  ijae
 *  Now 4D array. Compiles & runs on PC
 *
 *  Revision 1.2  2001/04/25 11:59:38  ijae
 *  A bit more comments.
 *
 *-------------------------------------------------- */


/* -------------------------------------------------- *
 *  Define TEST to check the # iterations in inner loop,
 *  and that the right value is found and returned
 * -------------------------------------------------- */


/* --------------------------------------------------
 *  Array of keys and values, 4-dimensional just
 *  for the fun of it.
 * -------------------------------------------------- */
int keys[5][5][5][5] =
{
  {
    {
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0}
    },
    {
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0}
    },
    {
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0}
    },
    {
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0}
    },
    {
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0},
      {0,0,0,0,0}
    }
  },
  {
    {
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1}
    },
    {
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1}
    },
    {
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1}
    },
    {
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1}
    },
    {
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1},
      {1,1,1,1,1}
    }
  },
  {
    {
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2}
    },
    {
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2}
    },
    {
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2}
    },
    {
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2}
    },
    {
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2}
    }
  },
  {
    {
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3}
    },
    {
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3}
    },
    {
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3}
    },
    {
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3}
    },
    {
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3},
      {3,3,3,3,3}
    }
  },
  {
    {
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4}
    },
    {
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4}
    },
    {
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4}
    },
    {
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4}
    },
    {
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,4},
      {4,4,4,4,
#ifdef FIND_TARGET
       400
#else
       401                      /* not searched for */
#endif
      }
    }
  }
};



int answer[5][5][5][5] =
{
  {
    {
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123}
    },
    {
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123}
    },
    {
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123}
    },
    {
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123}
    },
    {
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123},
      {123,123,123,123,123}
    }
  },
  {
    {
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234}
    },
    {
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234}
    },
    {
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234}
    },
    {
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234}
    },
    {
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234},
      {234,234,234,234,234}
    }
  },
  {
    {
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345}
    },
    {
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345}
    },
    {
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345}
    },
    {
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345}
    },
    {
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345},
      {345,345,345,345}
    }
  },
  {
    {
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456}
    },
    {
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456}
    },
    {
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456}
    },
    {
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456}
    },
    {
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456},
      {456,456,456,456,456}
    }
  },
  {
    {
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567}
    },
    {
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567}
    },
    {
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567}
    },
    {
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567}
    },
    {
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,567},
      {567,567,567,567,1111}
    }
  }
};


int foo(int x)
{
#ifdef TEST
  int c = 0;                    /* counter for innerloop */
#endif
  int i,j,k,l;

  for(i=0; i<5; i++)
    for(j=0 ; j<5 ; j++)
      for(k=0 ; k<5 ; k++)
        for(l=0 ; l<5 ; l++)
        {
#ifdef TEST
          c++;
#endif
          if( keys[i][j][k][l] == x )
            {
#ifdef TEST
              printf("   %d\n",c);
#endif
              return answer[i][j][k][l] + keys[i][j][k][l];
            }
        }
  return -1;
}


int main(void)
{
#ifdef TEST
  printf("result=%d\n",foo(400));
#else
  foo(400);
#endif
  return 0;
}
