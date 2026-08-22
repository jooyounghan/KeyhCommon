#include "RhiSystemPch.h"
#include "IRhiComputePipeline.h"

namespace keyh
{
	IRhiComputePipeline::IRhiComputePipeline(const RhiComputePipelineDesc& desc)
		: _desc(desc)
	{}
}
