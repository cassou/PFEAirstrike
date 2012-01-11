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

void *thread_function( void *arg );
void network_loop();
void process_packet(ENetEvent * event);
void sendMessage(int peerId, int msgType,int clientId,int data);

int  clientConnected[MAXPLAYERS];
int  clientPeerId[MAXPLAYERS];
int  clientCount= 0;

unsigned int counterOut = 0;
unsigned int counterIn = 0;

volatile int netStop=0 ;//place to 1 to stop the network
volatile int netStopped=0 ;//place to 1 when network stopped


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
	logOpen();
	mylog(LOG_INFO,"Starting the game",0);
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
		//fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
		mylog(LOG_ERROR,"An error occurred while trying to create an ENet server host.",0);
		exit(EXIT_FAILURE);
	}

	ENetEvent event;

	int prevTime = sprite_global.game_clock;

	int pingMin;
	int pingMax;
	int pingAverage;
	int pingCnt ;

	while (netStop!=1) {
		int k;
		if (sprite_global.game_clock-prevTime>=1000){
			mylog(LOG_NETWORK_OUT,"Output", counterOut);
			mylog(LOG_NETWORK_IN,"Input", counterIn);
			prevTime = sprite_global.game_clock;


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


		}



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




		int serviceResult = 1;
		/* Keep doing host_service until no events are left */
		while (serviceResult > 0) {
			/* Wait up to 1000 milliseconds for an event. */
			serviceResult = enet_host_service(server, &event, 10);

			if (serviceResult > 0) {
				switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT:
					//printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
					mylog(LOG_NETWORK,"A new client connected from ",event.peer->address.host);
					/* Store any relevant client information here. */
					//event.peer->data = (void *)"Client information";
					break;

				case ENET_EVENT_TYPE_RECEIVE:
					process_packet(&event);
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					printf("%s disconected.\n", (char*)event.peer->data);
					mylog(LOG_NETWORK,"A client disconnected from ",event.peer->address.host);
					/* Reset the peer's client information. */
					event.peer->data = NULL;

					break;
				}
			} else if (serviceResult > 0) {
				puts("Error with servicing the server");
				exit(EXIT_FAILURE);
			}
		}
	}
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

void process_packet(ENetEvent * event){
	AS_message_t * msg = (AS_message_t * )(event->packet->data);
	int peerID = event->peer->incomingPeerID;

	counterIn+=sizeof(AS_message_t);

	switch (msg->mess_type) {
	case MSG_HELLO:
		mylog(LOG_MESSAGE,"MSG_HELLO message received from",peerID);

		//assign an uid if player doesn't have one
		int client_id=msg->client_id;
		if (client_id<0 || client_id>=playerCount){
			if (clientCount<playerCount){
				int i;
				for (i=0;i<playerCount;i++){
					if (!clientConnected[i]){
						client_id = i;
						clientCount++;
						break;
					}
				}
			}else{
				sendMessage(peerID,MSG_NO_SPACE,0,0);
				mylog(LOG_MESSAGE,"MSG_NO_SPACE sended to",peerID);
				break;
			}
		}
		clientConnected[client_id]=1;
		clientPeerId[client_id]=peerID;
		sendMessage(peerID,MSG_HELLO,client_id,client_id);
		mylog(LOG_MESSAGE,"MSG_HELLO sended to",peerID);
		break;
	case MSG_KEY:
		printf("Key %d message received from %d\n",msg->data,peerID);
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





