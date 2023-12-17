#pragma once

#include <Windows.h>
#include <assert.h>
#include <optional>

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <array>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

#include <memory>
#include <functional>
#include <chrono>

#pragma comment(lib, "user32.lib") // prevent error LNK2019: unresolved external symbol __imp__GetMessageW@16 referenced in function _WinMain@16
#pragma comment(lib, "gdi32.lib")  // prevent error LNK2019: unresolved external symbol __imp__GetStockObject@4 referenced in function "public: bool __thiscall Window::Initialize(void)" (?Initialize@Window@@QAE_NXZ)
