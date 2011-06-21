#include "lexer.h"

#ifdef _DEBUG
#define dbgprintf(message,...) printf(message,__VA_ARGS__)
#else
#define dbgprintf(message)
#endif


int scan(char** p, char** lex)
{
    char* marker;
    if (lex) *lex = *p;

	{
		unsigned char yych;

		yych = (unsigned char)**p;
		if (yych <= ' ') {
			if (yych == '\t') goto yy6;
			if (yych <= 0x1F) goto yy8;
			goto yy6;
		} else {
			if (yych <= '9') {
				if (yych <= '/') goto yy8;
				goto yy4;
			} else {
				if (yych != 'v') goto yy8;
			}
		}
		yych = (unsigned char)*(marker = ++*p);
		if (yych == 'a') goto yy16;
yy3:
		{ return END; }
yy4:
		++*p;
		if ((yych = (unsigned char)**p) == '.') goto yy11;
		if (yych <= '/') goto yy5;
		if (yych <= '9') goto yy14;
yy5:
		{ return INT; }
yy6:
		++*p;
		yych = (unsigned char)**p;
		goto yy10;
yy7:
		{ return SPACE; }
yy8:
		yych = (unsigned char)*++*p;
		goto yy3;
yy9:
		++*p;
		yych = (unsigned char)**p;
yy10:
		if (yych == '\t') goto yy9;
		if (yych == ' ') goto yy9;
		goto yy7;
yy11:
		++*p;
		yych = (unsigned char)**p;
		if (yych <= '/') goto yy13;
		if (yych <= '9') goto yy11;
yy13:
		{ return FLOAT; }
yy14:
		++*p;
		yych = (unsigned char)**p;
		if (yych == '.') goto yy11;
		if (yych <= '/') goto yy5;
		if (yych <= '9') goto yy14;
		goto yy5;
yy16:
		yych = (unsigned char)*++*p;
		if (yych == 'r') goto yy18;
yy17:
		*p = marker;
		goto yy3;
yy18:
		yych = (unsigned char)*++*p;
		if (yych != '_') goto yy17;
		++*p;
		{ return VAR; }
	}

}

Token scan_string(
                  const char * pp, 
                  unsigned int str_len,
                  std::string & out_default_var
                 )
{
  char *p = (char *)pp;
  char *last;
  int token;

  token = scan(&p, &last);
  int sz = p - last;
  switch (token) 
  {
    case FLOAT: 
#ifdef DEBUGOUTPUTFORLEXER
                dbgprintf("FLOAT: '%.*s'\n", sz, last); 
#endif
                break;
    case INT  : 
#ifdef DEBUGOUTPUTFORLEXER
                dbgprintf("INT: '%.*s'\n", sz, last); 
#endif
                break;
    case VAR  : 
#ifdef DEBUGOUTPUTFORLEXER
                dbgprintf("VAR: '%.*s'\n", sz, last);
#endif
                if(str_len==sz){
                   token = (int)ERROR;
                   break;
                }
                //We should get default value
                token = scan(&p, &last);

                if(token != FLOAT && token != INT){
                  token = (int)ERROR;
                  break;
                }

                sz = p - last;

                out_default_var = std::string(last,sz);

                token = (int)VAR;

                break;
    default   : 
                token = (int)ERROR;
  }



  return (Token)token;
}