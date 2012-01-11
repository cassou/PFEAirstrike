#include "prototype.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "sprite.h"


int playerCount;
int teamCount;

FILE *fpLog;

void logOpen(){
	fpLog=fopen("airstrike.log","a");
	if(fpLog==NULL){
		perror("Failed to open log file.");
	}
}

void logClose(){
	if (!fpLog){
		printf("Trying to close a null file.");
		return;
	}

	int err=fclose(fpLog);
	if(err!=0){
		perror("Failed to close log file.");
	}
}

void mylog(int type, char * message, int value){

	time_t h = 0;
	h = time(NULL);

	char s[512];
	sprintf(&s,"%d	%s	%d	%d %s",type,message,value,sprite_global.game_clock, ctime(&h));
	printf(&s);
	fprintf(fpLog,&s);
}


/*void prototype_setup(){
	playerCount = 600;
}

void testing123(){
	printf("Testing123\n");
}
 */
