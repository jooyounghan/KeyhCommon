#pragma once
// CommonBase public API umbrella header.
//
// Include this single header in your project's ForcedIncludeFiles build setting
// (or PCH / shared common header) to expose all types without
// per-file includes.
//
// Visual Studio usage – add to each ItemDefinitionGroup in .vcxproj:
//   <ForcedIncludeFiles>CommonBase.h;%(ForcedIncludeFiles)</ForcedIncludeFiles>
//
// After that, every .cpp in the referencing project has access to all
// CommonBase types with no explicit #include required.

#include "CommonCore.h"

#include "AssertUtil.h"
#include "FileUtil.h"
#include "HashUtil.h"
#include "MathUtil.h"
#include "MemoryUtil.h"
#include "SimdUtil.h"
#include "StrUtil.h"

#include "StaticString.h"
#include "FlyweightString.h"

#include "Ptr.h"
#include "RefPtr.h"
#include "WeakPtr.h"

#include "HashMap.h"
#include "Vector.h"

#include "IBuffer.h"