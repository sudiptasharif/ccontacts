#include <pcre.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// PCRE: Perl Compatible Regular Expression

int main(int argc, char *argv[])
{
    pcre *reCompiled = NULL;
    pcre_extra *pcreExtra = NULL;
    int pcreExecRet = -1;
    int subStrVec[30];
    memset(subStrVec, -1, 30 * sizeof(subStrVec[0]));
    const char *pcreErrorStr = NULL;
    int pcreErrorOffset = -1;
    char *aStrRegex = NULL;
    char *aLineToMatch = NULL;
    const char *psubStrMatchStr = NULL;
    int j;

    aStrRegex = "^([\\+]?[1]?[\\s]?[-\\.\\(]?(\\d{3})[-\\.\\)\\s]?)?(\\d{3})[-\\.\\s]?(\\d{4})$";
    //aStrRegex = "^([\\+]?[1]?[\\s]?[-\\.\\(]?([0-9]{3})[-\\.\\)\\s]?)?([0-9]{3})[-\\.\\s]?([0-9]{4})$";
    //aStrRegex = "^([\\d]{5}[\\s\\.]?)?[\\d]{5}$";
    printf("Regex to use: %s\n", aStrRegex);

    aLineToMatch = argv[1];

    // First the regex string must be compiled/
    reCompiled = pcre_compile(aStrRegex, 0, &pcreErrorStr, &pcreErrorOffset, NULL);

    if (reCompiled == NULL)
    {
        printf("ERROR: Could not compile '%s': %s\n", aStrRegex, pcreErrorStr);
        exit(1);
    }

    // Optimize the regex
    pcreExtra = pcre_study(reCompiled, 0, &pcreErrorStr);

    if (pcreErrorStr != NULL)
    {
        printf("ERROR: Could not study '%s': %s\n", aStrRegex, pcreErrorStr);
        exit(1);
    }

    pcreExecRet = pcre_exec(
        reCompiled,
        pcreExtra,
        aLineToMatch,
        strlen(aLineToMatch),
        0,
        0,
        subStrVec,
        30);

    if (pcreExecRet < 0)
    {
        switch (pcreExecRet)
        {
        case PCRE_ERROR_NOMATCH:
            printf("String did not match the pattern\n");
            break;
        case PCRE_ERROR_NULL:
            printf("Something was null\n");
            break;
        case PCRE_ERROR_BADOPTION:
            printf("A bad option was passed\n");
            break;
        case PCRE_ERROR_BADMAGIC:
            printf("Magic number bad (compiled re corrupt?)\n");
            break;
        case PCRE_ERROR_UNKNOWN_NODE:
            printf("Something kooky in the compiled re\n");
            break;
        case PCRE_ERROR_NOMEMORY:
            printf("Ran out of memory\n");
            break;
        default:
            printf("Unknown error\n");
            break;
        }
    }
    else
    {
        printf("Result: We have a match!\n");
        pcre_free_substring(psubStrMatchStr);
    }

    printf("\n");
    pcre_free(reCompiled);
    if (pcreExtra != NULL)
    {
#ifdef PCRE_CONFIG_JIT
        pcre_free_study(pcreExtra);
#else
        pcre_free(pcreExtra);
#endif

        return 0;
    }
}