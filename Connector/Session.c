#include <stdio.h>
#include <stdlib.h>

#include "Session.h"

void setSession(Session* Ses, struct sockaddr* sock_addr, int fd, time_t addTime, int isLoggedIn){
	Ses->sock_addr= sock_addr;
	Ses->fd= fd;
	Ses->addTime= addTime;
	Ses->isLoggedIn= isLoggedIn;
}
void setNextsession(Session* Ses, Session* NextSession){
	Ses->NextSession= NextSession;
}

Session* createSession(void){
	Session* temp= (Session*) malloc(sizeof(Session));

	temp->NextSession= NULL;

	temp->setSession= setSession;

	return temp;
}

SsList* CreateSsList(void){
	SsList* temp= (SsList*) malloc(sizeof(SsList));
	temp->SsHead= NULL;
	temp->Append= Append;
	temp->Delete= Delete;
	temp->getTimeOut= getTimeOut;

	return temp;
}

int Append(SsList* SList, struct sockaddr* sock_addr, int fd, time_t addTime, int isLoggedIn){

	Session* SsTail;

	if(SList->SsHead== NULL){
		SList->SsHead= createSession();
		SList->SsHead->setSession(SList->SsHead, sock_addr, fd, addTime, isLoggedIn);
		return 0;
	}

	SsTail= SList->SsHead;

	while(SsTail->NextSession!= NULL){
		SsTail= SsTail->NextSession;
		printf("test: %d", SsTail->fd);
	}

	SsTail->NextSession= createSession();
	SsTail->NextSession->setSession(SsTail->NextSession, sock_addr, fd, addTime, isLoggedIn);
	return 0;
}

int Delete(SsList* SList, int fd){
	Session* SsTail;
	Session* preTail;

	if(SList->SsHead== NULL){
		printf("List is empty");
		return 1;
	}
	SsTail= SList->SsHead;
	if(SsTail->NextSession== NULL){
		if(SsTail->fd== fd){
			free(SsTail);
			SsTail= NULL;
			return 0;
		}
		else{
			return 1;
		}
	}
	if(SsTail->fd== fd){
		preTail= SsTail;
		SsTail= SsTail->NextSession;
		free(preTail);
		SList->SsHead= SsTail;
		return 0;
	}
	do{
		preTail= SsTail;
		SsTail= SsTail->NextSession;
		if(SsTail->fd== fd){
			preTail->NextSession= SsTail->NextSession;
			free(SsTail);
			return 0;
		}
	}while(SsTail->NextSession!= NULL);

	return 1;
}

int getTimeOut(SsList* SList, int DurTime){
	Session* SsTail;
	time_t CurTime= time(NULL);

	if(SList== NULL){
		printf("SList is invalid");
		return -1;
	}
	if(SList->SsHead== NULL){
		printf("SsHead is NULL");
		return -1;
	}

	SsTail= SList->SsHead;
	if(CurTime- SsTail->addTime> DurTime){
		return SsTail->fd;
	}
	do{
		SsTail= SsTail->NextSession;

		if(CurTime- SsTail->addTime> DurTime){
			return SsTail->fd;
		}
	}while(SsTail->NextSession!= NULL);

	return -1;
}


