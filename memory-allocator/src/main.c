#include "custom_test.h"

int main(){

    int passed = 5;

    passed-=simple_memory_allocation_test();

    passed-=freeing_one_of_several_test();

    passed-=freeing_two_of_several_test();

    passed-=heap_growth_test();

    passed-=heap_not_continuous_growth_test();

    printf("Passed tests %d out of 5",passed);
    
    return 0;
}
