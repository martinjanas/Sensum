#pragma once
#include <vector>
#include <memory>
#include "../base/BaseFeature.h"

namespace core
{
	class FeatureManager
	{
	public:
		FeatureManager();
		~FeatureManager();

	public:
		template<typename T>
		void Register() requires std::derived_from<T, BaseFeature>
		{
			auto feature = std::make_unique<T>();
			m_Features.emplace_back(std::move(feature));
		}

		void OnCreateMove();
		void OnFrameStageNotify();
		void OnRender();

	private:
		std::vector<std::unique_ptr<BaseFeature>> m_Features;
	};
}


