#pragma once
// MaterialSystem public API umbrella header.
//
// Include this single header in your project's ForcedIncludeFiles build setting
// (or PCH / shared common header) to expose all MaterialSystem, ReflectSystem,
// RenderRHI, and CommonBase types without per-file includes.
//
// Visual Studio usage – add to each ItemDefinitionGroup in .vcxproj:
//   <ForcedIncludeFiles>MaterialSystem.h;%(ForcedIncludeFiles)</ForcedIncludeFiles>

#include "ReflectSystem.h"
#include "RenderRHI.h"
