#pragma once

#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"

class C_PostProcessingVolume
{
public:
  /*activePostProcessingVolume : setSchemaInt("C_PostProcessingVolume", "m_bExposureControl", 1)
    activePostProcessingVolume : setSchemaFloat("C_PostProcessingVolume", "m_flMinExposure", 0.15)
    activePostProcessingVolume : setSchemaFloat("C_PostProcessingVolume", "m_flMaxExposure", 0.15)*/

    NETVAR(int, m_bExposureControl, "C_PostProcessingVolume", "m_bExposureControl");
    NETVAR(float, m_flMinExposure, "C_PostProcessingVolume", "m_flMinExposure");
    NETVAR(float, m_flMaxExposure, "C_PostProcessingVolume", "m_flMaxExposure");
};

class CPlayer_CameraServices
{
public:

    NETVAR(C_PostProcessingVolume*, m_PostProcessingVolumes, "CPlayer_CameraServices", "m_PostProcessingVolumes");
};