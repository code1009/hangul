#include <iostream>

#include "app/app_krc.hpp"
#include "app/app_bf.hpp"
#include "app/app_krc_input.hpp"

int main()
{
    krc_main();
    bf_main();
    krc_input_main();

    return 0;
}
