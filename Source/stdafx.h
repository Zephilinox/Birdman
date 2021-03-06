// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

//Speed up from 23 seconds to 8 seconds, clean compile time.

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//STD
#include <iostream>

#include <chrono>

#include <functional>

#include <memory>
#include <mutex>

#include <string>

#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

//LIB
#include <Engine/Platform.h>