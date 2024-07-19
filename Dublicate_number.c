#include <stdio.h>
#define size 6
// code to find the dublicate number in the array
void dublicate_number(int arr[],int s){
    for(int i = 0; i < s;i++){
        for(int j =i + 1; j<s; j++){
            if(arr[i] == arr[j]){
                printf("The dublicate number are %d/n",arr[i]);
                break;
            }
        }
    }
}

int main(){
    int arr[size] = { 1,2,3,3,5,5};
    dublicate_number(arr,size);
    return 0;
}
