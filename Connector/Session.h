#include <sys/socket.h>
#include <time.h>

typedef struct Session{
	struct sockaddr* sock_addr;
	int fd;
	time_t addTime;
	int isLoggedIn;
	struct Session* NextSession;

	void (*setSession)(struct Session* Ses, struct sockaddr* sock_addr, int fd, time_t addTime, int isLoggedIn);
} Session;

Session* createSession(void);
void setSession(Session* Ses, struct sockaddr* sock_addr, int fd, time_t addTime, int isLoggedIn);

typedef struct SsList{
	Session* SsHead;
	
	int (*Append)(struct SsList* SList, struct sockaddr* sock_addr, int fd, time_t addTime, int isLoggedIn);
	void (*Delete)(struct SsList* SList, int fd);
	int (*getTimeOut)(struct SsList* SList, int DurationTime);

} SsList;

SsList* CreateSsList(void);

int Append(SsList* SList, struct sockaddr* sock_addr, int fd, time_t addTime, int isLoggedIn);

int Delete(SsList* SList, int fd);

int getTimeOut(SsList* SList, int DurTime);
