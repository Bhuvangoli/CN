#include <stdio.h>
int main()
{
    int bucket_size, output_rate, n;
    int input, stored = 0;
    int i;

    printf("Enter bucket size: ");
    scanf("%d", &bucket_size);

    printf("Enter output rate: ");
    scanf("%d", &output_rate);

    printf("Enter number of packets: ");
    scanf("%d", &n);

    for (i = 1; i <= n; i++)
    {
        printf("\nEnter packet size: ");
        scanf("%d", &input);

        if (input <= bucket_size - stored)
        {
            stored += input;
            printf("Packet accepted\n");
        }
        else
        {
            printf("Packet dropped\n");
        }

        printf("Packets in bucket: %d\n", stored);

        if (stored >= output_rate)
        {
            stored -= output_rate;
            printf("Packets transmitted: %d\n", output_rate);
        }
        else
        {
            printf("Packets transmitted: %d\n", stored);
            stored = 0;
        }

        printf("Packets remaining: %d\n", stored);
    }

    while (stored > 0)
    {
        if (stored >= output_rate)
        {
            stored -= output_rate;
            printf("\nPackets transmitted: %d", output_rate);
        }
        else
        {
            printf("\nPackets transmitted: %d", stored);
            stored = 0;
        }
    }

    return 0;
}



// Sample Input

// Enter bucket size: 10
// Enter output rate: 4
// Enter number of packets: 5

// Enter packet size: 5
// Enter packet size: 8
// Enter packet size: 3
// Enter packet size: 6
// Enter packet size: 2