#include "packet.h"


#include <openssl/rsa.h>
#include <openssl/rand.h>


void proc_0xFE(share* shared, craftIk_epoll* clnt_epoll, int clnt_num){
	char message[1];
	//recv(clnt_epoll->events[clnt_num].data.fd, message, (size_t)sizeof(message), 0);
	proc_0xFF(shared, clnt_epoll, clnt_num);
}

void proc_0xFF(share* shared, craftIk_epoll* clnt_epoll, int clnt_num){
#ifdef DEBUG

	shared->prop->protocol_version = 29;
	shared->prop->server_version[1]= '1';
	shared->prop->server_version[3]= '.';
	shared->prop->server_version[5]= '2';
	shared->prop->server_version[7]= '.';
	shared->prop->server_version[9]= '5';
	shared->prop->motd[1]='D';
	shared->prop->motd[3]='E';
	shared->prop->motd[5]='B';
	shared->prop->motd[7]='U';
	shared->prop->motd[9]='G';
	shared->player_count= 25;
	shared->prop->max_players= 255;
#endif
	const char* motd= shared->prop->motd;
	int motd_length= ucs_str_length(motd, 1024);
	const char* serv_ver= shared->prop->server_version;
	int ver_length= ucs_str_length(serv_ver, 64);
	int proto_ver= shared->prop->protocol_version;
	int proto_ver_len= 0;
	int cur_player= shared-> player_count;
	int cur_player_len= 0 ;
	int max_player= shared->prop->max_players;
	int max_player_len= 0;

	int temp;

	char message[1024];

	short cur_length=0;
	memset(message, 0, sizeof(message));
	
	temp= proto_ver;
	while(temp>0|| proto_ver_len== 0){
		proto_ver_len++;
		temp/= 10;
	}
	temp= cur_player;
	while(temp>0|| cur_player_len== 0){
		cur_player_len++;
		temp/= 10;
	}
	temp= max_player;
	while(temp>0|| max_player_len== 0){
		max_player_len++;
		temp/= 10;
	}

	message[0]= 0xff;
	cur_length+= 1;// length of 0xff
	cur_length+= 2;// length of length of string
	message[4]= 0xa7;
	message[6]= 0x31;
	cur_length+= 4; // length of §1
	cur_length+= 2; // length of server_version delimeter
	for(int i=0; i< proto_ver_len; i++){
		message[cur_length+2*(proto_ver_len-i)-1]
			=proto_ver% 10+ '0';
		proto_ver/= 10;
	}

	cur_length+= proto_ver_len* 2;// length of protocol version
	
	cur_length+= 2;// length of server version delilmeter

	for(int i=0; i<ver_length; i++){
		message[cur_length+i]= serv_ver[i];
	}
	cur_length+= ver_length;// length of server version

	cur_length+= 2;//length of motd delimeter

	for(int i=0; i<motd_length; i++){
		message[cur_length+i]= motd[i];
	}
	cur_length+= motd_length;// length of motd

	cur_length+= 2;//length of cur player count delimeter


	for(int i=0; i< cur_player_len; i++){
		message[cur_length+2*(cur_player_len-i)-1]
			=cur_player% 10+ '0';
		cur_player/= 10;
	}

	cur_length+= cur_player_len* 2;// length of cur player count
	cur_length+=2; //length of max player delimeter
	for(int i=0; i< max_player_len; i++){
		message[cur_length+2*(max_player_len-i)-1]
			=max_player% 10+ '0';
		max_player/= 10;
	}

	cur_length+= max_player_len* 2;// length of max player
	cur_length-=3;
	cur_length/=2;
	message[2]= cur_length%256;
	message[1]= cur_length/256;
	cur_length*=2;

	send(clnt_epoll->events[clnt_num].data.fd, message, cur_length+3, 0);
}

void proc_0x02(share* shared, craftIk_epoll* clnt_epoll, int clnt_num)
{
	short len;
	recv(clnt_epoll->events[clnt_num].data.fd, &len, sizeof(short), 0);
	len = ntohs(len);

	short* data = (short*)malloc(sizeof(short)*len);
	char* string = (char*)malloc(sizeof(char)*len+1);
	recv(clnt_epoll->events[clnt_num].data.fd, data, sizeof(short)*len, 0);
	memset(string, 0x00, len+1);
	for(int i=0; i<len; i++){
		string[i] = ntohs(data[i]);
	}
	free(data);

	char username[50];
	memset(username, 0x00, 50);
#ifdef DEBUG
	fprintf(stderr,"[DEBUG] handshake : %s\n", string);
#endif
	for(int i=0; string[i] != ';'; i++){
		username[i] = string[i];
	}
	free(string);

	craftIk_session* thisession = craftIk_session_get( clnt_epoll->events[clnt_num].data.fd );
	strcpy(thisession->username, username);

#ifdef DEBUG
	fprintf(stderr, "[DEBUG] username added to session : %s\n", thisession->username);
#endif

	// TODO : send 0x02 hash '-'
	char packet[10];
	packet[0] = 0x02;
	packet[1] = 0x00;
	packet[2] = 0x01;
	packet[3] = 0x00;
	packet[4] = '-';
	send(clnt_epoll->events[clnt_num].data.fd, packet, 5, 0);
}

void proc_0x01(share* shared, craftIk_epoll* clnt_epoll, int clnt_num)
{
	int pversion;
	recv(clnt_epoll->events[clnt_num].data.fd, &pversion, sizeof(int), 0);
	pversion = ntohl(pversion);
#ifdef DEBUG
	fprintf(stderr, "[DEBUG] protocol version %d\n", pversion);
#endif

	short len;
	short* data;
	char* string;
	recv(clnt_epoll->events[clnt_num].data.fd, &len, sizeof(short), 0);
	len = ntohs(len);

	data = (short*)malloc(sizeof(short)*len);
	string = (char*)malloc(sizeof(char)*len + 1);
	memset(string, 0x00, len+1);
	recv(clnt_epoll->events[clnt_num].data.fd, data, sizeof(short)*len, 0);
	for(int i=0; i<len; i++){
		string[i] = (char)ntohs(data[i]);
	}
#ifdef DEBUG
	fprintf(stderr,"[DEBUG] 0x01 username : %s\n", string);
#endif
	free(data);
	free(string);

	len = 0;
	data = NULL;
	string = NULL;
	recv(clnt_epoll->events[clnt_num].data.fd, &len, sizeof(short), 0);
        len = ntohs(len);

	if(len > 0){
		data = (short*)malloc(sizeof(short)*len);
	       	string = (char*)malloc(sizeof(char)*len + 1);
	        memset(string, 0x00, len+1);
	        recv(clnt_epoll->events[clnt_num].data.fd, data, sizeof(short)*len, 0);
	        for(int i=0; i<len; i++){
	                string[i] = (char)ntohs(data[i]);
	        }
#ifdef DEBUG
        	fprintf(stderr,"[DEBUG] 0x01 empty string  [%d,%s]\n",len, string);
#endif
		free(data);
		free(string);
	}

	void* flush = malloc(11);
	recv(clnt_epoll->events[clnt_num].data.fd, flush, 11, 0);
	free(flush);


	// send from here
	int plen = 0;
	craftIk_session* thisession = craftIk_session_get( clnt_epoll->events[clnt_num].data.fd );

	char packet[256];
	memset(packet, 0x00, 256);
	packet[0] = 0x01;
	plen++;

	int eid = thisession->EID;
	eid = htonl(eid);
	memcpy( &packet[plen], &eid, sizeof(int) );
	plen += sizeof(int);

	len = 0;
	string = NULL;
	data = NULL;
	memcpy( &packet[plen], &len, sizeof(short) );
	plen += sizeof(short);

	len = strlen("default");
	string = (char*)malloc(sizeof(char)*len+1);
	data = (short*)malloc(sizeof(short)*len);
	strcpy(string, "default");
	for(int i=0; i<len; i++){
		data[i] = htons((short)string[i]);
	}
	len = htons(len);
	memcpy( &packet[plen], &len, sizeof(short) );
	plen += sizeof(short);

	len = ntohs(len);
	memcpy( &packet[plen], data, sizeof(short)*len );
	plen += sizeof(short)*len;

	int idata = 0;
	idata = htonl(idata);
	memcpy( &packet[plen], &idata, sizeof(int) );
	plen += sizeof(int);

	idata = 0;
	idata = htonl(idata);
	memcpy( &packet[plen], &idata, sizeof(int) );
	plen += sizeof(int);

	char cdata = 0;
	memcpy( &packet[plen], &cdata, sizeof(char) );
	plen += sizeof(char);

	cdata = 0; // not used
	memcpy( &packet[plen], &cdata, sizeof(char) );
	plen += sizeof(char);
	
	cdata =shared->prop->max_players; // max player
	memcpy( &packet[plen], &cdata, sizeof(char) );
	plen += sizeof(char);

	send(clnt_epoll->events[clnt_num].data.fd, packet, plen, 0);
}

void proc_0x0A(share* shared, craftIk_epoll* clnt_epoll, int clnt_num){
	char message[1];
	recv(clnt_epoll->events[clnt_num].data.fd, message, (size_t)sizeof(message), 0);
	players* thisPlayer= getThisPlayer(clnt_epoll->events[clnt_num].data.fd);
	memcpy(&(thisPlayer->on_ground), message, sizeof(thisPlayer->on_ground));
	if(alive_tick_cool()== true){//TODO: alive_tick_cool
		send_keep_alive(shared, clnt_epoll, clnt_num);//TODO: send_keep_alive
	}
}

void proc_0x0B(share* shared, craftIk_epoll* clnt_epoll, int clnt_num){

	char message[33];
	double new_x_pos, new_y_pos, new_stance, new_z_pos;
	recv(clnt_epoll->events[clnt_num].data.fd, message, (size_t)sizeof(message), 0);
	players* thisPlayer= getThisPlayer(clnt_epoll->events[clnt_num].data.fd);//TODO: make get this player

	memcpy(&(new_x_pos), message, sizeof(thisPlayer->abs_x_pos));
	memcpy(&(new_y_pos), message+ 8, sizeof(thisPlayer->abs_y_pos));
	memcpy(&(new_stance), message+ 16, sizeof(thisPlayer->stance));
	memcpy(&(new_z_pos), message+ 24, sizeof(thisPlayer->abs_z_pos));

	detect_illegal_stance(shared, clnt_epoll, clnt_num, new_y_pos, new_stance);

	memcpy(&(thisPlayer->abs_x_pos), message, sizeof(thisPlayer->abs_x_pos));
	memcpy(&(thisPlayer->abs_y_pos), message+ 8, sizeof(thisPlayer->abs_y_pos));
	memcpy(&(thisPlayer->stance), message+ 16, sizeof(thisPlayer->stance));
	memcpy(&(thisPlayer->abs_z_pos), message+ 24, sizeof(thisPlayer->abs_z_pos));
	memcpy(&(thisPlayer->on_ground), message+ 32, sizeof(thisPlayer->on_ground));

}

void proc_0x0C(share* shared, craftIk_epoll* clnt_epoll, int clnt_num){
	char message[9];

	recv(clnt_epoll->events[clnt_num].data.fd, message, (size_t)sizeof(message), 0);

	players* thisPlayer= getThisPlayer(clnt_epoll->events[clnt_num].data.fd);

	memcpy(&(thisPlayer->yaw), message, sizeof(thisPlayer->yaw));
	memcpy(&(thisPlayer->pitch), message+ 4, sizeof(thisPlayer->pitch));
	memcpy(&(thisPlayer->on_ground), message+ 9, sizeof(thisPlayer->on_ground));
		
}

void proc_0x0D(share* shared, craftIk_epoll* clnt_epoll, int clnt_num){
	char message[41]={0};
	
	recv(clnt_epoll->events[clnt_num].data.fd, message, (size_t)sizeof(message), 0);

	players* thisPlayer= getThisPlayer(clnt_epoll->events[clnt_num].data.fd);
	
	memcpy(&(thisPlayer->abs_x_pos), message, sizeof(thisPlayer->abs_x_pos));
	memcpy(&(thisPlayer->abs_y_pos), message+ 8, sizeof(thisPlayer->abs_y_pos));
	memcpy(&(thisPlayer->stance), message+ 16, sizeof(thisPlayer->stance));
	memcpy(&(thisPlayer->abs_z_pos), message+ 24, sizeof(thisPlayer->abs_z_pos));
	memcpy(&(thisPlayer->yaw), message+ 32, sizeof(thisPlayer->yaw));
	memcpy(&(thisPlayer->pitch), message+ 36, sizeof(thisPlayer->pitch));
	memcpy(&(thisPlayer->on_ground), message+ 40, sizeof(thisPlayer->on_ground));

	memcpy(message+ 8, &(thisPlayer->stance), sizeof(thisPlayer->stance));
	memcpy(message+ 16, &(thisPlayer->abs_y_pos), sizeof(thisPlayer->abs_y_pos));


	send(clnt_epoll->events[clnt_num].data.fd, message, sizeof(message), 0);
}

void proc_0x00(share* shared, craftIk_epoll* clnt_epoll, int clnt_num){//TODO
	char message[1];
	recv(clnt_epoll->events[clnt_num].data.fd, message, (size_t)sizeof(message), 0);	
	
}
