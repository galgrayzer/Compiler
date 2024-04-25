#pragma once

using namespace std;
#include <string>

typedef struct Register
{
    string name; // name of the register
    bool isFree = true; // state of the register
} Register;
