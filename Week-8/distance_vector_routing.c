#include <stdio.h>
#define INF 999
int main()
{
    int n, i, j, k;
    int cost[10][10], dist[10][10], via[10][10];

    printf("Enter number of routers: ");
    scanf("%d", &n);

    printf("Enter cost matrix:\n");

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            scanf("%d", &cost[i][j]);
            dist[i][j] = cost[i][j];
            via[i][j] = j;
        }
    }

    for (k = 0; k < n; k++)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                if (dist[i][j] > cost[i][k] + dist[k][j])
                {
                    dist[i][j] = cost[i][k] + dist[k][j];
                    via[i][j] = k;
                }
            }
        }
    }

    printf("\nRouting Tables:\n");

    for (i = 0; i < n; i++)
    {
        printf("\nRouter %d:\n", i + 1);
        printf("Destination\tCost\tNext Hop\n");

        for (j = 0; j < n; j++)
        {
            printf("%d\t\t%d\t%d\n",
                   j + 1, dist[i][j], via[i][j] + 1);
        }
    }

    return 0;
}




// Sample Input

// Enter number of routers: 4
// Enter cost matrix:
// 0 2 999 1
// 2 0 3 999
// 999 3 0 4
// 1 999 4 0