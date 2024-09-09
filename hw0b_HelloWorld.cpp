#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <deque>
#include <list>
#include <cmath>      // for std::abs
#include <utility>  // for std::swap
#include <type_traits> // for std::enable_if and std::is_same C++ 17

// Functors for custom sorting based on different criteria

//*****************
// Functor: OddFirst
// Purpose: Sorts integers such that odd numbers come before even numbers,
//          and within the same group, sorts in ascending order.
//*****************
struct OddFirst
{
	bool operator()(const int& a, const int& b) const
	{
		if ((a % 2 != 0) && (b % 2 == 0)) return false; // Odd numbers first
		if ((a % 2 == 0) && (b % 2 != 0)) return true;  // Even numbers after
		return a < b;  // Sort within the same group (odd or even) in ascending order
	}
};

//*****************
// Functor: DivisibleBy3First
// Purpose: Sorts integers such that numbers divisible by 3 come first,
//          and within the same group, sorts in ascending order.
//*****************
struct DivisibleBy3First
{
	bool operator()(const int& a, const int& b) const
	{
		if ((a % 3 == 0) && (b % 3 != 0)) return false; // Divisible by 3 come first
		if ((a % 3 != 0) && (b % 3 == 0)) return true;
		return a < b;  // Sort within the same group (divisible by 3 or not)
	}
};

//*****************
// Functor: EvenFirst
// Purpose: Sorts integers such that even numbers come before odd numbers,
//          and within the same group, sorts in ascending order.
//*****************
struct EvenFirst
{
	bool operator()(const int& a, const int& b) const
	{
		if ((a % 2 == 0) && (b % 2 != 0)) return false; // Even numbers first
		if ((a % 2 != 0) && (b % 2 == 0)) return true;  // Odd numbers after
		return a < b;  // Sort within same group (even or odd)
	}
};

//*****************
// Functor: SumOfDigits
// Purpose: Sorts integers based on the sum of their digits in ascending order.
//*****************
struct SumOfDigits
{
	//*****************
	// Helper Function: sumDigits
	// Purpose: Returns the sum of the digits of a given integer.
	//*****************
	int sumDigits(int n) const
	{
		int sum = 0;
		while (n > 0)
		{
			sum += n % 10;
			n /= 10;
		}
		return sum;
	}

	// Overloaded operator() to compare numbers based on the sum of their digits.
	bool operator()(const int& a, const int& b) const
	{
		return sumDigits(a) < sumDigits(b); // Sort by sum of digits
	}
};

//*****************
// Functor: AlphabeticalPosition
// Purpose: Sorts strings based on the sum of their alphabetical positions
//          (e.g., 'a' = 1, 'b' = 2, ..., 'z' = 26).
//*****************
struct AlphabeticalPosition
{
	bool operator()(const std::string& a, const std::string& b) const
	{
		int posA = 0, posB = 0;
		for (char c : a) posA += c - 'a' + 1;  // Calculate position for a
		for (char c : b) posB += c - 'a' + 1;  // Calculate position for b
		return posA < posB;  // Compare positions
	}
};

//*****************
// Function name: printContainer
// Purpose: Prints the contents of any container that supports iteration.
// Parameters:
//    - holder: A const reference to a container that holds elements to be printed.
// Returns: void
// Return type: void
//*****************
template <typename Container>
void printContainer(const Container& holder) noexcept
{
	// Loop through the container and print each item, separated by spaces.
	for (const auto& item : holder)
	{
		std::cout << item << " ";
	}
	// Using newline escape sequence to avoid buffer flush
	std::cout << "\n";
}

// Helper type trait to detect if a type is a container
template<typename T, typename _ = void>
struct is_container : std::false_type {};

// This helper struct checks for the presence of typical container types and iterators
template<typename... Ts>
struct is_container_helper {};

// Specialization for containers that have value_type, iterators, and a size method
template<typename T>
struct is_container<T, std::conditional_t<false, is_container_helper<
	typename T::value_type,
	typename T::iterator,
	typename T::const_iterator,
	decltype(std::declval<T>().size())
>, void>> : std::true_type {};

//*****************
// Template Function: printNDVector (for non-containers)
// Purpose: Prints nested vectors (N-dimensional containers) with indentation based on depth.
// Parameters:
//    - holder: A const reference to a potentially multi-dimensional container.
//    - depth: An optional integer specifying the level of depth for indentation (default: 0).
// Returns: void
//*****************
template <typename T>
typename std::enable_if<!is_container<T>::value and std::is_arithmetic<T>::value>::type
printNDVector(const T& holder, int depth = 0)
{
	// Indent based on depth to visually show nesting
	std::cout << std::string(depth * 4, ' ') << holder;  // Increase indentation per depth level
}

//*****************
// Template Function: printNDVector (for containers)
// Purpose: Prints higher-dimensional containers (e.g., vectors of vectors) recursively.
// Parameters:
//    - holder: A const reference to a container that may hold other containers (multi-dimensional).
//    - depth: An integer specifying the level of depth for indentation.
// Returns: void
//*****************
template <typename T>
typename std::enable_if<is_container<T>::value>::type
printNDVector(const T& holder, int depth = 0)
{
	std::cout << std::string(depth * 4, ' ') << "\n{";  // Opening bracket to denote dimension

	for (const auto& subHolder : holder)
	{
		printNDVector(subHolder, depth + 1);  // Recursive call for nested elements
		;  // Newline for better readability after each element
	}

	std::cout << std::string(depth * 4, ' ') << "}\n";  // Closing bracket for this dimension
}

//*****************
// Template Function: bubbleSort
// Purpose: Performs a Bubble Sort on a given container using a specified comparator.
// Parameters:
//    - holder: A reference to a container that needs to be sorted.
//    - compare: A comparator function or functor for custom sorting true bubbles up (default: greater).
// Returns: void
//*****************
template <typename Container, typename Comparator = std::greater<typename Container::value_type>>
void bubbleSort(Container& holder, Comparator compare = Comparator()) noexcept
{
	bool swapped;
	const std::size_t n = std::distance(holder.begin(), holder.end());
	if (n < 2) return;

	// Perform bubble sort with a comparator to define sorting order
	for (std::size_t i = 0; i < n - 1; ++i)
	{
		swapped = false;
		auto it = holder.begin();
		for (std::size_t j = 0; j < n - i - 1; ++j)
		{
			auto next = std::next(it);
			if (compare(*it, *next))
			{
				std::swap(*it, *next);
				swapped = true;
			}
			it = next;
		}
		if (!swapped) break;
	}
}

//*****************
// Template Function: recursiveSort
// Purpose: Recursively sorts a container and its nested containers (if any) using the provided comparator.
// Parameters:
//    - container: A reference to a container that may contain nested containers to be sorted.
//    - compare: A comparator function or functor for custom sorting (default: greater).
// Returns: void
//*****************
template <typename Container, typename Comparator = std::greater<typename Container::value_type>>
void recursiveSort(Container& container, Comparator compare = Comparator())
{
	// If elements of the container are themselves containers, sort them recursively
	if constexpr (is_container<typename Container::value_type>::value) // If subContainer is a container
	{
		for (auto& subContainer : container)
		{
			recursiveSort(subContainer, compare); // Recursively sort nested containers
		}
	}
	else // Base case: Sort if it's not a nested container
	{
		bubbleSort(container, compare);
	}
}

int main()
{
	std::vector<int> vec1D = { 5, 2, 9, 1, 5, 6 };
	std::cout << "Original 1D vector: ";
	printContainer(vec1D);
	recursiveSort(vec1D, [](int a, int b) { return a < b; });
	std::cout << "Sorted 1D vector (Descending): ";
	printContainer(vec1D);
	std::cout << "\n";

	std::list<int> list1D = { 5, 2, 9, 1, 5, 6 };
	std::cout << "Original 1D list: ";
	printContainer(list1D);
	recursiveSort(list1D, OddFirst());
	std::cout << "Sorted 1D list (Odd numbers first): ";
	printContainer(list1D);
	std::cout << "\n";

	std::array<int, 6> arr1D = { 234, 56, 123, 12, 345, 678 };
	std::cout << "Original 1D array: ";
	printContainer(arr1D);
	recursiveSort(arr1D, SumOfDigits()); // Sum of digits
	std::cout << "Sorted 1D array (Sum of digits): ";
	printContainer(arr1D);
	std::cout << "\n";

	std::vector<std::vector<int>> vec2D = { {5, 2, 9}, {6, 3, 8}, {1, 7, 4} };
	std::cout << "Original 2D vector (matrix):\n";
	printNDVector(vec2D);
	recursiveSort(vec2D, DivisibleBy3First()); // Divisible by 3 first
	std::cout << "Sorted 2D vector (Divisible by 3 first):\n";
	printNDVector(vec2D);
	std::cout << "\n";

	std::list<std::list<int>> list2D = { {5, 2, 9}, {6, 3, 8}, {1, 7, 4} };
	std::cout << "Original 2D list:\n";
	printNDVector(list2D);
	recursiveSort(list2D, EvenFirst()); // Even numbers first
	std::cout << "Sorted 2D list (Even numbers first):\n";
	printNDVector(list2D);
	std::cout << "\n";

	std::array<std::array<int, 3>, 3> arr2D = { {{234, 56, 123}, {789, 23, 456}, {12, 345, 678}} };
	std::cout << "Original 2D array:\n";
	printNDVector(arr2D);
	recursiveSort(arr2D, SumOfDigits()); // Sum of digits
	std::cout << "Sorted 2D array (Sum of digits):\n";
	printNDVector(arr2D);
	std::cout << "\n";

	std::vector<std::vector<std::vector<int>>> vec3D = { {{1, 20, 5}, {8, 15, 2}}, {{30, 12, 4}, {7, 10, 11}}, {{25, 3, 14}, {9, 6, 18}} };
	std::cout << "Original 3D vector:\n";
	printNDVector(vec3D);
	recursiveSort(vec3D, [](int a, int b) { return std::abs(a - 10) > std::abs(b - 10); }); // Proximity to 10
	std::cout << "Sorted 3D vector (Proximity to 10):\n";
	printNDVector(vec3D);
	std::cout << "\n";

	std::list<std::list<std::list<int>>> list3D = { {{1, 20, 5}, {8, 15, 2}}, {{30, 12, 4}, {7, 10, 11}}, {{25, 3, 14}, {9, 6, 18}} };
	std::cout << "Original 3D list:\n";
	printNDVector(list3D);
	recursiveSort(list3D, OddFirst()); // Odd numbers first
	std::cout << "Sorted 3D list (Odd numbers first):\n";
	printNDVector(list3D);
	std::cout << "\n";

	return 0;
}