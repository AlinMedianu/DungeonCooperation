#pragma once

#include <array>
#include "Enumeration.h"
#include "Scene.h"

namespace Scenes
{
	

	template<Enumeration SceneType>
	class Manager
	{
		std::array<Scene*, static_cast<size_t>(SceneType::COUNT)> scenes;
		SceneType current;
		bool wantsToQuit;
	public:
		explicit Manager(std::array<Scene*, static_cast<size_t>(SceneType::COUNT)>&& scenes) noexcept;
		[[nodiscard]] bool WantsToQuit() const noexcept;
		void UpdateCurrent();
		void DrawCurrent(Renderer& with) const;
	};
	template<Enumeration SceneType>
	Manager<SceneType>::Manager(std::array<Scene*, static_cast<size_t>(SceneType::COUNT)>&& scenes) noexcept
		: scenes(scenes), current{}, wantsToQuit{}
	{

	}

	template<Enumeration SceneType>
	inline bool Manager<SceneType>::WantsToQuit() const noexcept
	{
		return wantsToQuit;
	}

	template<Enumeration SceneType>
	void Manager<SceneType>::UpdateCurrent()
	{
		scenes[static_cast<size_t>(current)]->Update();
		current = scenes[static_cast<size_t>(current)]->WantsToTransitionTo();
		wantsToQuit = current == SceneType::COUNT;
	}

	template<Enumeration SceneType>
	inline void Manager<SceneType>::DrawCurrent(Renderer& with) const
	{
		scenes[static_cast<size_t>(current)]->GetDrawn(with);
	}
}