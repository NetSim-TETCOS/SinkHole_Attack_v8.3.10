/************************************************************************************
 * Copyright (C) 2014                                                               *
 * TETCOS, Bangalore. India                                                         *
 *                                                                                  *
 * Tetcos owns the intellectual property rights in the Product and its content.     *
 * The copying, redistribution, reselling or publication of any or all of the       *
 * Product or its content without express prior written consent of Tetcos is        *
 * prohibited. Ownership and / or any other right relating to the software and all  *
 * intellectual property rights therein shall remain at all times with Tetcos.      *
 *                                                                                  *
 * Author:    Shashi Kant Suman                                                     *
 *                                                                                  *
 * ---------------------------------------------------------------------------------*/
#ifndef _NETSIM_APPLICATION_H_
#define _NETSIM_APPLICATION_H_
#include "main.h"
#include "Stack.h"
#define roundoff(d) ((long)(d+0.5))
#define MAX_TTL 255
#define MAX_PORT 65535
unsigned int nApplicationCount;
typedef struct stru_Application_Info APP_INFO;
typedef struct stru_Application_DataInfo APP_DATA_INFO;
typedef struct stru_Application_VoiceInfo APP_VOICE_INFO;
typedef struct stru_Application_VideoInfo APP_VIDEO_INFO;
typedef struct stru_Application_HTTPInfo APP_HTTP_INFO;
typedef struct stru_Application_EMAILInfo APP_EMAIL_INFO;
typedef struct stru_Application_P2PInfo APP_P2P_INFO;
typedef struct stru_Application_CallInfo APP_CALL_INFO;
typedef	struct stru_Application_EmulationInfo APP_EMULATION_INFO;
/// Enumeration for application events
typedef enum
{
	event_APP_START=PROTOCOL_APPLICATION*100+1,
	event_APP_END,
	event_APP_RESTART,
}APP_EVENT;
/// Enumeration for application control packets
typedef enum
{
	packet_HTTP_REQUEST=PROTOCOL_APPLICATION*100+1,
}APP_CONTROL_PACKET;
/// Enumeration for application state.
typedef enum
{
	appState_Started,
	appState_Paused,
	appState_Terminated,
}APP_STATE;
#include "List.h"
/// Structure to store received information
struct stru_ReceivedInfo
{
	NETSIM_ID nPacketId;
	NETSIM_ID nSegmentId;
	_ele* ele;
};
#define RECEIVEDINFO_ALLOC() (struct stru_ReceivedInfo*)list_alloc(sizeof(struct stru_ReceivedInfo),offsetof(struct stru_ReceivedInfo,ele))
/// Structure to store application metrics
struct stru_Application_Metrics
{
	unsigned int nApplicationId;
	NETSIM_ID nSourceId;
	NETSIM_ID nDestinationId;
	unsigned int nPacketTransmitted;
	unsigned int nPacketReceived;
	unsigned int nDuplicatePacketReceived;
	double dPayloadTransmitted;
	double dPayloadReceived;
	double dThroughput;
	double dDelay;
	struct stru_ReceivedInfo* receivedInfo;
};
/// Structure to store socket information
struct stru_SocketInfo
{
	NETSIM_ID nSourceId;
	NETSIM_ID nDestinationId;
	NETSIM_ID nSourcePort;
	NETSIM_ID nDestPort;
	unsigned int nSocketId;
	_ele* ele;
};
#define SOCKETINFO_ALLOC() (struct stru_SocketInfo*)list_alloc(sizeof(struct stru_SocketInfo),offsetof(struct stru_SocketInfo,ele))
/// Structure to store application information
struct stru_Application_Info
{
	//config variable
	NETSIM_ID id;
	NETSIM_ID nConfigId;
	APPLICATION_TYPE nAppType;
	unsigned int nSourceCount;
	unsigned int nDestCount;
	NETSIM_ID* sourceList;
	NETSIM_ID* destList;
	double dStartTime;
	double dEndTime;
	double dGenerationRate;

	unsigned int nPacketId;
	unsigned int* sourcePort;
	unsigned int* destPort;
	APP_STATE** nAppState;
	struct stru_Application_Metrics*** appMetrics;
	void* appData;

	struct stru_SocketInfo* socketInfo;
};
APP_INFO** appInfo;
/// Structure for Data information such as packetsize and inter arrival time,this is applicable for custom,FTP,Database Traffic
struct stru_Application_DataInfo 
{
	DISTRIBUTION packetSizeDistribution;
	double dPacketSize;
	DISTRIBUTION IATDistribution;
	double dIAT;
};
/// Structure for voice information such as packetsize, inter arrival time and service type,this is applicable for voice traffic
struct stru_Application_VoiceInfo
{
	DISTRIBUTION packetSizeDistribution;
	double dPacketSize;
	DISTRIBUTION IATDistribution;
	double dIAT;
	SERVICE_TYPE nServiceType;
	SUPPRESSION_MODEL nSuppressionModel;
	/*for Deterministics*/
	int nSuccessRatio;
	/*For markov*/
	int nTSL;//Talk sprut length
	double dSAF;//Speech activity factor
};
/// structure to store the video application information
struct stru_Application_VideoInfo
{
	VIDEO_MODEL nVidModel;
	int fps;
	int ppf;
	double mu;
	double sigma;
	double const_a;
	double const_b;
	double eta;
	int Msource;
	int ql;
	double beta_I;
	double beta_P;
	double beta_B;
	double gamma_I;
	double gamma_P;
	double gamma_B;
	double eta_I;
	double eta_P;
	double eta_B;
	char* info;
};
///Structure to store the emulation application information
struct stru_Application_EmulationInfo
{
	NETSIM_ID nDestinationId;
	NETSIM_ID nSourceId;
	NETSIM_ID nSourcePort;
	NETSIM_ID nDestinationPort;
	NETSIM_IPAddress realSourceIP;
	NETSIM_IPAddress realDestIP;
	NETSIM_IPAddress simSourceIP;
	NETSIM_IPAddress simDestIP;
};
/// Structure to store the HTTP application information
struct stru_Application_HTTPInfo
{
	double pageIAT;
	DISTRIBUTION pageIATDistribution;
	unsigned int pageCount;
	double pageSize;
	DISTRIBUTION pageSizeDistribution;
};
/// Structure to store email application information
struct stru_Application_EMAILInfo
{
	struct stru_emailinfo
	{
		double dDuration;
		DISTRIBUTION durationDistribution;
		double dEmailSize;
		DISTRIBUTION sizeDistribution;
	}SEND,RECEIVE;
};

/// Structure for Seeder list of peer_to_peer traffic
typedef struct stru_Application_P2P_SeederList
{
	NetSim_PACKET* packet;
	unsigned int nSeederCount;
	NETSIM_ID* nDeviceIds;
}SEEDER_LIST;
/// Structure for peer list of peer_to_peer traffic
typedef struct stru_Application_p2P_PeerList
{
	NETSIM_ID nDeviceId;
	unsigned int yetToReceiveCount;
	unsigned int* flag;
}PEER_LIST;
/// Structure fot peer_to_peer application
struct stru_Application_P2PInfo
{
	double dFileSize;
	DISTRIBUTION fizeSizeDistribution;
	double dPiecesSize;
	
	unsigned int nPieceCount;	//Pieces count
	NetSim_PACKET** pieceList; //Pieces
	SEEDER_LIST** seederList; //For each pieces
	PEER_LIST** peerList;	//For each peers
};
/// Structure for Erlang_call application
struct stru_Application_CallInfo
{
	DISTRIBUTION callDurationDistribution;
	double dCallDuration;
	DISTRIBUTION callIATDistribution;
	double dCallIAT;
	int** nCallStatus;
	double **dCallEndTime;
	NetSim_PACKET*** nextPacket;
	APP_VOICE_INFO VoiceInfo;

	unsigned long long int** nEventId;
	unsigned long long int** nAppoutevent;
	int (*fn_BlockCall)(APP_INFO* appInfo,NETSIM_ID nSourceId,NETSIM_ID nDestinationId,double time);
};

//For Email.c
typedef struct stru_email_detail
{
	int type;
	APP_INFO* appInfo;
}DETAIL;

#include "NetSim_Graph.h"
//Statistics
bool nApplicationThroughputGraphFlag;
char* szApplicationThroughputGraphVal;
bool nApplicationThroughputGraphRealTime;
struct stru_GenericGraph** genericAppGraph;
bool* nAppGraphFlag;
double* dPayloadReceived;

NetSim_PACKET* fn_NetSim_Application_GeneratePacket(double ldArrivalTime,
													NETSIM_ID nSourceId,
													NETSIM_ID nDestinationId,
													unsigned int nPacketId,
													APPLICATION_TYPE nAppType,
													PACKET_PRIORITY nPacketPriority,
													QUALITY_OF_SERVICE nQOS,
													unsigned int sourcePort,
													unsigned int destPort);
int fn_NetSim_Application_GenerateNextPacket(APP_INFO* appInfo,NETSIM_ID nSource,NETSIM_ID nDestination,double time);

/* Distribution Function */
_declspec(dllexport) int fnDistribution(DISTRIBUTION nDistributionType, double *fDistOut,
		unsigned long *uSeed, unsigned long *uSeed1,double* args);

/* Random number generator */
_declspec(dllexport) int fnRandomNo(long lm, double *fRandNo, unsigned long *uSeed,unsigned long *uSeed1);

int fnGetSourceIndex(APP_INFO* appInfo,NETSIM_ID source);
int fnGetDestIndex(APP_INFO* appInfo,NETSIM_ID dest);

/* HTTP Application */
int fn_NetSim_Application_StartHTTPAPP(APP_INFO* appInfo,double time,NETSIM_ID nSourceId,NETSIM_ID nDestId);
int fn_NetSim_Application_ConfigureHTTPTraffic(APP_INFO* appInfo,void* xmlNetSimNode);
int fn_NetSim_Application_HTTP_ProcessRequest(APP_INFO* pstruappinfo,NetSim_PACKET* pstruPacket);

/* Video Application */
int fn_NetSim_Application_StartVideoAPP(APP_INFO* appInfo,double time,NETSIM_ID nSourceId,NETSIM_ID nDestId);
int fn_NetSim_Application_ConfigureVideoTraffic(APP_INFO* appInfo,void* xmlNetSimNode);
_declspec(dllexport) int fn_NetSim_TrafficGenerator_Video(APP_VIDEO_INFO* info,
														   double* fPacketSize,
														   double* ldArrival,
														   unsigned long* uSeed,
														   unsigned long* uSeed1,
														   unsigned long* uSeed2,
														   unsigned long* uSeed3);

/* Voice Application */
int fn_NetSim_Application_StartVoiceAPP(APP_INFO* appInfo,double time,NETSIM_ID nSourceId,NETSIM_ID nDestId);
int fn_NetSim_Application_ConfigureVoiceTraffic(APP_INFO* appInfo,void* xmlNetSimNode);
_declspec(dllexport) int fn_NetSim_TrafficGenerator_Voice(APP_VOICE_INFO* info,
														   double* fSize,
														   double* ldArrival,
														   unsigned long* uSeed,
														   unsigned long* uSeed1,
														   unsigned long* uSeed2,
														   unsigned long* uSeed3);

/* Peer To Peer Application */
int fn_NetSim_Application_P2P_GenerateFile(APP_INFO* appInfo);
int fn_NetSim_Application_P2P_InitSeederList(APP_INFO* appInfo);
int fn_NetSim_Application_P2P_InitPeers(APP_INFO* appInfo);
int fn_NetSim_Application_P2P_SendNextPiece(APP_INFO* appInfo,NETSIM_ID destination,double time);
int fn_NetSim_Application_StartP2PAPP(APP_INFO* appInfo,double time,NETSIM_ID nSourceId,NETSIM_ID nDestId);
int fn_NetSim_Application_ConfigureP2PTraffic(APP_INFO* appInfo,void* xmlNetSimNode);
int fn_NetSim_Application_P2P_MarkReceivedPacket(APP_INFO* pstruappinfo,NetSim_PACKET* pstruPacket);

/* Email Application */
int fn_NetSim_Application_StartEmailAPP(APP_INFO* appInfo,double time,NETSIM_ID nSourceId,NETSIM_ID nDestId);
int fn_NetSim_Application_ConfigureEmailTraffic(APP_INFO* appInfo,void* xmlNetSimNode);
APP_INFO* fn_NetSim_Application_Email_GenerateNextPacket(DETAIL* detail,NETSIM_ID nSourceId,NETSIM_ID nDestinationId,double time);

/* Custom, FTP, Database Application */
int fn_NetSim_Application_StartDataAPP(APP_INFO* appInfo,double time,NETSIM_ID nSourceId,NETSIM_ID nDestId);
int fn_NetSim_Application_ConfigureDataTraffic(APP_INFO* appInfo,void* xmlNetSimNode);
int fn_NetSim_Application_ConfigureDatabaseTraffic(APP_INFO* appInfo,void* xmlNetSimNode);
int fn_NetSim_Application_ConfigureFTPTraffic(APP_INFO* appInfo,void* xmlNetSimNode);
_declspec(dllexport) int fn_NetSim_TrafficGenerator_Custom(APP_DATA_INFO* info,
														   double* fSize,
														   double* ldArrival,
														   unsigned long* uSeed,
														   unsigned long* uSeed1,
														   unsigned long* uSeed2,
														   unsigned long* uSeed3);

/* Erlang Call */
int fn_NetSim_Application_ErlangCall_StartCall(NetSim_EVENTDETAILS* pstruEventDetails);
int fn_NetSim_Application_ErlangCall_EndCall(NetSim_EVENTDETAILS* pstruEventDetails);
int fn_NetSim_Application_StartErlangCallAPP(APP_INFO* appInfo,double time,NETSIM_ID nSourceId,NETSIM_ID nDestId);
int fn_NetSim_Application_ConfigureErlangCallTraffic(APP_INFO* appInfo,void* xmlNetSimNode);

/* Emulation */
int fn_NetSim_Application_ConfigureEmulationTraffic(APP_INFO* appInfo,void* xmlNetSimNode);


/* Application API's */
_declspec(dllexport) int fn_NetSim_Application_Init_F(struct stru_NetSim_Network *NETWORK_Formal,
													NetSim_EVENTDETAILS *pstruEventDetails_Formal,
													char *pszAppPath_Formal,
													char *pszWritePath_Formal,
													int nVersion_Type,
													void **fnPointer);
_declspec(dllexport) int fn_NetSim_Application_Configure_F(void** var);
_declspec(dllexport) int fn_NetSim_Application_Metrics_F(char* szMetrics);
int fn_NetSim_Application_Metrics_Source(APP_INFO* appInfo,NetSim_PACKET* pstruPacket);
int fn_NetSim_Application_Metrics_Destination(APP_INFO* appInfo,NetSim_PACKET* pstruPacket);
int fn_NetSim_App_RestartApplication();
_declspec(dllexport) int fn_NetSim_Application_PlotGraph(NetSim_PACKET* pstruPacket);


//Application Interface function
int fnCreateSocketBuffer(APP_INFO* appInfo,NETSIM_ID nSourceId,NETSIM_ID nDestinationId,NETSIM_ID nSourcePort,NETSIM_ID nDestPort);
_declspec(dllexport) unsigned int fnGetSocketId(NETSIM_ID nAppId,NETSIM_ID nSourceId,NETSIM_ID nDestinationId,NETSIM_ID nSourcePort,NETSIM_ID nDestPort); //Function present in NetworkStack.dll.

int fn_NetSim_Add_DummyPayload(NetSim_PACKET* packet);
#endif