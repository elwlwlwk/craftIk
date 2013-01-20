#include "proc_lib.h"

bool alive_tick_cool(){
	//TODO
	return true;
}

players* getThisPlayer(int clnt_fd){//TODO
	players* P= NULL;

	return P;
}

int int2crft_str(char* buffer, int src, int buffer_size){
	memset(buffer, 0, buffer_size);
	
	short src_length=0;
	for(int i=1; src/i!= 0; i*= 10){
		src_length++;
	}
	src_length*= 2;
	memcpy(buffer, &src_length, sizeof(short));
	for(int i=0; i< src_length; i++){
		buffer[1+(2*(src_length- i))]= src/10;
		src/= 10;
	}
	return src_length;
}

int ucs_str2crft_str(char* buffer, char* src, int buffer_size){

	short src_len= 0;
	memset(buffer, 0, buffer_size);
	
	while(src[src_len]!= 0x00&& src[src_len+1]!= 0x00){
		src_len++;
	}
	memcpy(buffer, &src_len, sizeof(short));
	memcpy(buffer+ sizeof(short), src, src_len);
	
	return src_len;
}

int ascii_str2crft_str(char* buffer, char* src, int buffer_size){
	//TODO
	return 0;
}

int ucs_str_length(const char* str, int arr_size){
	int str_len=0;
	for(int i= 0; i< arr_size-1; i++){
		if(str[i]== 0&& str[i+1]== 0){
			str_len= i;
			break;
		}
	}
	if(str_len== 0){
		str_len= arr_size;
	}
	return str_len;
}

void send_keep_alive(share* shared, craftIk_epoll* clnt_epoll, int clnt_num){
//		
}

void detect_illegal_stance(share* shared, craftIk_epoll* clnt_epoll, int clnt_num, double new_y_pos, double new_stance){
	if((new_stance- new_y_pos< 0.1)|| (new_stance- new_y_pos> 1.65)){
		kick_player(shared, clnt_epoll, clnt_num, "Illegal Stance");
	}
}

void kick_player(share* shared, craftIk_epoll* clnt_epoll, int clnt_num, char* message){
//TODO

}

