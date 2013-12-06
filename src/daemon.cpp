// For fork()
#include <unistd.h>

// For umask()
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>

// For exit()
#include <cstdlib>

// other...
#include <fstream>
#include "server.hpp"

int main(int argc, char **argv)
{
	pid_t	pid, sid;

	// Starting daemon
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	else if (pid > 0)
		exit(EXIT_SUCCESS);
	
	umask(0);
	
	setlogmask (LOG_UPTO(LOG_NOTICE));
	openlog("aionlyd", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	syslog(LOG_INFO, "Staring AI-Only daemon...");

	sid = setsid();
	if (sid < 0)
	{
		syslog(LOG_ERR, "Failed create new unique session.");
		closelog();
		exit(EXIT_FAILURE);
	}

	if ((chdir("/")) < 0)
	{
		syslog(LOG_ERR, "Failed change directory.");
		closelog();
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	// Initializing daemon
	AIOnly::Server ai(5000);

	while (1)
	{
		sleep(1);
	}

	// Stopping daemon
	syslog(LOG_INFO, "Success stopping AI-Only daemon.");
	closelog();
	return EXIT_SUCCESS;
}
