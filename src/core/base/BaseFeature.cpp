#include "BaseFeature.h"
#include "../core.h"

template <typename T>
T& BaseFeature::GetContext()
{
	return core::contexts().GetContext<T>();
}