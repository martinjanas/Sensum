#pragma once

#include <string>
#include <array>
#include <vector>

enum ECstrike15UserMessages
{
	CS_UM_VGUIMenu = 1,
	CS_UM_Geiger = 2,
	CS_UM_Train = 3,
	CS_UM_HudText = 4,
	CS_UM_SayText = 5,
	CS_UM_SayText2 = 6,
	CS_UM_TextMsg = 7,
	CS_UM_HudMsg = 8,
	CS_UM_ResetHud = 9,
	CS_UM_GameTitle = 10,
	CS_UM_Shake = 12,
	CS_UM_Fade = 13,
	CS_UM_Rumble = 14,
	CS_UM_CloseCaption = 15,
	CS_UM_CloseCaptionDirect = 16,
	CS_UM_SendAudio = 17,
	CS_UM_RawAudio = 18,
	CS_UM_VoiceMask = 19,
	CS_UM_RequestState = 20,
	CS_UM_Damage = 21,
	CS_UM_RadioText = 22,
	CS_UM_HintText = 23,
	CS_UM_KeyHintText = 24,
	CS_UM_ProcessSpottedEntityUpdate = 25,
	CS_UM_ReloadEffect = 26,
	CS_UM_AdjustMoney = 27,
	CS_UM_UpdateTeamMoney = 28,
	CS_UM_StopSpectatorMode = 29,
	CS_UM_KillCam = 30,
	CS_UM_DesiredTimescale = 31,
	CS_UM_CurrentTimescale = 32,
	CS_UM_AchievementEvent = 33,
	CS_UM_MatchEndConditions = 34,
	CS_UM_DisconnectToLobby = 35,
	CS_UM_PlayerStatsUpdate = 36,
	CS_UM_DisplayInventory = 37,
	CS_UM_WarmupHasEnded = 38,
	CS_UM_ClientInfo = 39,
	CS_UM_XRankGet = 40,
	CS_UM_XRankUpd = 41,
	CS_UM_CallVoteFailed = 45,
	CS_UM_VoteStart = 46,
	CS_UM_VotePass = 47,
	CS_UM_VoteFailed = 48,
	CS_UM_VoteSetup = 49,
	CS_UM_ServerRankRevealAll = 50,
	CS_UM_SendLastKillerDamageToClient = 51,
	CS_UM_ServerRankUpdate = 52,
	CS_UM_ItemPickup = 53,
	CS_UM_ShowMenu = 54,
	CS_UM_BarTime = 55,
	CS_UM_AmmoDenied = 56,
	CS_UM_MarkAchievement = 57,
	CS_UM_MatchStatsUpdate = 58,
	CS_UM_ItemDrop = 59,
	CS_UM_GlowPropTurnOff = 60,
	CS_UM_SendPlayerItemDrops = 61,
	CS_UM_RoundBackupFilenames = 62,
	CS_UM_SendPlayerItemFound = 63,
	CS_UM_ReportHit = 64,
	CS_UM_XpUpdate = 65,
	CS_UM_QuestProgress = 66,
	CS_UM_ScoreLeaderboardData = 67,
	CS_UM_PlayerDecalDigitalSignature = 68,
	MAX_ECSTRIKE15USERMESSAGES
};

enum EItemDefinitionIndex
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

enum EClassId : int
{
	CAI_BaseNPC = 0,
	CAK47 = 1,
	CBaseAnimating = 2,
	CBaseAnimatingOverlay = 3,
	CBaseAttributableItem = 4,
	CBaseButton = 5,
	CBaseCombatCharacter = 6,
	CBaseCombatWeapon = 7,
	CBaseCSGrenade = 8,
	CBaseCSGrenadeProjectile = 9,
	CBaseDoor = 10,
	CBaseEntity = 11,
	CBaseFlex = 12,
	CBaseGrenade = 13,
	CBaseParticleEntity = 14,
	CBasePlayer = 15,
	CBasePropDoor = 16,
	CBaseTeamObjectiveResource = 17,
	CBaseTempEntity = 18,
	CBaseToggle = 19,
	CBaseTrigger = 20,
	CBaseViewModel = 21,
	CBaseVPhysicsTrigger = 22,
	CBaseWeaponWorldModel = 23,
	CBeam = 24,
	CBeamSpotlight = 25,
	CBoneFollower = 26,
	CBRC4Target = 27,
	CBreachCharge = 28,
	CBreachChargeProjectile = 29,
	CBreakableProp = 30,
	CBreakableSurface = 31,
	CBumpMine = 32,
	CBumpMineProjectile = 33,
	CC4 = 34,
	CCascadeLight = 35,
	CChicken = 36,
	CColorCorrection = 37,
	CColorCorrectionVolume = 38,
	CCSGameRulesProxy = 39,
	CCSPlayer = 40,
	CCSPlayerResource = 41,
	CCSRagdoll = 42,
	CCSTeam = 43,
	CDangerZone = 44,
	CDangerZoneController = 45,
	CDEagle = 46,
	CDecoyGrenade = 47,
	CDecoyProjectile = 48,
	CDrone = 49,
	CDronegun = 50,
	CDynamicLight = 51,
	CDynamicProp = 52,
	CEconEntity = 53,
	CEconWearable = 54,
	CEmbers = 55,
	CEntityDissolve = 56,
	CEntityFlame = 57,
	CEntityFreezing = 58,
	CEntityParticleTrail = 59,
	CEnvAmbientLight = 60,
	CEnvDetailController = 61,
	CEnvDOFController = 62,
	CEnvGasCanister = 63,
	CEnvParticleScript = 64,
	CEnvProjectedTexture = 65,
	CEnvQuadraticBeam = 66,
	CEnvScreenEffect = 67,
	CEnvScreenOverlay = 68,
	CEnvTonemapController = 69,
	CEnvWind = 70,
	CFEPlayerDecal = 71,
	CFireCrackerBlast = 72,
	CFireSmoke = 73,
	CFireTrail = 74,
	CFish = 75,
	CFists = 76,
	CFlashbang = 77,
	CFogController = 78,
	CFootstepControl = 79,
	CFunc_Dust = 80,
	CFunc_LOD = 81,
	CFuncAreaPortalWindow = 82,
	CFuncBrush = 83,
	CFuncConveyor = 84,
	CFuncLadder = 85,
	CFuncMonitor = 86,
	CFuncMoveLinear = 87,
	CFuncOccluder = 88,
	CFuncReflectiveGlass = 89,
	CFuncRotating = 90,
	CFuncSmokeVolume = 91,
	CFuncTrackTrain = 92,
	CGameRulesProxy = 93,
	CGrassBurn = 94,
	CHandleTest = 95,
	CHEGrenade = 96,
	CHostage = 97,
	CHostageCarriableProp = 98,
	CIncendiaryGrenade = 99,
	CInferno = 100,
	CInfoLadderDismount = 101,
	CInfoMapRegion = 102,
	CInfoOverlayAccessor = 103,
	CItem_Healthshot = 104,
	CItemCash = 105,
	CItemDogtags = 106,
	CKnife = 107,
	CKnifeGG = 108,
	CLightGlow = 109,
	CMapVetoPickController = 110,
	CMaterialModifyControl = 111,
	CMelee = 112,
	CMolotovGrenade = 113,
	CMolotovProjectile = 114,
	CMovieDisplay = 115,
	CParadropChopper = 116,
	CParticleFire = 117,
	CParticlePerformanceMonitor = 118,
	CParticleSystem = 119,
	CPhysBox = 120,
	CPhysBoxMultiplayer = 121,
	CPhysicsProp = 122,
	CPhysicsPropMultiplayer = 123,
	CPhysMagnet = 124,
	CPhysPropAmmoBox = 125,
	CPhysPropLootCrate = 126,
	CPhysPropRadarJammer = 127,
	CPhysPropWeaponUpgrade = 128,
	CPlantedC4 = 129,
	CPlasma = 130,
	CPlayerPing = 131,
	CPlayerResource = 132,
	CPointCamera = 133,
	CPointCommentaryNode = 134,
	CPointWorldText = 135,
	CPoseController = 136,
	CPostProcessController = 137,
	CPrecipitation = 138,
	CPrecipitationBlocker = 139,
	CPredictedViewModel = 140,
	CProp_Hallucination = 141,
	CPropCounter = 142,
	CPropDoorRotating = 143,
	CPropJeep = 144,
	CPropVehicleDriveable = 145,
	CRagdollManager = 146,
	CRagdollProp = 147,
	CRagdollPropAttached = 148,
	CRopeKeyframe = 149,
	CSCAR17 = 150,
	CSceneEntity = 151,
	CSensorGrenade = 152,
	CSensorGrenadeProjectile = 153,
	CShadowControl = 154,
	CSlideshowDisplay = 155,
	CSmokeGrenade = 156,
	CSmokeGrenadeProjectile = 157,
	CSmokeStack = 158,
	CSnowball = 159,
	CSnowballPile = 160,
	CSnowballProjectile = 161,
	CSpatialEntity = 162,
	CSpotlightEnd = 163,
	CSprite = 164,
	CSpriteOriented = 165,
	CSpriteTrail = 166,
	CStatueProp = 167,
	CSteamJet = 168,
	CSun = 169,
	CSunlightShadowControl = 170,
	CSurvivalSpawnChopper = 171,
	CTablet = 172,
	CTeam = 173,
	CTeamplayRoundBasedRulesProxy = 174,
	CTEArmorRicochet = 175,
	CTEBaseBeam = 176,
	CTEBeamEntPoint = 177,
	CTEBeamEnts = 178,
	CTEBeamFollow = 179,
	CTEBeamLaser = 180,
	CTEBeamPoints = 181,
	CTEBeamRing = 182,
	CTEBeamRingPoint = 183,
	CTEBeamSpline = 184,
	CTEBloodSprite = 185,
	CTEBloodStream = 186,
	CTEBreakModel = 187,
	CTEBSPDecal = 188,
	CTEBubbles = 189,
	CTEBubbleTrail = 190,
	CTEClientProjectile = 191,
	CTEDecal = 192,
	CTEDust = 193,
	CTEDynamicLight = 194,
	CTEEffectDispatch = 195,
	CTEEnergySplash = 196,
	CTEExplosion = 197,
	CTEFireBullets = 198,
	CTEFizz = 199,
	CTEFootprintDecal = 200,
	CTEFoundryHelpers = 201,
	CTEGaussExplosion = 202,
	CTEGlowSprite = 203,
	CTEImpact = 204,
	CTEKillPlayerAttachments = 205,
	CTELargeFunnel = 206,
	CTEMetalSparks = 207,
	CTEMuzzleFlash = 208,
	CTEParticleSystem = 209,
	CTEPhysicsProp = 210,
	CTEPlantBomb = 211,
	CTEPlayerAnimEvent = 212,
	CTEPlayerDecal = 213,
	CTEProjectedDecal = 214,
	CTERadioIcon = 215,
	CTEShatterSurface = 216,
	CTEShowLine = 217,
	CTesla = 218,
	CTESmoke = 219,
	CTESparks = 220,
	CTESprite = 221,
	CTESpriteSpray = 222,
	CTest_ProxyToggle_Networkable = 223,
	CTestTraceline = 224,
	CTEWorldDecal = 225,
	CTriggerPlayerMovement = 226,
	CTriggerSoundOperator = 227,
	CVGuiScreen = 228,
	CVoteController = 229,
	CWaterBullet = 230,
	CWaterLODControl = 231,
	CWeaponAug = 232,
	CWeaponAWP = 233,
	CWeaponBaseItem = 234,
	CWeaponBizon = 235,
	CWeaponCSBase = 236,
	CWeaponCSBaseGun = 237,
	CWeaponCycler = 238,
	CWeaponElite = 239,
	CWeaponFamas = 240,
	CWeaponFiveSeven = 241,
	CWeaponG3SG1 = 242,
	CWeaponGalil = 243,
	CWeaponGalilAR = 244,
	CWeaponGlock = 245,
	CWeaponHKP2000 = 246,
	CWeaponM249 = 247,
	CWeaponM3 = 248,
	CWeaponM4A1 = 249,
	CWeaponMAC10 = 250,
	CWeaponMag7 = 251,
	CWeaponMP5Navy = 252,
	CWeaponMP7 = 253,
	CWeaponMP9 = 254,
	CWeaponNegev = 255,
	CWeaponNOVA = 256,
	CWeaponP228 = 257,
	CWeaponP250 = 258,
	CWeaponP90 = 259,
	CWeaponSawedoff = 260,
	CWeaponSCAR20 = 261,
	CWeaponScout = 262,
	CWeaponSG550 = 263,
	CWeaponSG552 = 264,
	CWeaponSG556 = 265,
	CWeaponShield = 266,
	CWeaponSSG08 = 267,
	CWeaponTaser = 268,
	CWeaponTec9 = 269,
	CWeaponTMP = 270,
	CWeaponUMP45 = 271,
	CWeaponUSP = 272,
	CWeaponXM1014 = 273,
	CWeaponZoneRepulsor = 274,
	CWorld = 275,
	CWorldVguiText = 276,
	DustTrail = 277,
	MovieExplosion = 278,
	ParticleSmokeGrenade = 279,
	RocketTrail = 280,
	SmokeTrail = 281,
	SporeExplosion = 282,
	SporeTrail = 283
};

enum ECSPlayerBone
{
	PELVIS = 0,
	SPINE_0,
	SPINE_1,
	SPINE_2,
	SPINE_3,
	NECK_0,
	HEAD_0,
	CLAVICLE_L,
	ARM_UPPER_L,
	ARM_LOWER_L,
	HAND_L,
	FINGER_MIDDLE_META_L,
	FINGER_MIDDLE_0_L,
	FINGER_MIDDLE_1_L,
	FINGER_MIDDLE_2_L,
	FINGER_PINKY_META_L,
	FINGER_PINKY_0_L,
	FINGER_PINKY_1_L,
	FINGER_PINKY_2_L,
	FINGER_INDEX_META_L,
	FINGER_INDEX_0_L,
	FINGER_INDEX_1_L,
	FINGER_INDEX_2_L,
	FINGER_THUMB_0_L,
	FINGER_THUMB_1_L,
	FINGER_THUMB_2_L,
	FINGER_RING_META_L,
	FINGER_RING_0_L,
	FINGER_RING_1_L,
	FINGER_RING_2_L,
	WEAPON_HAND_L,
	ARM_LOWER_L_TWIST,
	ARM_LOWER_L_TWIST1,
	ARM_UPPER_L_TWIST,
	ARM_UPPER_L_TWIST1,
	CLAVICLE_R,
	ARM_UPPER_R,
	ARM_LOWER_R,
	HAND_R,
	FINGER_MIDDLE_META_R,
	FINGER_MIDDLE_0_R,
	FINGER_MIDDLE_1_R,
	FINGER_MIDDLE_2_R,
	FINGER_PINKY_META_R,
	FINGER_PINKY_0_R,
	FINGER_PINKY_1_R,
	FINGER_PINKY_2_R,
	FINGER_INDEX_META_R,
	FINGER_INDEX_0_R,
	FINGER_INDEX_1_R,
	FINGER_INDEX_2_R,
	FINGER_THUMB_0_R,
	FINGER_THUMB_1_R,
	FINGER_THUMB_2_R,
	FINGER_RING_META_R,
	FINGER_RING_0_R,
	FINGER_RING_1_R,
	FINGER_RING_2_R,
	WEAPON_HAND_R,
	ARM_LOWER_R_TWIST,
	ARM_LOWER_R_TWIST1,
	ARM_UPPER_R_TWIST,
	ARM_UPPER_R_TWIST1,
	LEG_UPPER_L,
	LEG_LOWER_L,
	ANKLE_L,
	BALL_L,
	LFOOT_LOCK,
	LEG_UPPER_L_TWIST,
	LEG_UPPER_L_TWIST1,
	LEG_UPPER_R,
	LEG_LOWER_R,
	ANKLE_R,
	BALL_R,
	RFOOT_LOCK,
	LEG_UPPER_R_TWIST,
	LEG_UPPER_R_TWIST1,
	FINGER_PINKY_L_END,
	FINGER_PINKY_R_END,
	VALVEBIPED_WEAPON_BONE,
	LH_IK_DRIVER,
	PRIMARY_JIGGLE_JNT,
	MAX_ECSPLAYERBONES
};

enum EEntityFlag
{
	FL_ONGROUND = (1 << 0),
	FL_DUCKING = (1 << 1),
	FL_WATERJUMP = (1 << 2),
	FL_ONTRAIN = (1 << 3),
	FL_INRAIN = (1 << 4),
	FL_FROZEN = (1 << 5),
	FL_ATCONTROLS = (1 << 6),
	FL_CLIENT = (1 << 7),
	FL_FAKECLIENT = (1 << 8),
	MAX_ENTITYFLAGS
};

enum ELifeState : unsigned char
{
	LIFE_ALIVE = 0,// alive
	LIFE_DYING = 1, // playing death animation or still falling off of a ledge waiting to hit ground
	LIFE_DEAD = 2, // dead. lying still.
	MAX_LIFESTATE
};

enum EWeaponSound
{
	EMPTY,
	SINGLE,
	SINGLE_NPC,
	WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
	DOUBLE_NPC,
	BURST,
	RELOAD,
	RELOAD_NPC,
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,
	TAUNT,
	FAST_RELOAD,
	// Add new shoot sound types here
	REVERSE_THE_NEW_SOUND,
	NUM_SHOOT_SOUND_TYPES,
	MAX_WEAPONSOUND
};

enum EMoveType
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4,
	MAX_MOVETYPE
};

enum EClientFrameStage : int
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

enum EMaterialVarFlag
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),
	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	//MATERIAL_VAR_UNUSED = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE
	MATERIAL_VAR_UNUSED = (1 << 18), // UNUSED
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
	MATERIAL_VAR_VERTEXFOG = (1 << 31),
};

enum EBoxType : int
{
	Normal = 0,
	Corner
};

enum ELanguage : int
{
	English = 0,
	Russian
};

enum ESequence
{
	SEQUENCE_DEFAULT_DRAW = 0,
	SEQUENCE_DEFAULT_IDLE1 = 1,
	SEQUENCE_DEFAULT_IDLE2 = 2,
	SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
	SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
	SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
	SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
	SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
	SEQUENCE_DEFAULT_LOOKAT01 = 12,

	SEQUENCE_BUTTERFLY_DRAW = 0,
	SEQUENCE_BUTTERFLY_DRAW2 = 1,
	SEQUENCE_BUTTERFLY_IDLE1 = 2,
	SEQUENCE_BUTTERFLY_IDLE2 = 3,
	SEQUENCE_BUTTERFLY_LIGHT_MISS1 = 4,
	SEQUENCE_BUTTERFLY_LIGHT_MISS2 = 5,
	SEQUENCE_BUTTERFLY_LIGHT_HIT1 = 6,
	SEQUENCE_BUTTERFLY_LIGHT_HIT2 = 7,
	SEQUENCE_BUTTERFLY_LIGHT_BACKSTAB = 8,
	SEQUENCE_BUTTERFLY_LIGHT_BACKSTAB2 = 9,
	SEQUENCE_BUTTERFLY_HEAVY_MISS = 10,
	SEQUENCE_BUTTERFLY_HEAVY_HIT1 = 11,
	SEQUENCE_BUTTERFLY_HEAVY_BACKSTAB = 12,
	SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
	SEQUENCE_BUTTERFLY_LOOKAT02 = 14,
	SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

	SEQUENCE_FALCHION_IDLE1 = 1,
	SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
	SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
	SEQUENCE_FALCHION_LOOKAT01 = 12,
	SEQUENCE_FALCHION_LOOKAT02 = 13,

	SEQUENCE_DAGGERS_IDLE1 = 1,
	SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
	SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
	SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
	SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

	SEQUENCE_CSS_LOOKAT01 = 14, //was 14
	SEQUENCE_CSS_LOOKAT02 = 15,

	SEQUENCE_BOWIE_IDLE1 = 1,
};

enum team : int
{
	team_spec = 1,
	team_t = 2,
	team_ct = 3
};

struct sticker_setting
{
	int kit_index = 0;
	int kit_vector_index = 0;
	float wear = std::numeric_limits<float>::min();
	float scale = 1.f;
	float rotation = 0.f;
};

struct item_setting
{
	bool enabled = false;
	char name[32] = "Default";
	int definition_vector_index = 0;
	int definition_index = 1;
	int paint_kit_vector_index = 0;
	int paint_kit_index = 0;
	int definition_override_vector_index = 0;
	int definition_override_index = 0;
	int seed = 0;
	struct
	{
		bool enabled = false;
		int counter = 0;
	} stat_track;

	float wear = std::numeric_limits<float>::min();
	char custom_name[32] = "";
	std::array<sticker_setting, 5> stickers;
};

struct statrack_setting
{
	int definition_index = 1;
	struct
	{
		int counter = 0;
	}statrack_new;
};

struct weapon_kit_t
{
	int index;
	std::string rarity;
};

struct paint_kit_t
{
	int id;
	std::string english;
	std::vector<weapon_kit_t> weapons;
};

struct weapon_info_t
{
	constexpr weapon_info_t(const char* model, const char* icon = nullptr) :
		model(model),
		icon(icon)
	{}
	const char* model;
	const char* icon;
};

struct weapon_name_t
{
	constexpr weapon_name_t(const int definition_index, const char* name) :
		definition_index(definition_index),
		name(name)
	{}
	int definition_index = 0;
	const char* name = nullptr;
};