#pragma once

#include "debugnew.h"

#define new new(__FILE__, __LINE__)
