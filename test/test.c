#include "masterHeader.h"

char* trimStr(const char *str);

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        char *testStr = argv[1];
        int len = strlen(testStr);
        printf("'%s' len = %d\n", testStr, len);

        char *trmdStr = trimStr(testStr);
        printf("'%s' is trimmed.\n", trmdStr);
        printf("'%s' is len = %ld\n", trmdStr, strlen(trmdStr));
        if (trimStr != NULL)
        {
            free(trmdStr);
        }
    }
    else
    {
        printf("Invalid input.\n");
    }
    return 0;
}

char* trimStr(const char *str)
{
    int len = strlen(str);
    char *trmdStr = (char *) malloc((len+1)*sizeof(char));
    memset(trmdStr, NULL_TERMINATOR, strlen(trmdStr)*sizeof(trmdStr[0]));

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
    i = len-1;
    while (str[i] == SPACE_CHAR)
    {
        trailingSpaceStartIndex--;
        i--;
    }

    int j = 0;
    for(i = leadingSpaceEndIndex; i < trailingSpaceStartIndex; i++)
    {
        trmdStr[j] = str[i];
        j++;
    }

    //printf("'%s' len = %d\n", str, len);
    //printf("'%s' leading space end index = %d\n", str, leadingSpaceEndIndex);
    //printf("'%s' trailing space start index = %d\n", str, trailingSpaceStartIndex);
    return trmdStr; // must free this where returned called.
}

