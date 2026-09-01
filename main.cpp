#include <iostream>

/// @brief Declared in Task2Testing.cpp. Runs the Task 2 Composite demonstration.
void testTask2();
/// @brief Declared in Task3Testing.cpp. Runs the Task 3 Observer/cascading demonstration.
void testTask3();
/// @brief Declared in Task4Testing.cpp. Runs the Task 4 event-rules/reorganisation demonstration.
void testTask4();
/// @brief Declared in Task8Demo.cpp. Runs the Task 8 coherent integration demonstration.
void runEventFlowDemo();

/**
 * @brief Entry point.
 *
 * Runs every demonstration unconditionally: the individual Task 2, Task 3
 * and Task 4 demonstrations, followed by the Task 8 coherent integration
 * demonstration. Running everything by default (rather than gating the
 * regression demos behind a --regression flag) means a plain invocation
 * with no arguments, such as an autograder running the built executable
 * directly, exercises every task's code path in one run.
 *
 * @return 0 on success.
 */
int main()
{
    std::cout << "\n########## Regression: Task 2 (Composite) ##########\n";
    testTask2();
    std::cout << "\n########## Regression: Task 3 (Observer) ##########\n";
    testTask3();
    std::cout << "\n########## Regression: Task 4 (Event rules) ##########\n";
    testTask4();

    runEventFlowDemo();

    return 0;
}
