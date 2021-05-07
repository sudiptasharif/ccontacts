#include "masterHeader.h"

int privilegedUser()
{
    int result = 0;
    uid_t caller_uid = getuid();
    uid_t owner_uid = geteuid();

    setresuid(owner_uid, -1, caller_uid);
    if ((access(DB_NAME, R_OK) == 0) && (access(DB_NAME, W_OK) == 0) && (access(AUDIT_FILE, R_OK) == 0) && (access(AUDIT_FILE, W_OK) == 0) && (access(LOG_FILE, R_OK) == 0) && (access(LOG_FILE, W_OK) == 0))
    {
        result = 1;
    }
    setresuid(caller_uid, -1, owner_uid);
    return result;
}

int validName(char *name)
{
    int result = INVALID_INPUT;
    // Check the name against the following regular expressions
    char *nameStrRegexs[] = {"^[A-Z][a-z]+([,]?[\\s][A-Z][a-z]+)?$",
                            "^[A-Z][a-z]+[,][\\s][A-Z][a-z]+[\\s][A-Z][a-z]+$",
                            "^[A-Z]['][A-Z][a-z]+[,][\\s][A-Z][a-z]+[\\s][A-Z][\\.]$",
                            "^[A-Z][a-z]+[\\s][A-Z]['][A-Z][a-z]+-[A-Z][a-z]+$",
                            NULL};
    char **aStrRegex = NULL;
    for (aStrRegex = nameStrRegexs; *aStrRegex != NULL; aStrRegex++)
    {
        if (evalutateRegex(name, *aStrRegex) == REGEX_MATCH_FOUND)
        {
            result = VALID_INPUT;
        }
    }
    return result;
}

int validNumber(char *number)
{
    int result = INVALID_INPUT;
    // Check the number against the following regular expressions
    char *numStrRegexs[] = {"^([\\+]?[1]?[\\s]?[-\\.\\(]?(\\d{3})[-\\.\\)\\s]?)?(\\d{3})[-\\.\\s]?(\\d{4})$",
                            "^([\\+]|45[\\s\\.])?[\\d]{2}[\\s\\.]?[\\d]{2}[\\s\\.]?[\\d]{2}[\\s\\.]?[\\d]{2}$",
                            "^([\\+]|45[\\s\\.])?[\\d]{4}[\\s\\.]?[\\d]{4}$",
                            "^([\\d]{5}[\\s\\.]?)?[\\d]{5}$",
                            "^[\\d]{3}[\\s]{1}[\\d]?[\\s]?[\\d]{3}[\\s]{1}[\\d]{3}[\\s]{1}[\\d]{4}$",
                            "^\\+[\\d]{2}[\\s]?[\\(\\.-]?[\\d]{2}[-\\)\\.]?[\\s]?[\\d]{3}[-\\.\\s]?[\\d]{4}$",
                            "^[\\d]{1}[-\\.\\s]?[\\d]{3}[-\\.\\s]?[\\d]{3}[-\\.\\s]?[\\d]{4}$",
                            "^\\+[\\d]{1}[\\s]?[\\.\\-\\(]?[\\d]{3}[\\.\\-\\)]?[\\s]?[\\d]{3}[\\.\\-\\s]?[\\d]{4}$",
                            NULL};
    char **aStrRegex = NULL;
    for (aStrRegex = numStrRegexs; *aStrRegex != NULL; aStrRegex++)
    {
        if (evalutateRegex(number, *aStrRegex) == REGEX_MATCH_FOUND)
        {
            result = VALID_INPUT;
        }
    }
    return result;
}

/**
 * Credit: I reused code found in the following URL
 * https://www.mitchr.me/SS/exampleCode/AUPG/pcre_example.c.html
*/
int evalutateRegex(char *input, char *aStrRegex)
{
    int result = REGEX_MATCH_NOT_FOUND;
    pcre *reCompiled = NULL;
    pcre_extra *pcreExtra = NULL;
    int pcreExecRet = -1;
    int subStrVec[30];
    memset(subStrVec, -1, 30 * sizeof(subStrVec[0]));
    const char *pcreErrorStr = NULL;
    int pcreErrorOffset = -1;
    const char *psubStrMatchStr = NULL;
    logMsg(PBSECURITY_C_FILE, "evalutateRegex: Regex to use", aStrRegex);

    // First, the regex string must be compiled.
    reCompiled = pcre_compile(aStrRegex, PCRE_ANCHORED, &pcreErrorStr, &pcreErrorOffset, NULL);
    if (reCompiled == NULL)
    {
        logMsg(PBSECURITY_C_FILE, "evalutateRegex: Could not compile Regex", pcreErrorStr);
        result = REGEX_COMPILE_FAILED;
    }
    else
    {
        // Optimize the regex
        pcreExtra = pcre_study(reCompiled, 0, &pcreErrorStr);
        if (pcreErrorStr != NULL)
        {
            logMsg(PBSECURITY_C_FILE, "evalutateRegex: Could not study Regex", pcreErrorStr);
            result = REGEX_STUDY_FAILED;
        }
        else
        {
            /* Try to find the regex in input, and report results. */
            pcreExecRet = pcre_exec(reCompiled,
                                    pcreExtra,
                                    input,
                                    strlen(input), // length of string
                                    0,             // Start looking at this point
                                    PCRE_ANCHORED, // OPTIONS
                                    subStrVec,
                                    30); // Length of subStrVec

            if (pcreExecRet < 0)
            { // Something bad happened..
                switch (pcreExecRet)
                {
                case PCRE_ERROR_NOMATCH:
                    logMsg(PBSECURITY_C_FILE, "evalutateRegex", "String did not match the pattern");
                    break;
                case PCRE_ERROR_NULL:
                    logMsg(PBSECURITY_C_FILE, "evalutateRegex", "Something was null");
                    break;
                case PCRE_ERROR_BADOPTION:
                    logMsg(PBSECURITY_C_FILE, "evalutateRegex", "A bad option was passed");
                    break;
                case PCRE_ERROR_BADMAGIC:
                    logMsg(PBSECURITY_C_FILE, "evalutateRegex", "Magic number bad (compiled re corrupt?)");
                    break;
                case PCRE_ERROR_UNKNOWN_NODE:
                    logMsg(PBSECURITY_C_FILE, "evalutateRegex", "Something kooky in the compiled re");
                    break;
                case PCRE_ERROR_NOMEMORY:
                    logMsg(PBSECURITY_C_FILE, "evalutateRegex", "Ran out of memory");
                    break;
                default:
                    logMsg(PBSECURITY_C_FILE, "evalutateRegex", "Unknown error");
                    break;
                } /* end switch */
            }
            else
            {
                result = REGEX_MATCH_FOUND;
                logMsg(PBSECURITY_C_FILE, "evalutateRegex", "Match Found!");
            }
        }
    }
    // Free up the substring
    if (psubStrMatchStr != NULL)
        pcre_free_substring(psubStrMatchStr);

    // Free up the regular expression.
    if (reCompiled != NULL)
        pcre_free(reCompiled);

    // Free up the EXTRA PCRE value (may be NULL at this point)
    if (pcreExtra != NULL)
    {
#ifdef PCRE_CONFIG_JIT
        pcre_free_study(pcreExtra);
#else
        pcre_free(pcreExtra);
#endif
    }

    return result;
}

char *trimStr(char *str)
{
    int len = strlen(str);
    char *trmdStr = (char *)malloc((len + 1) * sizeof(char));
    memset(trmdStr, NULL_TERMINATOR, strlen(trmdStr) * sizeof(trmdStr[0]));

    int i = 0;
    int leadingSpaceEndIndex = 0;
    int trailingSpaceStartIndex = len;

    // first, find the position where the leading space ends
    while (str[i] == SPACE_CHAR)
    {
        leadingSpaceEndIndex++;
        i++;
    }
    // now, find the position where the trailing space begins
    i = len - 1;
    while (str[i] == SPACE_CHAR)
    {
        trailingSpaceStartIndex--;
        i--;
    }

    int j = 0;
    for (i = leadingSpaceEndIndex; i < trailingSpaceStartIndex; i++)
    {
        trmdStr[j] = str[i];
        j++;
    }
    return trmdStr; // must free this where returned called.
}

char* extractNumbers(char *str)
{
    int len = strlen(str);
    char *numbers = (char *) malloc((len+1)*sizeof(char));
    memset(numbers, NULL_TERMINATOR, strlen(numbers)*sizeof(numbers[0]));

    int numIndex = 0;
    for (int i = 0; i < len; i++)
    {
        if(str[i] == '0' || str[i] == '1' || str[i] == '2' || str[i] == '3' || str[i] == '4' || str[i] == '5' || str[i] == '6' || str[i] == '7' || str[i] == '8' || str[i] == '9')
        {
            numbers[numIndex] = str[i];
            numIndex++;
        }
    }
    return numbers; // Must free this where everl called.
}

// Only used during testing. Keep for maintenance
// int main(int argc, char *argv[])
// {
//     // int result = INVALID_INPUT;
//     // char *msg = NULL;
//     // char *inputType = NULL;
//     // if (argc == 3)
//     // {
//     //     if (strncmp(argv[1], "num", 3) == 0)
//     //     {
//     //         result = validNumber(argv[2]);
//     //         inputType = "Number";
//     //     }
//     //     else
//     //     {
//     //         result = validName(argv[2]);
//     //         inputType = "Name";
//     //     }

//     //     msg = (result == VALID_INPUT) ? "Valid" : "Invalid";
//     //     printf("%s %s = %s\n", inputType, argv[2], msg);
//     // }
//     // else
//     // {
//     //     printf("Program stopped. Invalid input.\n");
//     // }
//     if (argc == 2)
//     {
//         char *numbers = extractNumbers(argv[1]);
//         printf("Formatted Number: %s\n", argv[1]);
//         printf("Unformatted Number: %s\n", numbers);
//         free(numbers);
//     }
//     else
//     {
//         printf("Invalid Input\n");
//     }

//     return 0;
// }

