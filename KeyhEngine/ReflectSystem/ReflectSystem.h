#pragma once
// ReflectSystem public API umbrella header.
//
// Include this single header in your project's ForcedIncludeFiles build setting
// (or PCH / shared common header) to expose all types without
// per-file includes.
//
// Visual Studio usage – add to each ItemDefinitionGroup in .vcxproj:
//   <ForcedIncludeFiles>ReflectSystem.h;%(ForcedIncludeFiles)</ForcedIncludeFiles>
//
// After that, every .cpp in the referencing project has access to all
// ReflectSystem types with no explicit #include required.

#include "ReflectionUtil.h"
#include "IReflectProperty.h"
#include "IReflectObject.h"
#include "ReflectMetaObject.h"
#include "ReflectObject.h"
#include "ReflectProperty.h"
#include "ReflectPropertyPolicy.h"
#include "ReflectSerializer.h"
