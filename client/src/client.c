//gcc enet1.c -o enet1 -lenet
#include <stdlib.h>
#include <stdio.h>
#include <enet/enet.h>
#include <string.h>
#include <time.h>
#include "messages.h"
#include "keys.h"

void set_rand_key(int key);
void set_key(int key);

ENetHost *client;
ENetAddress address;
ENetEvent event;
ENetPeer *peer;

int next_time;


void update_state(){
	int current_time = time(NULL);
	if (current_time>next_time){
		set_key(KEY__ACCELERATE);
		next_time = current_time+(rand()%1)+1;
		set_rand_key(KEY_FIRE);
		//printf("%d",current_time);
	}
}

void set_rand_key(int key){

	AS_message_t msg;
	msg.source_id = 0;
	msg.mess_type=MSG_KEY;

	if (rand()>(RAND_MAX/2)){
		msg.data=key;
	}else{
		msg.data=-key;
	}
	ENetPacket *packet = enet_packet_create(&msg, sizeof(AS_message_t), ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void set_key(int key){

	AS_message_t msg;
	msg.source_id = 0;
	msg.mess_type=MSG_KEY;
	msg.data=key;
	ENetPacket *packet = enet_packet_create(&msg, sizeof(AS_message_t), ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}



int main(int argc, char **argv)
{

	srand(42);

	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}

	client = enet_host_create(NULL /* create a client host */ ,
			1 /* only allow 1 outgoing connection */ ,
			2 /* allow up 2 channels to be used, 0 and 1 */ ,
			57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */ ,
			14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */ );

	if (client == NULL) {
		fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}

	/* Connect to some.server.net:1234. */
	enet_address_set_host(&address, "127.0.0.1");
	address.port = 1234;

	/* Initiate the connection, allocating the two channels 0 and 1. */
	peer = enet_host_connect(client, &address, 2, 0);

	if (peer == NULL) {
		fprintf(stderr, "No available peers for initiating an ENet connection.\n");
		exit(EXIT_FAILURE);
	}

	/* Wait up to 5 seconds for the connection attempt to succeed. */
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		puts("Connection to some.server.net:1234 succeeded.");

	} else {
		/* Either the 5 seconds are up or a disconnect event was */
		/* received. Reset the peer in the event the 5 seconds   */
		/* had run out without any significant event.            */
		enet_peer_reset(peer);

		puts("Connection to some.server.net:1234 failed.");
		exit(EXIT_FAILURE);
	}

	int serviceResult;
	while (42) {
		/* Keep doing host_service until no events are left */
		while ( (serviceResult=enet_host_service(client, &event, 0))!=0) {
			if (serviceResult > 0) {
				switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT:
					printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
					event.peer->data = (void *)"New User";
					break;

				case ENET_EVENT_TYPE_RECEIVE:
					printf("A packet of length %u containing '%s' was received from %s on channel %u.\n",
							event.packet->dataLength, event.packet->data, (char*)event.peer->data, event.channelID);
					/* Clean up the packet now that we're done using it. */
					enet_packet_destroy(event.packet);
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					printf("%s disconected.\n", (char*)event.peer->data);

					break;
				case ENET_EVENT_TYPE_NONE:
					break;
				}
			} else if (serviceResult < 0) {
				puts("Error with servicing the client");
				exit(EXIT_FAILURE);
			}

		}


		update_state();
		usleep(20);

	}

	enet_host_destroy(client);
	enet_deinitialize();
}
