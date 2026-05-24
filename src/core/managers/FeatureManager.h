#pragma once
#include <vector>
#include <memory>
#include "../interfaces/IFeature.h"

namespace core
{
	class FeatureManager
	{
	public:
		FeatureManager();
		~FeatureManager();

	public:
		template<typename T, typename... Args>
		T& Register(Args&&... args)
		{
			static_assert(std::derived_from<T, IFeature>);

			std::unique_ptr<T> feature = std::make_unique<T>(std::forward<Args>(args)...);
			T* ptr = feature.get();
			m_Features.emplace_back(std::move(feature));

			return *ptr;
		}

		void OnCreateMove();
		void OnFrameStageNotify();
		void OnRender();

	private:
		std::vector<std::unique_ptr<IFeature>> m_Features;
	};
}


