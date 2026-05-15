#include "stdio.h"
#include "stdlib.h"

int findMax(int *numbers, int N){
    int max = numbers[0];
    for (int i=1; i<N; i++){
        if (numbers[i]>max){
            max = numbers[i];
        }
    }
    return max;
}


int findMin(int *numbers, int N){
    int min = numbers[0];
    for (int i=1; i<N; i++){
        if (numbers[i]<min){
            min = numbers[i];
        }
    }
    return min;
}

void bubbleSort(int *numbers, int N){
    for (int i=0; i<N; i++){
        for (int j=i+1; j<N; j++){
            if (numbers[i]>numbers[j]){
                int temp = numbers[i];
                numbers[i] = numbers[j];
                numbers[j]=temp;
            }
        }
    }
}
int main(){
    int N;
    printf("Numder of numbers: ");
    scanf("%d", &N);

    int *numbers = malloc(N*sizeof(int));

    printf("Numbers: ");
    for (int i=0; i<N; i++){
        scanf("%d", &numbers[i]);
    }

    int max = findMax(numbers, N);
    int min = findMin(numbers, N);

    bubbleSort(numbers, N);

    printf("%d ", max);
    printf("%d ", min);

    for (int i =0; i<N; i++){
        printf("%d ", numbers[i]);
    }

}