#include <iostream>
#define LINQ_USE_MACROS
#include "../lib/linq.h"

#ifdef __linux__
#include <sys/time.h>
#else
#include <Windows.h>
#endif

using namespace std;

#define PERFORMANCE_TEST false
#define PERFORMANCE_ITERATIONS 10000

int main() {
	typedef struct { int i; std::string desc; } description;
	int nums[10] = { 1,2,3,4,5,6,7,8,9,10 };

	typedef struct { int id, proj_id; std::string name; } Employee;
	typedef struct { int id; std::string name; } Project;
	typedef struct { struct { int id; std::string name; } employee; std::string proj_name; } EmployeeProject;
	Employee employees[3] = { { 0, 0, "Joe" },{ 1, 1, "Jane" },{ 2, 1, "Alex" } };
	vector<Project> projects = { { 0, "Financial Inc." },{ 1, "Contracts R Us" } };

#if PERFORMANCE_TEST
	float cumulative = 0;
#ifdef __linux__
	timespec start, end;
#else
	LARGE_INTEGER start, frequency, end;
	QueryPerformanceFrequency(&frequency);
#endif
	for (int __IT = 0; __IT < PERFORMANCE_ITERATIONS; __IT++) {
#ifdef __linux__
		clock_gettime(CLOCK_REALTIME, &start);
#else
		QueryPerformanceCounter(&start);
#endif
#endif
		// Select the even numbers from the list
		auto evens = FROM(nums, 10) WHERE { return item % 2 == 0; } ORDERBY(DESCENDING) END;
		// Select the odd numbers from the list
		auto odds = linq::from(nums, 10).where([](auto item) { return item % 2 == 1; }).orderby(linq::descending);
		// Change the data to a new type
		auto even_descs = FROM(evens) SELECT(description) { return { item, "even" }; } END;
		auto odd_descs = FROM(odds) SELECT(description) { return { item, "odd" }; } END;

		// Find the first even number
		int test = linq::from(nums, 10).first([](int item) { return (item % 2) == 0; });
		// Find the number equal to 11 (will fail and return default)
		test = linq::from(nums, 10).first_or_default(0, [](int item) { return item == 11; });
		// Macro version of finding first even number
		test = FROM(nums, 10) FIRST { return (item % 2) == 0; } END;
		// Macro version of finding the number equal to 11 (will fail and return default)
		test = FROM(nums, 10) FIRST_OR_DEFAULT(0) { return item == 11; } END;

		// Find the last even number
		test = linq::from(nums, 10).last([](int item) { return (item % 2) == 0; });
		// Find the number equal to 11 (will fail and return default)
		test = linq::from(nums, 10).last_or_default(0, [](int item) { return item == 11; });
		// Macro version of finding last even number
		test = FROM(nums, 10) LAST { return (item % 2) == 0; } END;
		// Macro version of finding the number equal to 11 (will fail and return default)
		test = FROM(nums, 10) LAST_OR_DEFAULT(0) { return item == 11; } END;

		// Select the odds and then convert the array to a vector
		auto vec = linq::from(nums, 10).where([](auto item) { return item % 2 == 1; }).to_vector();
		// Select the evens and the convert the array to a vector
		vec = FROM(nums, 10) WHERE { return item % 2 == 0; }  TO_VECTOR END;

		// Retrieves the first element
		auto first = linq::from(nums, 10).first();
		// Retrieves the first even number in the list
		first = linq::from(nums, 10).first([](auto item) { return item % 2 == 0; });
		// Retrieves the first number greater than 10, or returns 0 if none match
		first = linq::from(nums, 10).first_or_default(0, [](auto item) { return item > 10; });
		// Retrieves the last element in the array
		auto last = linq::from(nums, 10).last();
		// Retrieves the last number in the array that is less than 10
		last = linq::from(nums, 10).last([](auto item) { return item < 10; });
		// Retrieves the last number in the array greater than 10, or returns 0 if none match
		last = linq::from(nums, 10).last_or_default(0, [](auto item) { return item > 10; });

		// Checks if the array contains any elements
		auto exists = linq::from(nums, 10).any();
		// Checks if there is any element less than 0
		exists = linq::from(nums, 10).any([](auto item) { return item < 0; });

		// Calculates the sum of all the elements
		int total = linq::from(nums, 10).sum<int>([](auto item) { return item; });

		// First method of joining using the MERGE_JOIN
		auto empProjs = FROM(employees, 3)
			MERGE_JOIN(Project, EmployeeProject) projects
			INTO(EmployeeProject) { return { { left.id, left.name }, right.name }; } // required
			ON { return left.proj_id == right.id; } // left/right condition
			END;
		// Second method of joining using the JOIN to get an array of pairs and then using SELECT to construct the new array
		empProjs = FROM(employees, 3)
			PAIR_JOIN(Project) projects
			ON { return left.proj_id == right.id; } // left/right condition
			SELECT(EmployeeProject) { return { { item.left.id, item.left.name }, item.right.name }; }
			END;
		// First option as before but without macros
		empProjs = linq::from(employees, 3)
			.join<Project, EmployeeProject>(projects,
				[](auto left, auto right) -> EmployeeProject { return { { left.id, left.name }, right.name }; },
				[](auto left, auto right) -> bool { return left.proj_id == right.id; }); // left/right condition
		// Second option as before but without macros
		empProjs = linq::from(employees, 3)
			.join<Project>(projects, [](auto left, auto right) -> bool { return left.proj_id == right.id; }) // left/right condition
			.select<EmployeeProject>([](auto item) ->EmployeeProject { return { { item.left.id, item.left.name }, item.right.name }; });

		// Group the employees by their ID
		auto empDirectory = FROM(employees, 3)
			MAP(int, Employee)
				KEY { return item.id; }
				VALUE { return item; }
			END;
		// Group the employee names by their ID using full lambdas
		auto empNameDirectory = FROM(employees, 3)
			MAP(int, std::string)
				KEY_L [](auto item) { return item.id; }
				VALUE_L [](auto item) { return item.name; }
			END;

		empDirectory = linq::from(employees, 3).to_map<int>([](auto item) { return item.id; });
		empNameDirectory = linq::from(employees, 3).to_map<int, std::string>([](auto item)->int { return item.id; }, [](auto item)->string { return item.name; });

#if PERFORMANCE_TEST
		float milliseconds = 0;
#ifdef __linux__
		clock_gettime(CLOCK_REALTIME, &end);
		milliseconds = (end.tv_nsec - start.tv_nsec) / 1000000.0f;
#else
		QueryPerformanceCounter(&end);
		milliseconds = (((end.QuadPart - start.QuadPart) * 1000000) / frequency.QuadPart) / 1000.0f;
#endif
		cumulative += milliseconds;
		printf("%.0f%% %.3fms\n", ((float)__IT / (float)PERFORMANCE_ITERATIONS) * 100.0f, milliseconds);
	}
	std::cout << " Average: " << cumulative / (float)PERFORMANCE_ITERATIONS << "ms\n";
	std::cout << "Duration: " << cumulative << "ms" << std::endl;
#endif
	return 0;
}
