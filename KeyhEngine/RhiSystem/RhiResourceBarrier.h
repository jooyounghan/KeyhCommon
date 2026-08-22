#pragma once
#include "RhiEnum.h"

namespace keyh
{
	class IRhiBuffer;
	class IRhiTexture;

#pragma region ResourceBarrierType
	enum class EResourceBarrierType : uint8
	{
		Transition,
		Aliasing,
		UnorderedAccess,
		Count
	};
#pragma endregion

#pragma region ResourceBarrierTarget
	enum class EResourceBarrierTarget : uint8
	{
		Buffer,
		Texture
	};
#pragma endregion

#pragma region RhiResourceBarrier
	struct RhiResourceBarrier
	{
		EResourceBarrierType _type = EResourceBarrierType::Transition;

		struct TransitionBarrier
		{
			union
			{
				IRhiBuffer*  _buffer  = nullptr;
				IRhiTexture* _texture;
			};
			EResourceBarrierTarget _target       = EResourceBarrierTarget::Buffer;
			EResourceState         _stateBefore  = EResourceState::Common;
			EResourceState         _stateAfter   = EResourceState::Common;
			uint32                 _subresource  = 0xFFFFFFFF;
		} _transition;

		static RhiResourceBarrier transitionBuffer(IRhiBuffer* buffer, EResourceState before, EResourceState after, uint32 subresource = 0xFFFFFFFF)
		{
			RhiResourceBarrier barrier;
			barrier._type                       = EResourceBarrierType::Transition;
			barrier._transition._buffer         = buffer;
			barrier._transition._target         = EResourceBarrierTarget::Buffer;
			barrier._transition._stateBefore    = before;
			barrier._transition._stateAfter     = after;
			barrier._transition._subresource    = subresource;
			return barrier;
		}

		static RhiResourceBarrier transitionTexture(IRhiTexture* texture, EResourceState before, EResourceState after, uint32 subresource = 0xFFFFFFFF)
		{
			RhiResourceBarrier barrier;
			barrier._type                       = EResourceBarrierType::Transition;
			barrier._transition._texture        = texture;
			barrier._transition._target         = EResourceBarrierTarget::Texture;
			barrier._transition._stateBefore    = before;
			barrier._transition._stateAfter     = after;
			barrier._transition._subresource    = subresource;
			return barrier;
		}
	};
#pragma endregion
}
