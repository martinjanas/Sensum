#include "FeatureManager.h"

namespace core
{
	FeatureManager::FeatureManager()
	{

	}

	FeatureManager::~FeatureManager()
	{

	}

	void FeatureManager::OnCreateMove()
	{
		for (const auto& feature : m_Features)
		{
			if (!feature->IsEnabled())
				continue;

			feature->OnCreateMove();
		}
	}

	void FeatureManager::OnFrameStageNotify()
	{
		for (const auto& feature : m_Features)
		{
			if (!feature->IsEnabled())
				continue;

			feature->OnFrameStageNotify();
		}
	}

	void FeatureManager::OnRender()
	{
		for (const auto& feature : m_Features)
		{
			if (!feature->IsEnabled())
				continue;

			feature->OnRender();
		}
	}
}
