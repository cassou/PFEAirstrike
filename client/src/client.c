//gcc enet1.c -o enet1 -lenet
#include <stdlib.h>
#include <stdio.h>
#include <enet/enet.h>
#include <string.h>
#include <time.h>
#include <SDL/SDL.h>
#include "messages.h"
#include "keys.h"


void set_rand_key(int key);
void set_key(int key);
void process_key(int key_status, SDLKey key);

char ip_addr[16]="127.0.0.1\0";
int port = 1234;

ENetHost *client;
ENetAddress address;
ENetEvent event;
ENetPeer *peer;

int next_time;
int keep_running = 42;
int bot_mod = 0;

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

void network_loop(){
	int serviceResult;
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
				printf("Server disconected.\n");
				exit(EXIT_FAILURE);

				break;
			case ENET_EVENT_TYPE_NONE:
				break;
			}
		} else if (serviceResult < 0) {
			puts("Error with servicing the client");
			exit(EXIT_FAILURE);
		}
	}
}

void sdl_loop(){
	SDL_Event event;
	SDL_WaitEvent(&event);
	switch(event.type){
	case SDL_QUIT:
		keep_running = 0;
		break;
	case SDL_KEYUP:
		process_key(-1,event.key.keysym.sym);
		break;
	case SDL_KEYDOWN:
		process_key(1,event.key.keysym.sym);
		break;
	}
}

void process_key(int key_status, SDLKey key){
	switch (key)
	{
	case SDLK_ESCAPE:
		keep_running = 0;
		break;
	case SDLK_RIGHT:
		set_key(key_status*KEY__DOWN);
		break;
	case SDLK_LEFT:
		set_key(key_status*KEY__UP);
		break;
	case SDLK_UP:
		set_key(key_status*KEY__ACCELERATE);
		break;
	case SDLK_x:
		set_key(key_status*KEY_FIRE);
		break;
	case SDLK_SPACE:
		set_key(key_status*KEY_BOMB);
		break;
	default:
		break;
	}
}

void graphic_init(){
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_SetVideoMode(400, 300, 32, SDL_HWSURFACE);
	SDL_WM_SetCaption("AirStrike Client", NULL);
}

void network_init(){
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		exit( EXIT_FAILURE);
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
	enet_address_set_host(&address, ip_addr);
	address.port = port;



	/* Wait up to 5 seconds for the connection attempt to succeed. */
	int trying_left = 5;
	int connect_succeeded=0;
	while (!connect_succeeded){
		printf("Trying to connect to %s:%d...\n",ip_addr,port);
		/* Initiate the connection, allocating the two channels 0 and 1. */
		peer = enet_host_connect(client, &address, 2, 0);

		if (peer == NULL) {
			fprintf(stderr, "No available peers for initiating an ENet connection.\n");
			exit(EXIT_FAILURE);
		}

		if (enet_host_service(client, &event, 3000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
			printf("Connection to %s:%d succeeded.\n",ip_addr,port);
			connect_succeeded=1;
		} else {
			/* Either the 5 seconds are up or a disconnect event was */
			/* received. Reset the peer in the event the 5 seconds   */
			/* had run out without any significant event.            */
			enet_peer_reset(peer);
			//printf("Connection to %s:%d failed.\n",ip_addr,port);
			trying_left--;
			if(trying_left<=0){
				printf("Could not connect to %s:%d\n",ip_addr,port);
				exit(EXIT_FAILURE);
			}
		}
	}
}

int main(int argc, char **argv){
	//TODO : handle errors in command line interpretation
	int nb = 1;
	while(nb < argc){
		if (!strcmp(argv[nb],"-ip") && nb+1<argc){
			strncpy(ip_addr,argv[nb+1],15);
			printf("Server address set to %s\n", ip_addr);
			ip_addr[15]='\n';
			nb++;
		}
		if (!strcmp(argv[nb],"-p") && nb+1<argc){
			sscanf(argv[nb+1],"%d", &port);
			printf("Server port set to %d\n", port);
			nb++;
		}
		if (!strcmp(argv[nb],"-b") ){
			bot_mod=1;
			printf("Will run as a bot !\n");
		}
		nb++;
	}



	srand(42);
	if (!bot_mod)
		graphic_init();

	network_init();
	while (keep_running) {
		if (!bot_mod){
			sdl_loop();
		}else{
			update_state();
		}
		network_loop();
		usleep(20);/** To avoid 100% CPU **/
	}

	enet_host_destroy(client);
	enet_deinitialize();
	if (!bot_mod)
		SDL_Quit();
	return EXIT_SUCCESS;
}
