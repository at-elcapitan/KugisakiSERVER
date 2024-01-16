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

struct Response *process_request(struct Request *req) {
  struct Response *resp = (struct Response *)malloc(sizeof(struct Response));

  char path[strlen(req->request_file) + 6];
  char *filetype = strchr(req->request_file, '.');
  char respcode[RMAXSIZE] = "";
  char conttype[CMAXSIZE] = "";

  FILE *fd;

  if (filetype == NULL) {
    filetype = "html";
    strcat(req->request_file, "/index.html");
  } else
    filetype++;

  resp->response_header = (char *)malloc(250);
  sprintf(path, "./html%s", req->request_file);

  fd = fopen(path, "rb");

  if (fd == NULL) {
    log_debug("Requested file %s not found", path);

    fd = fopen(P404, "r");

    if (fd == NULL) {
      log_warn("Unable to open error page");
      return NULL;
    }

    filetype = "html";
    strcpy(respcode, R404);
  } else {
    strcpy(respcode, R200);
  }

  log_debug("Filetype: %s", filetype);

  fseek(fd, 0, SEEK_END);
  size_t fsize = ftell(fd);
  fseek(fd, 0, SEEK_SET);

  if (strcmp(filetype, "html") == 0) {
    strcpy(conttype, CHTML);
  } else if (strcmp(filetype, "mp3") == 0) {
    strcpy(conttype, CMPEG);
  } else if (strcmp(filetype, "ico") == 0) {
    strcpy(conttype, CXICO);
  } else if (strcmp(filetype, "mp4") == 0) {
    strcpy(conttype, CVMP4);
  } else if (strcmp(filetype, "jpg") == 0 || strcmp(filetype, "jpeg") == 0) {
    strcpy(conttype, CJPEG);
  } else if (strcmp(filetype, "png") == 0) {
    strcpy(conttype, CIPNG);
  }

  sprintf(resp->response_header,
          "HTTP/1.1 %s\r\n"
          "Content-Type: %s\r\n"
          "Content-Length: %zu\r\n"
          "Connection: close\r\n\r\n",
          respcode, conttype, fsize);

  log_debug("Filesize: %d", fsize);

  resp->response_content = (char *)malloc(fsize);
  resp->content_size = fsize;
  fread(resp->response_content, 1, fsize, fd);

  fclose(fd);
  return resp;
}
