#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cipher.h"

int main (int argc, char **argv)
{
  bool bEncrypt = true;
  bool isDeck = true;
  char* pOutput = NULL;
  int c = -1;

  while ((c = getopt(argc, argv, "dko:")) != -1)
  {
    switch (c)
    {
    case 'd':
      bEncrypt = false;
      break;
    case 'k':
      isDeck = false;
      break;
    case 'o':
      pOutput = optarg;
      break;
    case '?':
      if (optopt == 'o')
        fprintf (stderr, "Option -%o requires an argument.\n", optopt);
      else if (isprint (optopt))
        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
      return EXIT_FAILURE;
    default:
      abort();
    }
  }

  char* pInput = NULL;
  for (int index = optind; index < argc; index++)
  {
    if (pInput != NULL)
    {
      fprintf (stderr, "More than one input file provided.\n");
      return EXIT_FAILURE;
    }
    else
    {
       pInput = argv[index];
    }
  }

  if (pInput == NULL)
  {
    fprintf (stderr, "No input file provided.\n");
    return EXIT_FAILURE;
  }

  if (!run(pInput, bEncrypt, isDeck, pOutput))
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
