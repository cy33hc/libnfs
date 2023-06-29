/* -*-  mode:c; tab-width:8; c-basic-offset:8; indent-tabs-mode:nil;  -*- */
/*
   Copyright (C) 2021 by Ronnie Sahlberg <ronniesahlberg@gmail.com>

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

#ifndef _VITA_COMPAT_H_
#define _VITA_COMPAT_H_

#ifdef __vita__

#include <errno.h>
#include <sys/time.h>
#include <sys/utime.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/errno.h>

#define IFNAMSIZ 16
#define IPPORT_RESERVED   1024
#define MSG_NOSIGNAL      0x20000
#define O_NOFOLLOW        0400000
#define MINORBITS         20
#define MINORMASK         ((1U << MINORBITS) - 1)

#define major(dev)        ((unsigned int) ((dev) >> MINORBITS))
#define minor(dev)        ((unsigned int) ((dev) & MINORMASK))

#define getservbyport(a,b) NULL

struct statvfs {
        unsigned long int f_bsize;
        unsigned long int f_frsize;
        unsigned long int f_blocks;
        unsigned long int f_bfree;
        unsigned long int f_bavail;
        unsigned long int f_files;
        unsigned long int f_ffree;
        unsigned long int f_favail;
        unsigned long int f_fsid;
        unsigned long int f_flag;
        unsigned long int f_namemax;
};

//#define getlogin_r(a,b) ENXIO

#define writev nfs_writev
#define readv nfs_readv
#define getnameinfo nfs_getnameinfo
ssize_t nfs_writev(int fd, const struct iovec *iov, int iovcnt);
ssize_t nfs_readv(int fd, const struct iovec *iov, int iovcnt);
int nfs_getnameinfo(const struct sockaddr *sa, socklen_t salen,
char *host, size_t hostlen,
char *serv, size_t servlen, int flags);

#define SOL_TCP IPPROTO_TCP
#define EAI_AGAIN EAGAIN

#define IFF_UP            0x1     /* interface is up          */
#define IFF_BROADCAST     0x2     /* broadcast address valid  */
#define IFF_DEBUG         0x4     /* turn on debugging        */
#define IFF_LOOPBACK      0x8     /* is a loopback net        */

struct ifmap {
       unsigned long mem_start;
       unsigned long mem_end;
       unsigned short base_addr; 
       unsigned char irq;
       unsigned char dma;
       unsigned char port;
       /* 3 bytes spare */
};
  
struct ifreq {
    char ifr_name[IFNAMSIZ]; /* Interface name */
    union {
        struct sockaddr ifr_addr;
        struct sockaddr ifr_dstaddr;
        struct sockaddr ifr_broadaddr;
        struct sockaddr ifr_netmask;
        struct sockaddr ifr_hwaddr;
        short           ifr_flags;
        int             ifr_ifindex;
        int             ifr_metric;
        int             ifr_mtu;
        struct ifmap    ifr_map;
        char            ifr_slave[IFNAMSIZ];
        char            ifr_newname[IFNAMSIZ];
        char           *ifr_data;
    };
};

struct ifconf {
    int                 ifc_len; /* size of buffer */
    union {
        char           *ifc_buf; /* buffer address */
        struct ifreq   *ifc_req; /* array of structures */
    };
};

#endif /* __vita__ */

#endif /* _VITA_COMPAT_H_ */
