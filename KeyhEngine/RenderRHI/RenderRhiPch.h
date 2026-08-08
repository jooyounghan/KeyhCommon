#pragma region Windows Platform
#if defined(KEYH_PLATFORM_WINDOWS)
#include <windows.h>
#include <wrl/client.h>
#include <wrl/implements.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#else
#endif
#pragma endregion