#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // max_int
#define min(a,b) (((a) < (b)) ? (a) : (b))
int isDigit(char c)
{
    return '0' <= c && c <= '9';
}

int **A(char *str, int* matSize)
{
    const char s[2] = " ";
    char *token;

    token = strtok(str, s);
    token = strtok(NULL, s);
    *matSize = atoi(token);

    int **mat = (int**)malloc(*matSize * sizeof(int *));
    for (int i = 0; i < *matSize; i++)
    {
        mat[i] = (int*)malloc(*matSize * sizeof(int));
    }

    for (size_t i = 0; i < *matSize; i++)
        for (size_t j = 0; j < *matSize; j++)
            mat[i][j] = INT_MAX;

    int nFound = 0,
        iFound = 0,
        jFound = 0;
    int i, j, weight;
    while (token != NULL)
    {
        token = strtok(NULL, s);
        if (token == NULL)
            break;
        if (strcmp(token, "n") == 0)
        {
            nFound = 1;
            iFound = 0;
            jFound = 0;
        }
        else if (nFound && !iFound)
        {
            i = atoi(token);
            iFound = 1;
        }
        else if (iFound && !jFound)
        {
            j = atoi(token);
            jFound = 1;
        }
        else if (jFound)
        {
            weight = atoi(token);
            mat[i][j] = weight;
            jFound = 0;
        }
    }
    return mat;
}

int **B(char *str, int **mat, int* matSize)
{
    const char s[2] = " ";
    char *token;

    token = strtok(str, s);
    token = strtok(NULL, s);
    int i = atoi(token);
    if (i >= *matSize)
    {
        mat = (int**)realloc(mat, (i + 1) * sizeof(int *));
        for (size_t j = 0; j < i + 1; j++)
        {
            mat[j] = (int*)realloc(mat[j], (i + 1) * sizeof(int));
            for (size_t k = 0; k < i + 1; k++)
            {
                if (j < *matSize && k < *matSize)
                {
                    continue;
                }
                mat[j][k] = INT_MAX;
            }
        }
        *matSize = i + 1;
    }
    else
    {
        for (size_t j = 0; j < *matSize; j++)
        {
            mat[i][j] = INT_MAX;
        }
    }

    int jFound = 0;
    int j, weight;
    while (token != NULL)
    {
        token = strtok(NULL, s);
        if (token == NULL)
            break;
        if (!jFound)
        {
            jFound = 1;
            j = atoi(token);
        }
        else
        {
            weight = atoi(token);
            mat[i][j] = weight;
            jFound = 0;
        }
    }
    return mat;
}

int **D(char *str, int **mat, int matSize)
{
    const char s[2] = " ";
    char *token;

    token = strtok(str, s);
    token = strtok(NULL, s);
    int i = atoi(token);
    if (i < matSize)
    {
        for (size_t j = 0; j < matSize; j++)
            mat[i][j] = INT_MAX;
        for (size_t j = 0; j < matSize; j++)
            mat[j][i] = INT_MAX;
    }
    return mat;
}

void printMat(int **mat, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

void kazarmatrix(int **matrix, int **kazar, int matSize)
{
    for (int i = 0; i < matSize; i++)
    {
        for (int j = 0; j < matSize; j++)
        {
            if (matrix[i][j] == 0)
            {
                kazar[i][j] = INT_MAX;
            }
            else
            {
                kazar[i][j] = matrix[i][j];
            }
        }
    }

    for (int k = 0; k < matSize; k++)
    {
        for (int i = 0; i < matSize; i++)
        {
            for (int j = 0; j < matSize; j++)
            {
                if (kazar[i][k] != INT_MAX && kazar[k][j] != INT_MAX)
                    kazar[i][j] = min(kazar[i][j], (kazar[i][k] + kazar[k][j]));
            }
        }
    }
}


void freeMat(int** mat, int matSize) {
    for (int i = 0; i < matSize; i++)
        free(mat[i]);
    free(mat);
}

void S(char *str, int **mat, int matSize)
{
    const char s[2] = " ";
    char *token;

    token = strtok(str, s);
    token = strtok(NULL, s);
    int i = atoi(token);
    token = strtok(NULL, s);
    int j = atoi(token);

    int **matKazar = (int**)malloc(matSize * sizeof(int *));
    for (int i = 0; i < matSize; i++)
        matKazar[i] = (int*)malloc(matSize * sizeof(int));

    kazarmatrix(mat, matKazar, matSize);
    if (matKazar[i][j] == INT_MAX)
        printf("Dijsktra shortest path: -1\n");
    else
        printf("Dijsktra shortest path: %d\n", matKazar[i][j]);
    free(matKazar);
}

int C(int* R, int i, int** matKazar, int matSize, int RSize, int* visited) {
    int visitedAll = 1;
    for (size_t j = 0; j < RSize; j++) {
        if (!visited[R[j]]) {
            visitedAll = 0;
            break;
        }
    }
    if (visitedAll)
        return 0;
    
    int minCost = INT_MAX;
    for (size_t j = 0; j < matSize; j++) {
        if (!visited[j] && matKazar[i][j] != INT_MAX) {
            int* visitedCopy = malloc(sizeof(int) * matSize);
            memcpy(visitedCopy, visited, matSize * sizeof(int));
            visitedCopy[j] = 1;
            int curr = C(R, j, matKazar, matSize, RSize, visitedCopy);
            if (curr != INT_MAX) {
                minCost = min(
                    minCost,
                    curr + matKazar[i][j]
                );
            }
            free(visitedCopy);
        }
    }
    return minCost;
}

void T(char* str, int** mat, int matSize) {
    const char s[2] = " ";
    char *token;
    token = strtok(str, s);
    token = strtok(NULL, s);
    int k = atoi(token);
    int* arr = (int*)malloc(k * sizeof(int));
    for (size_t i = 0; i < k; i++)
    {
        token = strtok(NULL, s);
        arr[i] = atoi(token);
    }
    int **matKazar = (int**)malloc(matSize * sizeof(int *));
    for (int i = 0; i < matSize; i++)
        matKazar[i] = (int*)malloc(matSize * sizeof(int));
    kazarmatrix(mat, matKazar, matSize);
    
    int minVal = INT_MAX;
    for (size_t i = 0; i < matSize; i++)
    {
        int* visited = calloc(matSize, sizeof(int));
        visited[i] = 1;
        int res = C(arr, i, matKazar, matSize, k, visited);
        minVal = min(minVal, res);
        free(visited);
    }
    
    
    if (minVal == INT_MAX)
        printf("TSP shortest path: -1\n");
    else
        printf("TSP shortest path: %d\n", minVal);
    free(arr);
    freeMat(matKazar, matSize);
}

int isCommand(char c) {
    return c == 'A' || c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\n';
}

int main()
{
    int size = 4;
    int i = 0;
    char* str = malloc(size);
    str[1] = 0;
    char currTask = 0,
        nextTask = 0;

    int** mat = NULL;
    int matSize = -1;

    while (1) {
        currTask = nextTask;
        while (1) {
            if (scanf("%c", &str[i]) == EOF) {
                free(str);
                freeMat(mat, matSize);
                return 1;
            }
            if (isCommand(str[i])) {
                nextTask = str[i];
                str[i] = 0;
                i = 2;
                break;
            }
            i++;
            if (i == size - 1) {
                size *= 2;
                str = realloc(str, size);
            }
        }
        if (currTask != 0) {
            str[0] = currTask;
            str[1] = ' ';
            if (currTask == 'A') {
                freeMat(mat, matSize);
                mat = A(str, &matSize);
            }
            else if (currTask == 'B')
                mat = B(str, mat, &matSize);
            else if (currTask == 'S')
                S(str, mat, matSize);
            else if (currTask == 'T')
                T(str, mat, matSize);
        }
    }
}