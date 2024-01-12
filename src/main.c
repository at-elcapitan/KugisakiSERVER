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
#include "main.h"

const char resp[] = "HTTP/1.1 200 OK\n"
                    "Content-Type: text/html\n\n"
                    "<!DOCTYPE html>\n"
                    "<html>\n"
                    "<head>\n"
                    "<title>Kugisaki SERVER is alive!</title>\n"
                    "</head>\n"
                    "<body>\n"
                    "<h1>Kugisaki SERVER is alive!</h1>\n"
                    "<p>Hello, user! The server is up and running.</p>\n"
                    "</body>\n"
                    "</html>\n";

struct Server *server;
bool running = true;

void sigint_handler() {
  running = false;
  free_server(server);
  log_info("Server struct at %p freed", server);
}

int main(int argc, char *argv[]) {
  int port = 80;

  log_set_level(LOG_INFO);

  if (argc > 1) {
    int i = 0;
    for (; i < argc; ++i) {
      if (strcmp(argv[i], "-verbose") == 0) {
        log_set_level(LOG_DEBUG);
      }

      if (strcmp(argv[i], "-port") == 0) {
        if (argc <= i + 1) {
          puts("Missing port number. Usage: -port <value>");
          return 0;
        }

        port = strtol(argv[i + 1], NULL, 0);

        if (port > 65535) {
          puts("Error: The specified port exceeds the allowed range. Please "
               "choose a port number between 1 and 65535.");
          return 0;
        }
        i++;
      }

      if (strcmp(argv[i], "-version") == 0) {
        puts("nkserver ver. 0.0.0 202401120654\n\n"
             "NKServer Copyright (C) 2024 Vladislav 'ElCapitan' Nazarov (AT "
             "PROJECT)\nThis program comes with ABSOLUTELY NO WARRANTY; for "
             "details check `LICENSE` file.\nThis is free software, and you"
             "are welcome to redistribute it "
             "under certain conditions.");
        return 0;
      }
    }
  }

  char buf;
  FILE *kugisaki = fopen("banner", "r");

  if (kugisaki != NULL) {
    while ((buf = fgetc(kugisaki)) != EOF) {
      putchar(buf);
    }
    putchar(0xA);
    fclose(kugisaki);
  } else {
    puts("AT PROJECT, 2024\nKugisaki SERVER\n");
  }

  signal(SIGINT, sigint_handler);
  server = create_server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 10, "");

  if (server == NULL) {
    log_fatal("Cannot create server, exiting...");
    return -1;
  }

  log_debug("start..");
  log_info("Server started listening port %d at %d", server->port,
           htonl(server->address.sin_addr.s_addr));

  char req_data[30000];
  while (running) {
    int adrsize = sizeof(server->address);
    int clsocket = accept(server->socket, (struct sockaddr *)&server->address,
                          (socklen_t *)&adrsize);

    if (clsocket < 0) {
      log_error("Error accepting data: %s", strerror(errno));
      continue;
    }

    if ((read(clsocket, req_data, sizeof(req_data))) < 0) {
      log_error("Error reading data: %s", strerror(errno));
    }

    log_info("Accepting `%s` from ", strtok(req_data, "\n"));
    printf("%s\n", req_data);
    write(clsocket, resp, sizeof(resp));
    close(clsocket);
  }
  return 0;
}
