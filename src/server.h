/*
 The GPLv3 License (GPLv3)

 Copyright (c) 2024 Vladislav 'ElCapitan' Nazarov (AT PROJECT)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef kugisaki_SERVER
#define kugisaki_SERVER

#include "log.h"

#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include "response.h"
#include "types.h"

struct Server {
  int socket;
  int protocol;
  int port;

  struct sockaddr_in address;
  char *files_path;
};

struct Server *create_server(int domain, int service, int protocol,
                             long interface, int port, int backlog,
                             char *files);
void free_server(struct Server *server);
char *process_request(struct Request *req);

#endif
