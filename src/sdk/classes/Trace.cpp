#include "Trace.h"

SurfaceData_t* GameTrace_t::GetSurfaceData()
{
	//return (SSurfaceData*)GEngine->Functions.GetSurfaceData(Surface);
	return nullptr;
}

//TraceFilter::TraceFilter(uint32_t mask, CCSPlayerController* controller_to_skip_one, CCSPlayerController* controller_to_skip_second, int layer)
//{
//	this->m_ui64lTraceMask = mask;
//	this->m_ui64lV1[0] = this->m_ui64lV1[1] = 0;
//	this->m_ui16V2 = 7;
//	this->m_ui8V3 = layer;
//	this->m_ui8V4 = 0x49;
//	this->m_ui8V5 = 0;
//
//	this->m_ui32SkipHandles[0] = controller_to_skip_one->GetEntityHandle();
//	this->m_ui32SkipHandles[1] = controller_to_skip_second->GetEntityHandle();
//	this->m_ui32SkipHandles[2] = controller_to_skip_one->GetOwnerHandle();
//	this->m_ui32SkipHandles[3] = controller_to_skip_second->GetOwnerHandle();
//
//	this->m_ui16Collisions[0] = Skip1->GetCollisionMask();
//	this->m_ui16Collisions[1] = Skip2->GetCollisionMask();
//}
//
//uint16_t SPlayerPawn::GetCollisionMask()
//{
//	if (this && Collision())
//		return Collision()->UnknownMask(); // Collision + 0x38
//	return 0;
//}
//
//uint32_t SPlayerPawn::GetOwnerHandle()
//{
//	uint32_t Result = -1;
//	if (this && Collision() && !(Collision()->SolidFlags() & 4))
//	{
//		auto* Entity = GEntityList->GetEntityFromHandle(this->OwnerEntityHandle());
//		if (Entity)
//		{
//			Result = Entity->GetEntityHandle();
//		}
//	}
//	return Result;
//}