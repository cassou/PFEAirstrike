#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <enet/enet.h>
#include "network.h"
#include "messages.h"
#include "prototype.h"
#include "players.h"
#include "keys.h"
#include "sprite.h"
#include <sys/time.h>
#include <sys/resource.h>



void *thread_function( void *arg );
void network_loop();
void process_packet(ENetEvent * event);
void sendMessage(int peerId, int msgType,int clientId,int data);
void sendMessageLoad(int peerId, int sizeInByte);

int  clientConnected[MAXPLAYERS];
int  clientPeerId[MAXPLAYERS];
int  clientCount= 0;

unsigned int counterOut = 0;
unsigned int counterIn = 0;

unsigned int lastCounterOut = 0;
unsigned int lastCounterIn = 0;

volatile int netStop=0 ;//place to 1 to stop the network
volatile int netStopped=0 ;//place to 1 when network stopped
int fps=0;

int networkLoad = 0;
int networkLoadinterval = 0;
void * loadData;

void network_init(){
	pthread_t thread;
	int rc;
	rc = pthread_create( &thread, NULL, thread_function, NULL);
	if(rc)
		error(EXIT_FAILURE, rc, "pthread_create");
}

void *thread_function( void *arg ){

	int i;
	for (i=0; i<MAXPLAYERS;i++){
		clientConnected[i]=0;
		clientPeerId[i]=-1;
	}

	char cb[250];
	time_t h = 0;
	h = time(NULL);
	sprintf(cb,"airlog_%dB_%d_ms_%d.log",networkLoad,networkLoadinterval,h);
	logOpen(cb);
	mylog(LOG_INFO,"Starting the game",0);
	sprintf(cb,"%d players; %d teams; load %d Bytes each %dms",playerCount,teamCount,networkLoad,networkLoadinterval);
	mylog(LOG_INFO,cb,0);
	network_loop();
	mylog(LOG_INFO,"Closing the game",0);
	logClose();
	netStopped=1;
	//pthread_exit(NULL);
}



ENetAddress address;
ENetHost *server;

void network_loop(){

	int rc;
	rc = enet_initialize();
	if(rc)
		error(EXIT_FAILURE, rc, "An error occurred while initializing ENet.\n");

	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */

	address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = 1234;


	mylog(LOG_NETWORK,"Listening on port",address.port);

	server = enet_host_create(&address /* the address to bind the server host to */ ,
			MAXPLAYERS /* allow up to 32 clients and/or outgoing connections */ ,
			2 /* allow up to 2 channels to be used, 0 and 1 */ ,
			0 /* assume any amount of incoming bandwidth */ ,
			0 /* assume any amount of outgoing bandwidth */ );
	if (server == NULL) {
		mylog(LOG_ERROR,"An error occurred while trying to create an ENet server host.",0);
		exit(EXIT_FAILURE);
	}

	ENetEvent event;

	int prevTimeStat = sprite_global.game_clock;
	int prevTimeLoad = prevTimeStat;
	int prevTimePlay = prevTimeStat;

	int pingMin;
	int pingMax;
	int pingAverage;
	int pingCnt ;
	int prev_utime=0;
	int cur_utime=0;
	int prevClock=0;
	int currClock=0;
	struct timeval tim;
	struct rusage usage;
	int tdiff;

	loadData = malloc(networkLoad);
	if (!loadData){
		perror("Issue in malloc of network load.");
	}

	while (netStop!=1) {
		int k;
		if (sprite_global.game_clock-prevTimeStat>=1000){
			mylog(LOG_NETWORK_TOTAL_OUT,"Total Output", counterOut);
			mylog(LOG_NETWORK_TOTAL_IN,"Total Input", counterIn);
			mylog(LOG_NETWORK_OUT,"Output", counterOut-lastCounterOut);
			mylog(LOG_NETWORK_IN,"Input", counterIn-lastCounterIn);
			mylog(LOG_FPS,"fps", fps);
			mylog(LOG_CLIENT_COUNT,"Clients", clientCount);

			lastCounterOut=counterOut;
			lastCounterIn=counterIn;

			pingMin=99999;
			pingMax=0;
			pingAverage=0;
			pingCnt = 0;
			for(k=0;k<playerCount;k++){
				if (clientConnected[k]){
					ENetPeer *p = &server->peers[k];
					if (!(p==NULL)){
						pingCnt++;
						pingAverage+=p->roundTripTime;
						pingMin=MIN(p->roundTripTime,pingMin);
						pingMax=MAX(p->roundTripTime,pingMax);
					}
				}
			}
			if (pingCnt!=0){
				pingAverage = div(pingAverage,pingCnt).quot;
				mylog(LOG_PING_AV,"Ping average",pingAverage);
				mylog(LOG_PING_MIN,"Ping min",pingMin);
				mylog(LOG_PING_MAX,"Ping max",pingMax);
			}

			if (!getrusage(RUSAGE_SELF, &usage) && !gettimeofday(&tim, NULL)){
				prev_utime=cur_utime;
				cur_utime=clock();
				prevClock=currClock;
				currClock=tim.tv_sec*1000000+tim.tv_usec;
				if ((tdiff = currClock-prevClock)!=0)
					mylog(LOG_CPU,"CPU %",(cur_utime-prev_utime)/(tdiff/100));
			}

			prevTimeStat = sprite_global.game_clock;
		}

		int u;
		if (sprite_global.game_clock-prevTimeLoad>=networkLoadinterval){

			for(u=0;u<playerCount;u++){
				if (clientConnected[u]){
					//printf("############### %d\n",u);
					printf("");
					sendMessageLoad(clientPeerId[u],networkLoad);
				}
			}
			prevTimeLoad = sprite_global.game_clock;
		}

		if (sprite_global.game_clock-prevTimePlay>=100){
			for(k=0;k<playerCount;k++){
				if (clientConnected[k]){
					sendMessage(clientPeerId[k],MSG_POINTS,k,players[k].points);
					sendMessage(clientPeerId[k],MSG_DAMAGE,k,players[k].damage);
					if (players[k].spawnTimer){
						sendMessage(clientPeerId[k],MSG_TIME2START,k,1+(players[k].spawnTimer-sprite_global.game_clock)/1000);
						//printf("%d \n",1+(players[k].spawnTimer-sprite_global.game_clock)/1000);
					}
				}
			}
			prevTimePlay=sprite_global.game_clock;
		}



		int serviceResult = 1;
		/* Keep doing host_service until no events are left */
		while (serviceResult > 0) {
			/* Wait up to 1000 milliseconds for an event. */
			serviceResult = enet_host_service(server, &event, 10);

			if (serviceResult > 0) {
				switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT:
					//printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
					mylog(LOG_NETWORK_CONNECT,"A new client connected from ",event.peer->address.host);
					/* Store any relevant client information here. */
					//event.peer->data = (void *)"Client information";
					break;

				case ENET_EVENT_TYPE_RECEIVE:
					process_packet(&event);
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					printf("%s disconected.\n", (char*)event.peer->data);
					int i;
					for (i=0;i<playerCount;i++){
						if (clientPeerId[i]==event.peer->incomingPeerID){
							clientConnected[i]=0;
							clientPeerId[i]=-1;
							strncpy(players[i].name," \0",32);
							players[i].isConnected=0;
							clientCount-=1;
							mylog(LOG_NETWORK_DISCONNECT,"A client disconnected ",i);
						}
					}

					/* Reset the peer's client information. */
					event.peer->data = NULL;
					break;
				case ENET_EVENT_TYPE_NONE:
					break;
				}
			} else if (serviceResult > 0) {
				puts("Error with servicing the server");
				exit(EXIT_FAILURE);
			}
		}
	}
	free(loadData);
}


void sendMessage(int peerId, int msgType,int clientId,int data){
	ENetPeer *p = &server->peers[peerId];
	if (!(p==NULL)){
		AS_message_t msg;
		msg.mess_type=msgType;
		msg.client_id = clientId;
		msg.data = data;
		msg.name[0] = '\0';
		ENetPacket *packet = enet_packet_create(&msg, sizeof(AS_message_t), ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(p, 0, packet);

		counterOut+=sizeof(AS_message_t);

	}
}


void sendMessageLoad(int peerId, int sizeInByte){
	ENetPeer *p = &server->peers[peerId];

	if (!(p==NULL)){
		if(loadData){
			ENetPacket *packet = enet_packet_create(NULL, sizeInByte, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(p, 1, packet);
			counterOut+=sizeInByte;
		}
	}
}


void process_packet(ENetEvent * event){
	AS_message_t * msg = (AS_message_t * )(event->packet->data);
	int peerID = event->peer->incomingPeerID;

	counterIn+=sizeof(AS_message_t);

	switch (msg->mess_type) {
	case MSG_HELLO:
		mylog(LOG_MESSAGE,"MSG_HELLO message received from",peerID);
		//if(!inGame || 1){
		int client_id;
		int success=0;
		int teamRequested = msg->data;
		if (msg->client_id>=0 && msg->client_id<playerCount && !clientConnected[msg->client_id] && players[msg->client_id].team->id == teamRequested){
			client_id = msg->client_id;
			clientCount++;
			clientConnected[client_id]=1;
			players[client_id].isConnected=1;
			clientPeerId[client_id]=peerID;
			strncpy(players[client_id].name,msg->name,32);
			printf("*******************************%s   %s\n",msg->name,players[client_id].name);
			players[client_id].name[31]='\0';
			sendMessage(peerID,MSG_HELLO,client_id,client_id);
			sendMessage(peerID,MSG_TEAM_ID,client_id,players[client_id].team->id);
			sendMessage(peerID,MSG_ID_IN_TEAM,client_id,players[client_id].id_in_team);
			mylog(LOG_MESSAGE,"MSG_HELLO sent to",peerID);
			success=1;
		}else{
			int i;
			for (i=0;i<playerCount;i++){
				if (!clientConnected[i] && players[i].team->id==teamRequested){
					client_id = i;
					clientCount++;
					clientConnected[client_id]=1;
					players[client_id].isConnected=1;
					clientPeerId[client_id]=peerID;
					strncpy(players[client_id].name,msg->name,32);
					printf("*******************************%s   %s\n",msg->name,players[client_id].name);
					players[client_id].name[31]='\0';
					sendMessage(peerID,MSG_HELLO,client_id,client_id);
					sendMessage(peerID,MSG_TEAM_ID,client_id,players[client_id].team->id);
					sendMessage(peerID,MSG_ID_IN_TEAM,client_id,players[client_id].id_in_team);
					mylog(LOG_MESSAGE,"MSG_HELLO sent to",peerID);
					success=1;
					break;

				}
			}
		}

		if(!success){
			sendMessage(peerID,MSG_NO_SPACE,0,0);
			mylog(LOG_MESSAGE,"MSG_NO_SPACE sent to",peerID);
			break;
		}

		//}else{
		/*int client_id=msg->client_id;
			if (client_id>=0 && client_id<playerCount && !clientConnected[client_id]){
				clientCount++;
				clientConnected[client_id]=1;
				players[client_id].isConnected=1;
				clientPeerId[client_id]=peerID;
				strncpy(players[client_id].name,msg->name,32);
				printf("*******************************%s   %s\n",msg->name,players[client_id].name);
				players[client_id].name[31]='\0';
				sendMessage(peerID,MSG_HELLO,client_id,client_id);
				sendMessage(peerID,MSG_TEAM_ID,client_id,players[client_id].team->id);
				sendMessage(peerID,MSG_ID_IN_TEAM,client_id,players[client_id].id_in_team);
				//sendMessage(peerID,MSG_ID_IN_TEAM,client_id,0);
				mylog(LOG_MESSAGE,"MSG_HELLO sent to",peerID);
			}else{
				sendMessage(peerID,MSG_NO_SPACE,0,0);
				mylog(LOG_MESSAGE,"MSG_NO_SPACE sent to",peerID);
				break;
			}*/

		//}

		break;
	case MSG_KEY:
		//printf("Key %d message received from %d\n",msg->data,peerID);
		if (clientConnected[msg->client_id]){
			if (msg->data>=0){
				network_keymap[msg->client_id][msg->data]=1;
			}else{
				network_keymap[msg->client_id][-msg->data]=0;
			}
		}
		break;
	default:
		break;
	}
	enet_packet_destroy(event->packet);
}





