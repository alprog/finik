module;
#include "Windows.h"
module Log;

import Finik.Core;

void log_impl(std::string text)
{
    OutputDebugStringA(text.c_str());
}
