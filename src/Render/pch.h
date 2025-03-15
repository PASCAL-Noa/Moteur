#ifndef PCH_H
#define PCH_H

// Standard Libraries
#include <iostream>
#include <wrl.h>
#include <string>
#include <Windows.h>
#include <stdexcept>
#include <WindowsX.h>
#include <cassert>
#include <crtdbg.h>
#include <exception>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <comdef.h>
#include <stdint.h>
#include <chrono>
#include <tuple>

// DirectX Libraries
#include <DirectXMath.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXColors.h>
#include <D3Dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#pragma warning(push)
#pragma warning(disable : 4005)
#pragma warning(pop)

#endif