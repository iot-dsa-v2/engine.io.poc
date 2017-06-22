#pragma once

#if defined(_WIN32) || defined(WIN32)
#include "platform-win.h"
#else
# include "platform-linux.h"
#endif