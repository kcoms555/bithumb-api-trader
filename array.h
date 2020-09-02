#ifndef __ARRAY__
#define __ARRAY__
#include <iostream>
#include <cstring>

template <typename T>
int arraymax(T * array, int size){
T max = array[0];
int index = 0;
for(int i=0;i<size;i++)
	if(max<array[i]){
		max=array[i];
		index=i;
		}
return index;
}

template <typename T>
int arraymin(T * array, int size){
T min = array[0];
int index = 0;
for(int i=0;i<size;i++)
	if(min>array[i]){
		min=array[i];
		index=i;
		}
return index;
}

template <typename T>
int arraymax(T * array, int size, int n){
if(n<1) return -1;
if(n>size) return -1;

T tmparray[size];
memcpy(tmparray, array, sizeof(T)*size);
T min = array[arraymin(array, size)];

	while(n!=1){
	tmparray[arraymax(tmparray, size)]=min-1;
	n--;
	}
return arraymax(tmparray, size);
}

template <typename T>
int arraymin(T * array, int size, int n){
if(n<1) return -1;
if(n>size) return -1;

T tmparray[size];
memcpy(tmparray, array, sizeof(T)*size);
T max = array[arraymax(array, size)];

while(n!=1){
	tmparray[arraymin(tmparray, size)]=max+1;
	n--;
}
return arraymin(tmparray, size);
}
#endif
