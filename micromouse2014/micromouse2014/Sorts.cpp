//#include "Sorts.h"
//
//namespace Sorting
//{
//	// Quicksort
//	template<typename T>
//	void Sorts<T>::quicksort(std::vector<T> &arr, int l, int r)
//	{
//		if (l >= r)
//			return;
//
//		int index = partition(arr, l, r);
//
//		quicksort(arr, l, index - 1);		// left
//		quicksort(arr, index + 1, r);		// right
//	}
//
//	template<typename T>
//	void Sorts<T>::quicksort(std::deque<T> &arra, int l, int r)
//	{
//		if (l >= r)
//			return;
//
//		int index = partition(arra, l, r);
//
//		quicksort(arra, l, index - 1);		// left
//		quicksort(arra, index + 1, r);		// right
//	}
//
//	// Quicksort Partition
//
//	template<typename T>
//	int Sorts<T>::partition(std::vector<T> &arr, int l, int r)
//	{
//		int i = l - 1, j = l;
//
//		while (j < r)
//		{
//			if (arr[j] <= arr[r])
//			{
//				i++;
//				swapvals(arr[j], arr[i]);	// swap a[i] & a[j]
//			}
//			j++;
//		}
//
//		swapvals(arr[i + 1], arr[r]);
//
//		return i + 1;
//	}
//
//	template<typename T>
//	int Sorts<T>::partition(std::deque<T> &arr, int l, int r)
//	{
//		int i = l - 1, j = l;
//
//		while (j < r)
//		{
//			if (arr[j] <= arr[r])
//			{
//				i++;
//				swapvals(arr[j], arr[i]);	// swap a[i] & a[j]
//			}
//			j++;
//		}
//
//		swapvals(arr[i + 1], arr[r]);
//
//		return i + 1;
//	}
//
//}