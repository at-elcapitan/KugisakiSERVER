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

int main(/*int argc, char *argv[]*/) {
  char buffer;
  FILE *kugisaki = fopen("banner", "r");

  if (kugisaki != NULL) {
    while ((buffer = fgetc(kugisaki)) != EOF) {
      putchar(buffer);
    }
    putchar(0xA);
    fclose(kugisaki);
  } else {
    puts("AT PROJECT, 2024\nKugisaki SERVER\n");
  }

  struct Server *server =
      create_server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, "");

  if (server == NULL) {
    log_fatal("Cannot create server, exiting...");
    return -1;
  }
  log_info("Server created");

  free_server(server);
  log_info("Server struct at %p freed", server);
  return 1;
}
