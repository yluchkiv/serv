#include <unistd.h>
#include <string.h>

void process_http_request(int client_fd)
{
	const char *msg = "Hello from server over TCP!";
	write(client_fd, msg, strlen(msg));
	close(client_fd);
}
