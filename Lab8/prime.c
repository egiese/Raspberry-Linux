#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void signalHandler(int);
int isPrime(int);

int latestPrime = 3;
char input[1];

int main()
{
    int testPrime = latestPrime;
    signal(SIGINT, signalHandler);
    
    for(testPrime; testPrime < 200000; testPrime++)
        if(isPrime(testPrime))
            latestPrime = testPrime;

    return 0;
}

int isPrime(int lastPrime)
{
    int i;

    if (lastPrime % 2 == 0) 
        return 0;
        
    for(i = 3; i < lastPrime; i++)
        if (lastPrime % i == 0)
            return 0;

    return 1;
}

void signalHandler(int signalNum)
{
    if(signalNum == SIGINT)
    {
        while(1)
        {
            printf("\nLast prime was %d --- conintue 'Y' or exit 'N' \n", latestPrime);
            scanf("%s", input);
            
            if(input[0] == 'Y' || input[0] == 'y')
                break;
                
            else if(input[0] == 'N' || input[0] == 'n')
            {
                printf("--- Exiting program ---\n");
                exit(0);
            }
            else
                printf("Didn't understand, please try again...\n\n");
        }
    }
}
