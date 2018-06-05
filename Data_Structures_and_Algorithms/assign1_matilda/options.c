#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* options */
int Special = 0;    /* option -s      */
int Number = 0;     /* option -n XXXX */
char* Name = 0;     /* option -N YYYY */

static int processOptions(int,char **);
void Fatal(char *,...);

int
main(int argc,char **argv)
    {
    int argIndex;

    if (argc == 1) Fatal("%d arguments!\n",argc-1);

    argIndex = processOptions(argc,argv);

    printf("Special is %s\n", Special == 0? "false" : "true");
    printf("Number is %d\n", Number);
    printf("Name is %s\n", Name == 0? "missing" : Name);

    if (argIndex == argc)
        printf("No arguments\n");
    else
        {
        int i;
        printf("Remaining arguments:\n");
        for (i = argIndex; i < argc; ++i)
            printf("    %s\n",argv[i]);
        }

    return 0;
    }

void
Fatal(char *fmt, ...)
    {
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
    }

/* only -oXXX  or -o XXX options */

static int
processOptions(int argc, char **argv)
    {
    int argIndex;
    int argUsed;
    int separateArg;
    char *arg;

    argIndex = 1;

    while (argIndex < argc && *argv[argIndex] == '-')
        {
        /* check if stdin, represented by "-" is an argument */
        /* if so, the end of options has been reached */
        if (argv[argIndex][1] == '\0') return argIndex;

        separateArg = 0;
        argUsed = 0;

        if (argv[argIndex][2] == '\0')
            {
            arg = argv[argIndex+1];
            separateArg = 1;
            }
        else
            arg = argv[argIndex]+2;

        switch (argv[argIndex][1])
            {
            /*
             * when option has an argument, do this
             *
             *     examples are -m4096 or -m 4096
             *
             *     case 'm':
             *         MemorySize = atol(arg);
             *         argUsed = 1;
             *         break;
             *
             *
             * when option does not have an argument, do this
             *
             *     example is -a
             *
             *     case 'a':
             *         PrintActions = 1;
             *         break;
             */

            case 'n':
                Number = atoi(arg);
                argUsed = 1;
                break;
            case 's':
                Special = 1;
                break;
            case 'N':
                Name = strdup(arg);
                argUsed = 1;
                break;
            default:
                Fatal("option %s not understood\n",argv[argIndex]);
            }

        if (separateArg && argUsed)
            ++argIndex;

        ++argIndex;
        }

    return argIndex;
    }