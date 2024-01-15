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

void free_server(struct Server *server) {
  close(server->socket);
  free(server);
}

struct Server *create_server(int domain, int service, int protocol,
                             long interface, int port, int backlog,
                             char *files) {
  struct Server *server = malloc(sizeof(struct Server));

  if (server == NULL) {
    log_error("Error while allocating memory: %s", strerror(errno));
    return NULL;
  }

  server->protocol = protocol;
  server->files_path = files;
  server->port = port;

  server->address.sin_family = domain;
  server->address.sin_addr.s_addr = htons(interface);
  server->address.sin_port = htons(port);

  log_debug("Link,");
  if ((server->socket = socket(domain, service, protocol)) < 0) {
    log_error("Error creating socket: %s", strerror(errno));
    free_server(server);
    return NULL;
  }

  if ((bind(server->socket, (struct sockaddr *)&server->address,
            sizeof(server->address))) < 0) {
    log_error("Error binding socket: %s", strerror(errno));
    free_server(server);
    return NULL;
  }

  if ((listen(server->socket, backlog)) < 0) {
    log_error("Listening start failed: %s", strerror(errno));
    free_server(server);
    return NULL;
  }
  return server;
}

char *process_request(struct Request *req) {
  char *filetype = strchr(req->request_file, '.');
  filetype++;
  char header[250] = "";

  char *path = (char *)malloc(sizeof(char) * (strlen(req->request_file) + 6));
  sprintf(path, "./html%s", req->request_file);

  FILE *fd = fopen(path, "r");

  if (fd == NULL) {
    log_warn("Requested file %s not found", path);
    free(path);
    return NULL;
  }

  log_debug("Filetype: ", filetype);

  if (strcmp(filetype, "css") == 0) {
    strcpy(header, "");
  } else if (strcmp(filetype, "html") == 0) {
    strcpy(header, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
  }

  fseek(fd, 0, SEEK_END);
  int fsize = ftell(fd);
  rewind(fd);

  char *response_content = (char *)malloc(sizeof(char) * fsize);
  fread(response_content, 1, fsize, fd);
  fclose(fd);

  char *res = (char *)malloc(sizeof(char) *
                             (strlen(header) + strlen(response_content)));

  strcpy(res, header);
  strcat(res, response_content);
  free(response_content);
  free(path);
  return res;
}
