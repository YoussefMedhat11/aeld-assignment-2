#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <syslog.h>
#include <errno.h>


int main(int argc, char *argv[])
{
    int fd;

    /* Initialize syslog */
    openlog("writer", LOG_PID, LOG_USER);

    /* Validate input arguments */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <writefile> <writestr>\n",
                argv[0]);

        syslog(LOG_ERR, "Invalid number of arguments");

        closelog();
        return 1;
    }

    const char *writefile = argv[1];
    const char *writestr = argv[2];

    /* Create or overwrite the file */
    fd = open(writefile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1)
    {
        syslog(LOG_ERR, "Cannot open file %s: %s",
               writefile, strerror(errno));

        perror("open");
        closelog();
        return 1;
    }

    /* Log the write operation */
    syslog(LOG_DEBUG, "Writing %s to %s",
           writestr, writefile);

    /* Write the string to the file */
    size_t length = strlen(writestr);
    ssize_t bytes_written = write(fd, writestr, length);

    if (bytes_written == -1)
    {
        syslog(LOG_ERR, "Failed to write to %s: %s",
               writefile, strerror(errno));

        perror("write");

        close(fd);
        closelog();
        return 1;
    }

    if ((size_t)bytes_written != length)
    {
        syslog(LOG_ERR,
               "Partial write: expected %zu bytes, wrote %zd bytes",
               length, bytes_written);

        fprintf(stderr, "Error: Partial write\n");

        close(fd);
        closelog();
        return 1;
    }

    /* Close the file */
    if (close(fd) == -1)
    {
        syslog(LOG_ERR, "Cannot close file %s: %s",
               writefile, strerror(errno));

        perror("close");
        closelog();
        return 1;
    }

    closelog();

    return 0;
}
