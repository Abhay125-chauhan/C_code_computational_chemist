#include <stdio.h>
#define SIZE 10 

int find_max(int arr[],int size){
    int max = arr[0];
    for(int i = 0;i< size; i++){
        if(arr[i]>max){
            max = arr[i];
        }
    }
    return max;
}

int main(){
    int arr[SIZE] = {1,2,3,4,5,6,7,8,9,10};
    int max_value = find_max( arr,SIZE);
    printf("the max value of array is %d",max_value);
    return 0;
    
}
