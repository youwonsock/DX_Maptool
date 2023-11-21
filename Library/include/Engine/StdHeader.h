#pragma once

#include <Windows.h>

#include <string>

#include <stdlib.h>
#include <vector>
#include <queue>
#include <list>
#include <array>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>

#include <memory>
#include <functional>
#include <chrono>

#include <assert.h>

#pragma comment(lib, "user32.lib") // prevent error LNK2019: unresolved external symbol __imp__GetMessageW@16 referenced in function _WinMain@16
#pragma comment(lib, "gdi32.lib")  // prevent error LNK2019: unresolved external symbol __imp__GetStockObject@4 referenced in function "public: bool __thiscall Window::Initialize(void)" (?Initialize@Window@@QAE_NXZ)
