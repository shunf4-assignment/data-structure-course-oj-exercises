#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int searchIn(int num, int *arr, int start, int end)
{
	if (start == end)
	{
		if (num == arr[start])
			return start;
		else
			return -1;
	}

	int mid;
	mid = (start + end) / 2;
	//if (num == arr[mid])
	//	return mid;
	//else
	if (num <= arr[mid])
		return searchIn(num, arr, start, mid);
	else
		return searchIn(num, arr, mid + 1, end);
}

int main()
{
	int n;
	scanf("%d", &n);
	int *arr;
	arr = (int *)malloc(n * sizeof(int));
	if (arr == NULL)
		return 0;

	for (int i = 0; i < n; i++)
	{
		scanf("%d", &arr[i]);
	}

	int num;
	scanf("%d", &num);

	printf("%d\n", searchIn(num, arr, 0, n - 1));
	return 0;
}