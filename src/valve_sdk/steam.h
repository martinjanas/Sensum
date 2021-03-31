#pragma once

typedef int32_t HSteamPipe;
typedef int32_t HSteamUser;

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
	uint32_t GetAccountID() const { return m_steamid.m_comp.m_unAccountID; }

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

//class ISteamFriends;

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

	ISteamHTTP* GetISteamHTTP(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion)
	{
		typedef ISteamHTTP* (__stdcall * func)(HSteamUser, HSteamPipe, const char*);
		return CallVFunction<func>(this, 23)(hSteamUser, hSteamPipe, pchVersion);
	}
};