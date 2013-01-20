#ifndef _SHARE_H_
#define _SHARE_H_

#include "craftik.h"

typedef struct {
	char generator_settings[1024];
	bool allow_nether;
	char level_name[1024];
	bool enable_query;
	bool allow_flight;
	int server_port;
	int level_type;
	bool enable_rcon;
	char level_seed[1024];
	char server_ip[1024];
	int max_build_height;
	bool spawn_npcs;
	bool white_list;
	bool spawn_animals;
	bool snooper_enabled;
	bool hardcore;
	char texture_pack[1024];
	bool online_mode;
	bool pvp;
	int difficulty;
	int gamemode;
	int max_players;
	bool spawn_monsters;
	bool generate_structures;
	int view_distance;
	char motd[1024];
	int num_workers;
	int num_connections_per_worker;
	int port;
	int protocol_version;//TODO: jincreator
	char server_version[64];//TODO: jincreator
} properties;

typedef struct players{
	int EID;
	char* player_name;
	double abs_x_pos;
	double abs_y_pos;
	double abs_z_pos;
	double stance;
	bool on_ground;

	float yaw;
	float pitch;

	//TODO:add necessary variable
} players;

typedef struct {
	int TODO;
} connections;

typedef struct {
	properties* prop;
	connections* conn;
	int listen_sock;
	players* player;
	int player_count;
	RSA* private_key;
} share;

#endif
