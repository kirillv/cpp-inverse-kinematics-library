#ifndef __STRINGLEXER__
#define __STRINGLEXER__

#include <stdio.h>
#include <string>

#include "SharedTypes.h"
#include "config.h"

enum Token {
    INT, FLOAT, SPACE, END , VAR , ERROR
};

Token scan_string(
                  IN  const char * p, 
                  IN  unsigned int str_len,
                  OUT std::string & out_default_var
                  );

#endif