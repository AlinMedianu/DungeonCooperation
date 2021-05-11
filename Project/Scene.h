#pragma once

#include "Renderer.h"

namespace Scenes
{
	enum class Type
	{
		MainMenu, Tutorial,
		COUNT
	};

	struct Scene
	{
		virtual void Update() = 0;
		virtual [[nodiscard]] Type WantsToTransitionTo() = 0;
		virtual void GetDrawn(Renderer& renderer) const = 0;
	};
}
