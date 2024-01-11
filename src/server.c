/*
 The GPLv3 License (GPLv3)

 Copyright (c) 2024 Vladislav 'ElCapitan' Nazarov (AT PROJECT)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WAr->aNTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOr->a PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "server.h"

struct Server *create_server(int domain, int service, int protocol,
                             long interface, int port, char *files) {
  struct Server *server = (struct Server *)malloc(sizeof(struct Server));

  server->protocol = protocol;
  server->files_path = files;

  server->address.sin_family = domain;
  server->address.sin_addr.s_addr = htonl(interface);
  server->address.sin_port = htonl(port);

  if ((server->socket = socket(domain, service, protocol)) == 0) {
    free(server);
    return NULL;
  }

  return server;
}

void free_server(struct Server *server) { free(server); }
