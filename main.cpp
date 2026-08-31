/// @brief Declared in Task2Testing.cpp. Runs the Task 2 Composite demonstration.
void testTask2();
/// @brief Declared in Task3Testing.cpp. Runs the Task 3 Observer/cascading demonstration.
void testTask3();
/// @brief Declared in Task4Testing.cpp. Runs the Task 4 event-rules/reorganisation demonstration.
void testTask4();

/**
 * @brief Entry point. Runs the Task 2, Task 3 and Task 4 demonstrations
 * in sequence so the whole system can be exercised with one executable.
 * @return 0 on success.
 */
int main()
{
    testTask2();
    testTask3();
    testTask4();
    return 0;
}