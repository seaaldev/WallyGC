#ifndef WDEBUG_H
#define WDEBUG_H

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0);

#endif
