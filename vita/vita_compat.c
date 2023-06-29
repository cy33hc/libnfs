/*
   Copyright (C) 2013 by Ronnie Sahlberg <ronniesahlberg@gmail.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.
*/
#ifdef __vita__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "vita_compat.h"

int nfs_getnameinfo(const struct sockaddr *sa, socklen_t salen,
char *host, size_t hostlen,
char *serv, size_t servlen, int flags)
{
  struct sockaddr_in *sin = (struct sockaddr_in *)sa;

  if (host) {
    snprintf(host, hostlen, inet_ntoa(sin->sin_addr));
  }

  return 0;
}

ssize_t nfs_writev(int fd, const struct iovec *vector, int count)
{
        /* Find the total number of bytes to be written.  */
        size_t bytes = 0;
        for (int i = 0; i < count; ++i) {
                /* Check for ssize_t overflow.  */
                if (((ssize_t)-1) - bytes < vector[i].iov_len) {
                        errno = EINVAL;
                        return -1;
                }
                bytes += vector[i].iov_len;
        }

        char *buffer = (char *)malloc(bytes);
        if (buffer == NULL)
                /* XXX I don't know whether it is acceptable to try writing
                the data in chunks.  Probably not so we just fail here.  */
                return -1;

        /* Copy the data into BUFFER.  */
        size_t to_copy = bytes;
        char *bp = buffer;
        for (int i = 0; i < count; ++i) {
                size_t copy = (vector[i].iov_len < to_copy) ? vector[i].iov_len : to_copy;

                memcpy((void *)bp, (void *)vector[i].iov_base, copy);

                bp += copy;
                to_copy -= copy;
                if (to_copy == 0)
                        break;
        }

        ssize_t bytes_written = write(fd, buffer, bytes);

        free(buffer);
        return bytes_written;
}
ssize_t nfs_readv (int fd, const struct iovec *vector, int count)
{
        /* Find the total number of bytes to be read.  */
        size_t bytes = 0;
        for (int i = 0; i < count; ++i)
        {
                /* Check for ssize_t overflow.  */
                if (((ssize_t)-1) - bytes < vector[i].iov_len) {
                        errno = EINVAL;
                        return -1;
                }
                bytes += vector[i].iov_len;
        }

        char *buffer = (char *)malloc(bytes);
        if (buffer == NULL)
                return -1;

        /* Read the data.  */
        ssize_t bytes_read = read(fd, buffer, bytes);
        if (bytes_read < 0) {
                free(buffer);
                return -1;
        }

        /* Copy the data from BUFFER into the memory specified by VECTOR.  */
        bytes = bytes_read;
        char *bp = buffer;
        for (int i = 0; i < count; ++i) {
                size_t copy = (vector[i].iov_len < bytes) ? vector[i].iov_len : bytes;

                memcpy((void *)vector[i].iov_base, (void *)bp, copy);

                bp += copy;
                bytes -= copy;
                if (bytes == 0)
                        break;
        }

        free(buffer);
        return bytes_read;
}

#endif