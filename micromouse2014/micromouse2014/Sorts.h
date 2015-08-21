#ifndef __SORTS_H__
#define __SORTS_H__

#include <vector>
#include <deque>

// NOTE: static function definitions must be in header file, for they can only be defined once...

namespace Sorting
{
	template<typename T>
	class Sorts
	{
	public:
		static void swapvals(T &val1, T &val2)
		{
			/*T temp = val1;
			val1 = val2;
			val2 = temp;*/
			std::swap(val1, val2);
		}

		/*static void swapvals(T const &val1, T const &val2)
		{
			T temp = val1;
			val1 = val2;
			val2 = temp;
		}*/

		static void reverse(std::vector<T> &arr)
		{
			int l = 0, u = arr.size() - 1;

			while (l < u)
			{
				swapvals(arr[l], arr[u]);
				l++;
				u--;
			}
		}

	private:
		// quicksort partition
		static int partition(std::vector<T> &arr, int l, int r)
		{
			int i = l - 1, j = l;

			while (j < r)
			{
				if (*arr[j] <= *arr[r])
				{
					i++;
					swapvals(arr[j], arr[i]);	// swap a[i] & a[j]
				}
				j++;
			}

			swapvals(arr[i + 1], arr[r]);

			return i + 1;
		}

		// quicksort partition
		static int partition(std::deque<T> &arr, int l, int r)
		{
			int i = l - 1, j = l;

			while (j < r)
			{
				if (*arr[j] <= *arr[r])
				{
					i++;
					swapvals(arr[j], arr[i]);	// swap a[i] & a[j]
				}
				j++;
			}

			swapvals(arr[i + 1], arr[r]);

			return i + 1;
		}

	public:
		// Quicksort
		static void quicksort(std::vector<T> &arr, int l, int r)
		{
			if (l >= r)
				return;

			int index = partition(arr, l, r);

			quicksort(arr, l, index - 1);		// left
			quicksort(arr, index + 1, r);		// right
		}

		static void quicksort(std::deque<T> &arra, int l, int r)
		{
			if (l >= r)
				return;

			int index = partition(arra, l, r);

			quicksort(arra, l, index - 1);		// left
			quicksort(arra, index + 1, r);		// right
		}
	};

	template<typename SequenceContainer>
	class SortingT
	{
	public:
		// Merge sort iterative
		static void mergesort_i(SequenceContainer &arr)
		{
			for (int j = 0; j < arr.size() / 2; j++)	// must do the inner loops size/2 times
				for (int i = 1; i <= (arr.size() / 2) + 1; i *= 2)
					for (int m = i; m < arr.size(); m += 2 * i)
					{
						int r;
						((m + i) < arr.size()) ? r = (m + i) : r = arr.size() - 1;
						merge(arr, (m - i), m, r);
					}
		}

	private:
		// merge sort
		static void merge(SequenceContainer &arr, int l, int mid, int r)
		{
			SequenceContainer left, right;				// two sub-lists
			
			for (int i = l; i <= mid; i++)
				left.push_back(std::move(arr[i]));				// create left list
			for (int i = mid + 1; i <= r; i++)
				right.push_back(std::move(arr[i]));			// create right list
				
			
			int index = l, i = 0, j = 0;
			while (index <= r)						// merge the two lists
			{
				if (i == left.size())
				{
					if (j != right.size())
					{
						arr[index] = std::move(right[j]);
						j++;
					}
				}
				else if (j == right.size())
				{
					if (i != left.size())
					{
						arr[index] = std::move(left[i]);
						i++;
					}
				}
				else if (*left[i] <= *right[j])
				{
					arr[index] = std::move(left[i]);
					i++;
				}
				else
				{
					arr[index] = std::move(right[j]);
					j++;
				}
				index++;
			}
		}

	};
}

#endif