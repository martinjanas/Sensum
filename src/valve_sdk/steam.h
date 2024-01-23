#pragma once

typedef int32_t HSteamPipe;
typedef int32_t HSteamUser;
typedef uint64_t SteamLeaderboard_t;
typedef uint64_t SteamLeaderboardEntries_t;
typedef uint64_t UGCHandle_t;

enum EGCResults
{
	k_EGCResultOK = 0,
	k_EGCResultNoMessage = 1,           // There is no message in the queue
	k_EGCResultBufferTooSmall = 2,      // The buffer is too small for the requested message
	k_EGCResultNotLoggedOn = 3,         // The client is not logged onto Steam
	k_EGCResultInvalidMessage = 4,      // Something was wrong with the message being sent with SendMessage
};

class ISteamGameCoordinator
{
public:
	virtual EGCResults GCSendMessage(int unMsgType, const void* pubData, int cubData) = 0;
	virtual bool IsMessageAvailable(int* pcubMsgSize) = 0;
	virtual EGCResults RetrieveMessage(int* punMsgType, void* pubDest, int cubDest, int* pcubMsgSize) = 0;
};

enum EUniverse
{
	k_EUniverseInvalid = 0,
	k_EUniversePublic = 1,
	k_EUniverseTestPublic = 2,
	k_EUniverseInternal = 3,

	k_EUniverseMax
};

enum EAccountType
{
	k_EAccountTypeInvalid = 0,
	k_EAccountTypeIndividual = 1,		// single user account
	k_EAccountTypeMultiseat = 2,		// multiseat (e.g. cybercafe) account
	k_EAccountTypeGameServer = 3,		// game server account
	k_EAccountTypeAnonGameServer = 4,	// anonomous game server account
	k_EAccountTypePending = 5			// pending
};

enum ELeaderboardSortMethod
{
	k_ELeaderboardSortMethodNone = 0,
	k_ELeaderboardSortMethodAscending = 1,	// top-score is lowest number
	k_ELeaderboardSortMethodDescending = 2,	// top-score is highest number
};

enum ELeaderboardDataRequest
{
	k_ELeaderboardDataRequestGlobal = 0,
	k_ELeaderboardDataRequestGlobalAroundUser = 1,
	k_ELeaderboardDataRequestFriends = 2,
	k_ELeaderboardDataRequestUsers = 3
};

enum ELeaderboardDisplayType
{
	k_ELeaderboardDisplayTypeNone = 0,
	k_ELeaderboardDisplayTypeNumeric = 1,			// simple numerical score
	k_ELeaderboardDisplayTypeTimeSeconds = 2,		// the score represents a time, in seconds
	k_ELeaderboardDisplayTypeTimeMilliSeconds = 3,	// the score represents a time, in milliseconds
};

enum ELeaderboardUploadScoreMethod
{
	k_ELeaderboardUploadScoreMethodNone = 0,
	k_ELeaderboardUploadScoreMethodKeepBest = 1,	// Leaderboard will keep user's best score
	k_ELeaderboardUploadScoreMethodForceUpdate = 2,	// Leaderboard will always replace score with specified
};

class CSteamID
{
public:
	CSteamID()
	{
		m_steamid.m_comp.m_unAccountID = 0;
		m_steamid.m_comp.m_EAccountType = 0;
		m_steamid.m_comp.m_EUniverse = 0;
		m_steamid.m_comp.m_unAccountInstance = 0;
	}

	CSteamID(uint64_t ulSteamID)
	{
		SetFromUint64(ulSteamID);
	}

	uint32_t GetAccountID() const { return m_steamid.m_comp.m_unAccountID; }

	void SetFromUint64(uint64_t ulSteamID)
	{
		m_steamid.m_comp.m_unAccountID = (ulSteamID & 0xFFFFFFFF);							// account ID is low 32 bits
		m_steamid.m_comp.m_unAccountInstance = ((ulSteamID >> 32) & 0xFFFFF);			// account instance is next 20 bits

		m_steamid.m_comp.m_EAccountType = (EAccountType)((ulSteamID >> 52) & 0xF);	// type is next 4 bits
		m_steamid.m_comp.m_EUniverse = (EUniverse)((ulSteamID >> 56) & 0xFF);			// universe is next 8 bits
	}

private:
	union SteamID_t
	{
		struct SteamIDComponent_t
		{
			uint32_t			m_unAccountID : 32;			// unique account identifier
			unsigned int		m_unAccountInstance : 20;	// dynamic instance ID (used for multiseat type accounts only)
			unsigned int		m_EAccountType : 4;			// type of account - can't show as EAccountType, due to signed / unsigned difference
			int					m_EUniverse : 8;	// universe this account belongs to
		} m_comp;

		uint64_t m_unAll64Bits;
	} m_steamid;
};

class ISteamUser
{
public:
	virtual uint32_t GetHSteamUser() = 0;
	virtual bool BLoggedOn() = 0;
	virtual CSteamID GetSteamID() = 0;
};

using HTTPRequestHandle = std::uint32_t;
enum : HTTPRequestHandle { INVALID_HTTPREQUEST_HANDLE = 0 };

using SteamAPICall_t = std::uint64_t;
enum : SteamAPICall_t { k_uAPICallInvalid = 0 };

struct LeaderboardEntry_t
{
	CSteamID m_steamIDUser; // user with the entry - use SteamFriends()->GetFriendPersonaName() & SteamFriends()->GetFriendAvatar() to get more info
	int m_nGlobalRank;	// [1..N], where N is the number of users with an entry in the leaderboard
	int m_nScore;			// score as set in the leaderboard
	int m_cDetails;		// number of int32 details available for this entry
	UGCHandle_t m_hUGC;		// handle for UGC attached to the entry
};

enum class EHTTPMethod
{
	Invalid = 0,
	GET,
	HEAD,
	POST
};

class ISteamHTTP
{
public:
	virtual HTTPRequestHandle CreateHTTPRequest(EHTTPMethod eHTTPRequestMethod, const char* pchAbsoluteURL) = 0;
	virtual bool SetHTTPRequestContextValue(HTTPRequestHandle hRequest, std::uint64_t ulContextValue) = 0;
	virtual bool SetHTTPRequestNetworkActivityTimeout(HTTPRequestHandle hRequest, std::uint32_t unTimeoutSeconds) = 0;
	virtual bool SetHTTPRequestHeaderValue(HTTPRequestHandle hRequest, const char* pchHeaderName, const char* pchHeaderValue) = 0;
	virtual bool SetHTTPRequestGetOrPostParameter(HTTPRequestHandle hRequest, const char* pchParamName, const char* pchParamValue) = 0;
	virtual bool SendHTTPRequest(HTTPRequestHandle hRequest, SteamAPICall_t* pCallHandle) = 0;
	virtual bool SendHTTPRequestAndStreamResponse(HTTPRequestHandle hRequest, SteamAPICall_t* pCallHandle) = 0;
	virtual bool DeferHTTPRequest(HTTPRequestHandle hRequest) = 0;
	virtual bool PrioritizeHTTPRequest(HTTPRequestHandle hRequest) = 0;
	virtual bool GetHTTPResponseHeaderSize(HTTPRequestHandle hRequest, const char* pchHeaderName, std::uint32_t* unResponseHeaderSize) = 0;
	virtual bool GetHTTPResponseHeaderValue(HTTPRequestHandle hRequest, const char* pchHeaderName, std::uint8_t* pHeaderValueBuffer, std::uint32_t unBufferSize) = 0;
	virtual bool GetHTTPResponseBodySize(HTTPRequestHandle hRequest, std::uint32_t* unBodySize) = 0;
	virtual bool GetHTTPResponseBodyData(HTTPRequestHandle hRequest, std::uint8_t* pBodyDataBuffer, std::uint32_t unBufferSize) = 0;
	virtual bool GetHTTPStreamingResponseBodyData(HTTPRequestHandle hRequest, std::uint32_t cOffset, std::uint8_t* pBodyDataBuffer, std::uint32_t unBufferSize) = 0;
	virtual bool ReleaseHTTPRequest(HTTPRequestHandle hRequest) = 0;
	virtual bool GetHTTPDownloadProgressPct(HTTPRequestHandle hRequest, float* pflPercentOut) = 0;
	virtual bool SetHTTPRequestRawPostBody(HTTPRequestHandle hRequest, const char* pchContentType, std::uint8_t* pubBody, std::uint32_t unBodyLen) = 0;
};

class ISteamFriends
{
public:
	// returns the local players name - guaranteed to not be NULL.
	// this is the same name as on the users community profile page
	// this is stored in UTF-8 format
	// like all the other interface functions that return a char *, it's important that this pointer is not saved
	// off; it will eventually be free'd or re-allocated
	virtual const char* GetPersonaName() = 0;

	// Sets the player name, stores it on the server and publishes the changes to all friends who are online.
	// Changes take place locally immediately, and a PersonaStateChange_t is posted, presuming success.
	//
	// The final results are available through the return value SteamAPICall_t, using SetPersonaNameResponse_t.
	//
	// If the name change fails to happen on the server, then an additional global PersonaStateChange_t will be posted
	// to change the name back, in addition to the SetPersonaNameResponse_t callback.
	virtual void SetPersonaName(const char* pchPersonaName) = 0;

	// gets the status of the current user
	virtual void GetPersonaState() = 0;

	// friend iteration
	// takes a set of k_EFriendFlags, and returns the number of users the client knows about who meet that criteria
	// then GetFriendByIndex() can then be used to return the id's of each of those users
	virtual int GetFriendCount(int iFriendFlags) = 0;

	// returns the steamID of a user
	// iFriend is a index of range [0, GetFriendCount())
	// iFriendsFlags must be the same value as used in GetFriendCount()
	// the returned CSteamID can then be used by all the functions below to access details about the user
	virtual CSteamID GetFriendByIndex(int iFriend, int iFriendFlags) = 0;

	// returns a relationship to a user
	virtual void GetFriendRelationship(CSteamID steamIDFriend) = 0;

	// returns the current status of the specified user
	// this will only be known by the local user if steamIDFriend is in their friends list; on the same game server; in a chat room or lobby; or in a small group with the local user
	virtual void GetFriendPersonaState(CSteamID steamIDFriend) = 0;

	// returns the name another user - guaranteed to not be NULL.
	// same rules as GetFriendPersonaState() apply as to whether or not the user knowns the name of the other user
	// note that on first joining a lobby, chat room or game server the local user will not known the name of the other users automatically; that information will arrive asyncronously
	// 
	virtual const char* GetFriendPersonaName(CSteamID steamIDFriend) = 0;

	// returns true if the friend is actually in a game, and fills in pFriendGameInfo with an extra details 
	virtual bool GetFriendGamePlayed(CSteamID steamIDFriend, void* pFriendGameInfo) = 0;
	// accesses old friends names - returns an empty string when their are no more items in the history
	virtual const char* GetFriendPersonaNameHistory(CSteamID steamIDFriend, int iPersonaName) = 0;
	// friends steam level
	virtual int GetFriendSteamLevel(CSteamID steamIDFriend) = 0;

	// Returns nickname the current user has set for the specified player. Returns NULL if the no nickname has been set for that player.
	virtual const char* GetPlayerNickname(CSteamID steamIDPlayer) = 0;

	// friend grouping (tag) apis
	// returns the number of friends groups
	virtual int GetFriendsGroupCount() = 0;
	// returns the friends group ID for the given index (invalid indices return k_FriendsGroupID_Invalid)
	virtual void GetFriendsGroupIDByIndex(int iFG) = 0;
	// returns the name for the given friends group (NULL in the case of invalid friends group IDs)
	virtual const char* GetFriendsGroupName(int friendsGroupID) = 0;
	// returns the number of members in a given friends group
	virtual int GetFriendsGroupMembersCount(int friendsGroupID) = 0;
	// gets up to nMembersCount members of the given friends group, if fewer exist than requested those positions' SteamIDs will be invalid
	virtual void GetFriendsGroupMembersList(int friendsGroupID, int nMembersCount) = 0;

	// returns true if the specified user meets any of the criteria specified in iFriendFlags
	// iFriendFlags can be the union (binary or, |) of one or more k_EFriendFlags values
	virtual bool HasFriend(CSteamID steamIDFriend, int iFriendFlags) = 0;

	// clan (group) iteration and access functions
	virtual int GetClanCount() = 0;
	virtual CSteamID GetClanByIndex(int iClan) = 0;
	virtual const char* GetClanName(CSteamID steamIDClan) = 0;
	virtual const char* GetClanTag(CSteamID steamIDClan) = 0;
	// returns the most recent information we have about what's happening in a clan
	virtual bool GetClanActivityCounts(CSteamID steamIDClan, int* pnOnline, int* pnInGame, int* pnChatting) = 0;
	// for clans a user is a member of, they will have reasonably up-to-date information, but for others you'll have to download the info to have the latest
	virtual SteamAPICall_t DownloadClanActivityCounts(int cClansToRequest) = 0;

	// iterators for getting users in a chat room, lobby, game server or clan
	// note that large clans that cannot be iterated by the local user
	// note that the current user must be in a lobby to retrieve CSteamIDs of other users in that lobby
	// steamIDSource can be the steamID of a group, game server, lobby or chat room
	virtual int GetFriendCountFromSource(CSteamID steamIDSource) = 0;
	virtual CSteamID GetFriendFromSourceByIndex(CSteamID steamIDSource, int iFriend) = 0;

	// returns true if the local user can see that steamIDUser is a member or in steamIDSource
	virtual bool IsUserInSource(CSteamID steamIDUser, CSteamID steamIDSource) = 0;

	// User is in a game pressing the talk button (will suppress the microphone for all voice comms from the Steam friends UI)
	virtual void SetInGameVoiceSpeaking(CSteamID steamIDUser, bool bSpeaking) = 0;

	// activates the game overlay, with an optional dialog to open 
	// valid options are "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements"
	virtual void ActivateGameOverlay(const char* pchDialog) = 0;

	// activates game overlay to a specific place
	// valid options are
	//		"steamid" - opens the overlay web browser to the specified user or groups profile
	//		"chat" - opens a chat window to the specified user, or joins the group chat 
	//		"jointrade" - opens a window to a Steam Trading session that was started with the ISteamEconomy/StartTrade Web API
	//		"stats" - opens the overlay web browser to the specified user's stats
	//		"achievements" - opens the overlay web browser to the specified user's achievements
	//		"friendadd" - opens the overlay in minimal mode prompting the user to add the target user as a friend
	//		"friendremove" - opens the overlay in minimal mode prompting the user to remove the target friend
	//		"friendrequestaccept" - opens the overlay in minimal mode prompting the user to accept an incoming friend invite
	//		"friendrequestignore" - opens the overlay in minimal mode prompting the user to ignore an incoming friend invite
	virtual void ActivateGameOverlayToUser(const char* pchDialog, CSteamID steamID) = 0;

	// activates game overlay web browser directly to the specified URL
	// full address with protocol type is required, e.g. http://www.steamgames.com/
	virtual void ActivateGameOverlayToWebPage(const char* pchURL) = 0;

	// activates game overlay to store page for app
	virtual void ActivateGameOverlayToStore(int nAppID, int eFlag) = 0;

	// Mark a target user as 'played with'. This is a client-side only feature that requires that the calling user is 
	// in game 
	virtual void SetPlayedWith(CSteamID steamIDUserPlayedWith) = 0;

	// activates game overlay to open the invite dialog. Invitations will be sent for the provided lobby.
	virtual void ActivateGameOverlayInviteDialog(CSteamID steamIDLobby) = 0;

	// gets the small (32x32) avatar of the current user, which is a handle to be used in IClientUtils::GetImageRGBA(), or 0 if none set
	virtual int GetSmallFriendAvatar(CSteamID steamIDFriend) = 0;

	// gets the medium (64x64) avatar of the current user, which is a handle to be used in IClientUtils::GetImageRGBA(), or 0 if none set
	virtual int GetMediumFriendAvatar(CSteamID steamIDFriend) = 0;

	// gets the large (184x184) avatar of the current user, which is a handle to be used in IClientUtils::GetImageRGBA(), or 0 if none set
	// returns -1 if this image has yet to be loaded, in this case wait for a AvatarImageLoaded_t callback and then call this again
	virtual int GetLargeFriendAvatar(CSteamID steamIDFriend) = 0;

	// requests information about a user - persona name & avatar
	// if bRequireNameOnly is set, then the avatar of a user isn't downloaded 
	// - it's a lot slower to download avatars and churns the local cache, so if you don't need avatars, don't request them
	// if returns true, it means that data is being requested, and a PersonaStateChanged_t callback will be posted when it's retrieved
	// if returns false, it means that we already have all the details about that user, and functions can be called immediately
	virtual bool RequestUserInformation(CSteamID steamIDUser, bool bRequireNameOnly) = 0;

	// requests information about a clan officer list
	// when complete, data is returned in ClanOfficerListResponse_t call result
	// this makes available the calls below
	// you can only ask about clans that a user is a member of
	// note that this won't download avatars automatically; if you get an officer,
	// and no avatar image is available, call RequestUserInformation( steamID, false ) to download the avatar
	virtual void RequestClanOfficerList(CSteamID steamIDClan) = 0;

	// iteration of clan officers - can only be done when a RequestClanOfficerList() call has completed

	// returns the steamID of the clan owner
	virtual CSteamID GetClanOwner(CSteamID steamIDClan) = 0;
	// returns the number of officers in a clan (including the owner)
	virtual int GetClanOfficerCount(CSteamID steamIDClan) = 0;
	// returns the steamID of a clan officer, by index, of range [0,GetClanOfficerCount)
	virtual CSteamID GetClanOfficerByIndex(CSteamID steamIDClan, int iOfficer) = 0;
	// if current user is chat restricted, he can't send or receive any text/voice chat messages.
	// the user can't see custom avatars. But the user can be online and send/recv game invites.
	// a chat restricted user can't add friends or join any groups.
	virtual int GetUserRestrictions() = 0;

	// Rich Presence data is automatically shared between friends who are in the same game
	// Each user has a set of Key/Value pairs
	// Up to 20 different keys can be set
	// There are two magic keys:
	//		"status"  - a UTF-8 string that will show up in the 'view game info' dialog in the Steam friends list
	//		"connect" - a UTF-8 string that contains the command-line for how a friend can connect to a game
	// GetFriendRichPresence() returns an empty string "" if no value is set
	// SetRichPresence() to a NULL or an empty string deletes the key
	// You can iterate the current set of keys for a friend with GetFriendRichPresenceKeyCount()
	// and GetFriendRichPresenceKeyByIndex() (typically only used for debugging)
	virtual bool SetRichPresence(const char* pchKey, const char* pchValue) = 0;
	virtual void ClearRichPresence() = 0;
	virtual const char* GetFriendRichPresence(CSteamID steamIDFriend, const char* pchKey) = 0;
	virtual int GetFriendRichPresenceKeyCount(CSteamID steamIDFriend) = 0;
	virtual const char* GetFriendRichPresenceKeyByIndex(CSteamID steamIDFriend, int iKey) = 0;
	// Requests rich presence for a specific user.
	virtual void RequestFriendRichPresence(CSteamID steamIDFriend) = 0;

	// rich invite support
	// if the target accepts the invite, the pchConnectString gets added to the command-line for launching the game
	// if the game is already running, a GameRichPresenceJoinRequested_t callback is posted containing the connect string
	// invites can only be sent to friends
	virtual bool InviteUserToGame(CSteamID steamIDFriend, const char* pchConnectString) = 0;

	// recently-played-with friends iteration
	// this iterates the entire list of users recently played with, across games
	// GetFriendCoplayTime() returns as a unix time
	virtual int GetCoplayFriendCount() = 0;
	virtual CSteamID GetCoplayFriend(int iCoplayFriend) = 0;
	virtual int GetFriendCoplayTime(CSteamID steamIDFriend) = 0;
	virtual void GetFriendCoplayGame(CSteamID steamIDFriend) = 0;

	// chat interface for games
	// this allows in-game access to group (clan) chats from in the game
	// the behavior is somewhat sophisticated, because the user may or may not be already in the group chat from outside the game or in the overlay
	// use ActivateGameOverlayToUser( "chat", steamIDClan ) to open the in-game overlay version of the chat
	virtual SteamAPICall_t JoinClanChatRoom(CSteamID steamIDClan) = 0;
	virtual bool LeaveClanChatRoom(CSteamID steamIDClan) = 0;
	virtual int GetClanChatMemberCount(CSteamID steamIDClan) = 0;
	virtual CSteamID GetChatMemberByIndex(CSteamID steamIDClan, int iUser) = 0;
	virtual bool SendClanChatMessage(CSteamID steamIDClanChat, const char* pchText) = 0;
	virtual int GetClanChatMessage(CSteamID steamIDClanChat, int iMessage, void* prgchText, int cchTextMax, void* peChatEntryType, CSteamID* psteamidChatter) = 0;
	virtual bool IsClanChatAdmin(CSteamID steamIDClanChat, CSteamID steamIDUser) = 0;

	// interact with the Steam (game overlay / desktop)
	virtual bool IsClanChatWindowOpenInSteam(CSteamID steamIDClanChat) = 0;
	virtual bool OpenClanChatWindowInSteam(CSteamID steamIDClanChat) = 0;
	virtual bool CloseClanChatWindowInSteam(CSteamID steamIDClanChat) = 0;

	// peer-to-peer chat interception
	// this is so you can show P2P chats inline in the game
	virtual bool SetListenForFriendsMessages(bool bInterceptEnabled) = 0;
	virtual bool ReplyToFriendMessage(CSteamID steamIDFriend, const char* pchMsgToSend) = 0;
	virtual int GetFriendMessage(CSteamID steamIDFriend, int iMessageID, void* pvData, int cubData, void* peChatEntryType) = 0;

	// following apis
		virtual SteamAPICall_t GetFollowerCount(CSteamID steamID) = 0;
		virtual SteamAPICall_t IsFollowing(CSteamID steamID) = 0;
		virtual SteamAPICall_t EnumerateFollowingList(int unStartIndex) = 0;
};

class ISteamUtils
{
public:
	// return the number of seconds since the user 
	virtual unsigned int GetSecondsSinceAppActive() = 0;
	virtual unsigned int GetSecondsSinceComputerActive() = 0;

	// the universe this client is connecting to
	virtual void* GetConnectedUniverse() = 0;

	// Steam server time - in PST, number of seconds since January 1, 1970 (i.e unix time)
	virtual unsigned int GetServerRealTime() = 0;

	// returns the 2 digit ISO 3166-1-alpha-2 format country code this client is running in (as looked up via an IP-to-location database)
	// e.g "US" or "UK".
	virtual const char* GetIPCountry() = 0;

	// returns true if the image exists, and valid sizes were filled out
	virtual bool GetImageSize(int iImage, unsigned int* pnWidth, unsigned int* pnHeight) = 0;

	// returns true if the image exists, and the buffer was successfully filled out
	// results are returned in RGBA format
	// the destination buffer size should be 4 * height * width * sizeof(char)
	virtual bool GetImageRGBA(int iImage, unsigned int* pubDest, int nDestBufferSize) = 0;

	// returns the IP of the reporting server for valve - currently only used in Source engine games
	virtual bool GetCSERIPPort(unsigned int* unIP, unsigned short* usPort) = 0;

	// return the amount of battery power left in the current system in % [0..100], 255 for being on AC power
	virtual unsigned char GetCurrentBatteryPower() = 0;

	// returns the appID of the current process
	virtual unsigned int GetAppID() = 0;

	// Sets the position where the overlay instance for the currently calling game should show notifications.
	// This position is per-game and if this function is called from outside of a game context it will do nothing.
	virtual void SetOverlayNotificationPosition(void* eNotificationPosition) = 0;

	// API asynchronous call results
	// can be used directly, but more commonly used via the callback dispatch API (see steam_api.h)
	virtual bool IsAPICallCompleted(SteamAPICall_t hSteamAPICall, bool* pbFailed) = 0;
	virtual void* GetAPICallFailureReason(SteamAPICall_t hSteamAPICall) = 0;
	virtual bool GetAPICallResult(SteamAPICall_t hSteamAPICall, void* pCallback, int cubCallback, int iCallbackExpected, bool* pbFailed) = 0;

	// Deprecated. Applications should use SteamAPI_RunCallbacks() instead. Game servers do not need to call this function.
	//virtual void RunFrame() = 0;

	// returns the number of IPC calls made since the last time this function was called
	// Used for perf debugging so you can understand how many IPC calls your game makes per frame
	// Every IPC call is at minimum a thread context switch if not a process one so you want to rate
	// control how often you do them.
	virtual unsigned int GetIPCCallCount() = 0;

	// API warning handling
	// 'int' is the severity; 0 for msg, 1 for warning
	// 'const char *' is the text of the message
	// callbacks will occur directly after the API function is called that generated the warning or message
	virtual void SetWarningMessageHook(void* pFunction) = 0;

	// Returns true if the overlay is running & the user can access it. The overlay process could take a few seconds to
	// start & hook the game process, so this function will initially return false while the overlay is loading.
	virtual bool IsOverlayEnabled() = 0;

	// Normally this call is unneeded if your game has a constantly running frame loop that calls the 
	// D3D Present API, or OGL SwapBuffers API every frame.
	//
	// However, if you have a game that only refreshes the screen on an event driven basis then that can break 
	// the overlay, as it uses your Present/SwapBuffers calls to drive it's internal frame loop and it may also
	// need to Present() to the screen any time an even needing a notification happens or when the overlay is
	// brought up over the game by a user.  You can use this API to ask the overlay if it currently need a present
	// in that case, and then you can check for this periodically (roughly 33hz is desirable) and make sure you
	// refresh the screen with Present or SwapBuffers to allow the overlay to do it's work.
	virtual bool BOverlayNeedsPresent() = 0;

	// Asynchronous call to check if an executable file has been signed using the public key set on the signing tab
	// of the partner site, for example to refuse to load modified executable files.  
	// The result is returned in CheckFileSignature_t.
	//   k_ECheckFileSignatureNoSignaturesFoundForThisApp - This app has not been configured on the signing tab of the partner site to enable this function.
	//   k_ECheckFileSignatureNoSignaturesFoundForThisFile - This file is not listed on the signing tab for the partner site.
	//   k_ECheckFileSignatureFileNotFound - The file does not exist on disk.
	//   k_ECheckFileSignatureInvalidSignature - The file exists, and the signing tab has been set for this file, but the file is either not signed or the signature does not match.
	//   k_ECheckFileSignatureValidSignature - The file is signed and the signature is valid.
	virtual SteamAPICall_t CheckFileSignature(const char* szFileName) = 0;

	// Activates the Big Picture text input dialog which only supports gamepad input
	virtual bool ShowGamepadTextInput(void* eInputMode, void* eLineInputMode, const char* pchDescription, unsigned int unCharMax, const char* pchExistingText) = 0;

	// Returns previously entered text & length
	virtual unsigned int GetEnteredGamepadTextLength() = 0;
	virtual bool GetEnteredGamepadTextInput(char* pchText, unsigned int cchText) = 0;

	// returns the language the steam client is running in, you probably want ISteamApps::GetCurrentGameLanguage instead, this is for very special usage cases
	virtual const char* GetSteamUILanguage() = 0;

	// returns true if Steam itself is running in VR mode
	virtual bool IsSteamRunningInVR() = 0;

	// Sets the inset of the overlay notification from the corner specified by SetOverlayNotificationPosition.
	virtual void SetOverlayNotificationInset(int nHorizontalInset, int nVerticalInset) = 0;

	// returns true if Steam & the Steam Overlay are running in Big Picture mode
	// Games much be launched through the Steam client to enable the Big Picture overlay. During development,
	// a game can be added as a non-steam game to the developers library to test this feature
	virtual bool IsSteamInBigPictureMode() = 0;

	// ask SteamUI to create and render its OpenVR dashboard
	virtual void StartVRDashboard() = 0;
};

class ISteamUserStats
{
public:
	// Ask the server to send down this user's data and achievements for this game
	virtual bool RequestCurrentStats() = 0;

	// Data accessors
	virtual bool GetStat(const char* pchName, int* pData) = 0;

	virtual bool GetStat(const char* pchName, float* pData) = 0;

	// Set / update data
	virtual bool SetStat(const char* pchName, int nData) = 0;

	virtual bool SetStat(const char* pchName, float fData) = 0;

	virtual bool UpdateAvgRateStat(const char* pchName, float flCountThisSession, double dSessionLength) = 0;

	// Achievement flag accessors
	virtual bool GetAchievement(const char* pchName, bool* pbAchieved) = 0;
	virtual bool SetAchievement(const char* pchName) = 0;
	virtual bool ClearAchievement(const char* pchName) = 0;

	// Get the achievement status, and the time it was unlocked if unlocked.
	// If the return value is true, but the unlock time is zero, that means it was unlocked before Steam 
	// began tracking achievement unlock times (December 2009). Time is seconds since January 1, 1970.
	virtual bool GetAchievementAndUnlockTime(const char* pchName, bool* pbAchieved, uint32_t* punUnlockTime) = 0;

	// Store the current data on the server, will get a callback when set
	// And one callback for every new achievement
	//
	// If the callback has a result of k_EResultInvalidParam, one or more stats 
	// uploaded has been rejected, either because they broke constraints
	// or were out of date. In this case the server sends back updated values.
	// The stats should be re-iterated to keep in sync.
	virtual bool StoreStats() = 0;

	// Achievement / GroupAchievement metadata

	// Gets the icon of the achievement, which is a handle to be used in ISteamUtils::GetImageRGBA(), or 0 if none set. 
	// A return value of 0 may indicate we are still fetching data, and you can wait for the UserAchievementIconFetched_t callback
	// which will notify you when the bits are ready. If the callback still returns zero, then there is no image set for the
	// specified achievement.
	virtual int GetAchievementIcon(const char* pchName) = 0;

	// Get general attributes for an achievement. Accepts the following keys:
	// - "name" and "desc" for retrieving the localized achievement name and description (returned in UTF8)
	// - "hidden" for retrieving if an achievement is hidden (returns "0" when not hidden, "1" when hidden)
	virtual const char* GetAchievementDisplayAttribute(const char* pchName, const char* pchKey) = 0;

	// Achievement progress - triggers an AchievementProgress callback, that is all.
	// Calling this w/ N out of N progress will NOT set the achievement, the game must still do that.
	virtual bool IndicateAchievementProgress(const char* pchName, uint32_t nCurProgress, uint32_t nMaxProgress) = 0;

	// Used for iterating achievements. In general games should not need these functions because they should have a
	// list of existing achievements compiled into them
	virtual uint32_t GetNumAchievements() = 0;
	// Get achievement name iAchievement in [0,GetNumAchievements)
	virtual const char* GetAchievementName(uint32_t iAchievement) = 0;

	// Friends stats & achievements

	// downloads stats for the user
	// returns a UserStatsReceived_t received when completed
	// if the other user has no stats, UserStatsReceived_t.m_eResult will be set to k_EResultFail
	// these stats won't be auto-updated; you'll need to call RequestUserStats() again to refresh any data
	virtual SteamAPICall_t RequestUserStats(CSteamID steamIDUser) = 0;

	// requests stat information for a user, usable after a successful call to RequestUserStats()
	virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, int* pData) = 0;

	virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, float* pData) = 0;

	virtual bool GetUserAchievement(CSteamID steamIDUser, const char* pchName, bool* pbAchieved) = 0;
	// See notes for GetAchievementAndUnlockTime above
	virtual bool GetUserAchievementAndUnlockTime(CSteamID steamIDUser, const char* pchName, bool* pbAchieved, uint32_t* punUnlockTime) = 0;

	// Reset stats 
	virtual bool ResetAllStats(bool bAchievementsToo) = 0;

	// Leaderboard functions

	// asks the Steam back-end for a leaderboard by name, and will create it if it's not yet
	// This call is asynchronous, with the result returned in LeaderboardFindResult_t
	virtual SteamAPICall_t FindOrCreateLeaderboard(const char* pchLeaderboardName, ELeaderboardSortMethod eLeaderboardSortMethod, ELeaderboardDisplayType eLeaderboardDisplayType) = 0;

	// as above, but won't create the leaderboard if it's not found
	// This call is asynchronous, with the result returned in LeaderboardFindResult_t
	virtual SteamAPICall_t FindLeaderboard(const char* pchLeaderboardName) = 0;

	// returns the name of a leaderboard
	virtual const char* GetLeaderboardName(SteamLeaderboard_t hSteamLeaderboard) = 0;

	// returns the total number of entries in a leaderboard, as of the last request
	virtual int GetLeaderboardEntryCount(SteamLeaderboard_t hSteamLeaderboard) = 0;

	// returns the sort method of the leaderboard
	virtual ELeaderboardSortMethod GetLeaderboardSortMethod(SteamLeaderboard_t hSteamLeaderboard) = 0;

	// returns the display type of the leaderboard
	virtual ELeaderboardDisplayType GetLeaderboardDisplayType(SteamLeaderboard_t hSteamLeaderboard) = 0;

	// Asks the Steam back-end for a set of rows in the leaderboard.
	// This call is asynchronous, with the result returned in LeaderboardScoresDownloaded_t
	// LeaderboardScoresDownloaded_t will contain a handle to pull the results from GetDownloadedLeaderboardEntries() (below)
	// You can ask for more entries than exist, and it will return as many as do exist.
	// k_ELeaderboardDataRequestGlobal requests rows in the leaderboard from the full table, with nRangeStart & nRangeEnd in the range [1, TotalEntries]
	// k_ELeaderboardDataRequestGlobalAroundUser requests rows around the current user, nRangeStart being negate
	//   e.g. DownloadLeaderboardEntries( hLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, -3, 3 ) will return 7 rows, 3 before the user, 3 after
	// k_ELeaderboardDataRequestFriends requests all the rows for friends of the current user 
	virtual SteamAPICall_t DownloadLeaderboardEntries(SteamLeaderboard_t hSteamLeaderboard, ELeaderboardDataRequest eLeaderboardDataRequest, int nRangeStart, int nRangeEnd) = 0;

	// as above, but downloads leaderboard entries for an arbitrary set of users - ELeaderboardDataRequest is k_ELeaderboardDataRequestUsers
	// if a user doesn't have a leaderboard entry, they won't be included in the result
	// a max of 100 users can be downloaded at a time, with only one outstanding call at a time
	virtual SteamAPICall_t DownloadLeaderboardEntriesForUsers(SteamLeaderboard_t hSteamLeaderboard, CSteamID* prgUsers, int cUsers) = 0;

	// Returns data about a single leaderboard entry
	// use a for loop from 0 to LeaderboardScoresDownloaded_t::m_cEntryCount to get all the downloaded entries
	// e.g.
	//		void OnLeaderboardScoresDownloaded( LeaderboardScoresDownloaded_t *pLeaderboardScoresDownloaded )
	//		{
	//			for ( int index = 0; index < pLeaderboardScoresDownloaded->m_cEntryCount; index++ )
	//			{
	//				LeaderboardEntry_t leaderboardEntry;
	//				int32 details[3];		// we know this is how many we've stored previously
	//				GetDownloadedLeaderboardEntry( pLeaderboardScoresDownloaded->m_hSteamLeaderboardEntries, index, &leaderboardEntry, details, 3 );
	//				assert( leaderboardEntry.m_cDetails == 3 );
	//				...
	//			}
	// once you've accessed all the entries, the data will be free'd, and the SteamLeaderboardEntries_t handle will become invalid
	virtual bool GetDownloadedLeaderboardEntry(SteamLeaderboardEntries_t hSteamLeaderboardEntries, int index, LeaderboardEntry_t* pLeaderboardEntry, int* pDetails, int cDetailsMax) = 0;

	// Uploads a user score to the Steam back-end.
	// This call is asynchronous, with the result returned in LeaderboardScoreUploaded_t
	// Details are extra game-defined information regarding how the user got that score
	// pScoreDetails points to an array of int32's, cScoreDetailsCount is the number of int32's in the list
	virtual SteamAPICall_t UploadLeaderboardScore(SteamLeaderboard_t hSteamLeaderboard, ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod, int nScore, const int* pScoreDetails, int cScoreDetailsCount) = 0;

	// Attaches a piece of user generated content the user's entry on a leaderboard.
	// hContent is a handle to a piece of user generated content that was shared using ISteamUserRemoteStorage::FileShare().
	// This call is asynchronous, with the result returned in LeaderboardUGCSet_t.
	virtual SteamAPICall_t AttachLeaderboardUGC(SteamLeaderboard_t hSteamLeaderboard, UGCHandle_t hUGC) = 0;

	// Retrieves the number of players currently playing your game (online + offline)
	// This call is asynchronous, with the result returned in NumberOfCurrentPlayers_t
	virtual SteamAPICall_t GetNumberOfCurrentPlayers() = 0;

	// Requests that Steam fetch data on the percentage of players who have received each achievement
	// for the game globally.
	// This call is asynchronous, with the result returned in GlobalAchievementPercentagesReady_t.
	virtual SteamAPICall_t RequestGlobalAchievementPercentages() = 0;

	// Get the info on the most achieved achievement for the game, returns an iterator index you can use to fetch
	// the next most achieved afterwards.  Will return -1 if there is no data on achievement 
	// percentages (ie, you haven't called RequestGlobalAchievementPercentages and waited on the callback).
	virtual int GetMostAchievedAchievementInfo(char* pchName, uint32_t unNameBufLen, float* pflPercent, bool* pbAchieved) = 0;

	// Get the info on the next most achieved achievement for the game. Call this after GetMostAchievedAchievementInfo or another
	// GetNextMostAchievedAchievementInfo call passing the iterator from the previous call. Returns -1 after the last
	// achievement has been iterated.
	virtual int GetNextMostAchievedAchievementInfo(int iIteratorPrevious, char* pchName, uint32_t unNameBufLen, float* pflPercent, bool* pbAchieved) = 0;

	// Returns the percentage of users who have achieved the specified achievement.
	virtual bool GetAchievementAchievedPercent(const char* pchName, float* pflPercent) = 0;

	// Requests global stats data, which is available for stats marked as "aggregated".
	// This call is asynchronous, with the results returned in GlobalStatsReceived_t.
	// nHistoryDays specifies how many days of day-by-day history to retrieve in addition
	// to the overall totals. The limit is 60.
	virtual SteamAPICall_t RequestGlobalStats(int nHistoryDays) = 0;

	// Gets the lifetime totals for an aggregated stat
	virtual bool GetGlobalStat(const char* pchStatName, int64_t* pData) = 0;

	virtual bool GetGlobalStat(const char* pchStatName, double* pData) = 0;

	// Gets history for an aggregated stat. pData will be filled with daily values, starting with today.
	// So when called, pData[0] will be today, pData[1] will be yesterday, and pData[2] will be two days ago, 
	// etc. cubData is the size in bytes of the pubData buffer. Returns the number of 
	// elements actually set.

	virtual int GetGlobalStatHistory(const char* pchStatName, int64_t* pData, uint32_t cubData) = 0;

	virtual int GetGlobalStatHistory(const char* pchStatName, double* pData, uint32_t cubData) = 0;

	// For achievements that have related Progress stats, use this to query what the bounds of that progress are.
	// You may want this info to selectively call IndicateAchievementProgress when appropriate milestones of progress
	// have been made, to show a progress notification to the user.
	virtual bool GetAchievementProgressLimits(const char* pchName, int* pnMinProgress, int* pnMaxProgress) = 0;

	virtual bool GetAchievementProgressLimits(const char* pchName, float* pfMinProgress, float* pfMaxProgress) = 0;
};


class ISteamClient
{
public:
	ISteamUser* GetISteamUser(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion)
	{
		typedef ISteamUser* (__stdcall * func)(HSteamUser, HSteamPipe, const char*);
		return CallVFunction<func>(this, 5)(hSteamUser, hSteamPipe, pchVersion);
	}

	ISteamFriends* GetISteamFriends(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion)
	{
		typedef ISteamFriends* (__stdcall * func)(HSteamUser, HSteamPipe, const char*);
		return CallVFunction<func>(this, 8)(hSteamUser, hSteamPipe, pchVersion);
	}

	ISteamUtils* GetISteamUtils(HSteamPipe hSteamPipe, const char* pchVersion)
	{
		typedef ISteamUtils* (__stdcall* func)(HSteamPipe, const char*);
		return CallVFunction<func>(this, 9)(hSteamPipe, pchVersion);
	}

	ISteamGameCoordinator* GetISteamGenericInterface(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion)
	{
		typedef ISteamGameCoordinator* (__stdcall * func)(HSteamUser, HSteamPipe, const char*);
		return CallVFunction<func>(this, 12)(hSteamUser, hSteamPipe, pchVersion);
	}

	ISteamUserStats* GetISteamUserStats(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion)
	{
		typedef ISteamUserStats*(__stdcall* func)(HSteamUser, HSteamPipe, const char*);
		return CallVFunction<func>(this, 13)(hSteamUser, hSteamPipe, pchVersion);
	}

	ISteamHTTP* GetISteamHTTP(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion)
	{
		typedef ISteamHTTP* (__stdcall * func)(HSteamUser, HSteamPipe, const char*);
		return CallVFunction<func>(this, 23)(hSteamUser, hSteamPipe, pchVersion);
	}
};