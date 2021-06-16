#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <prompt.h>


#include <urjtag/chain.h>
#include <urjtag/bus.h>
#include <urjtag/cmd.h>
#include <urjtag/flash.h>
#include <urjtag/parse.h>
#include <urjtag/jtag.h>

#include <urjtag/repl.h>


#define URJ_REPL_DEBUG
#undef URJ_REPL_DEBUG

/*
  internal function prototypes
*/

static int urj_repl_init(lua_State *);
static int urj_command(lua_State *);

static int urj_repl_init(lua_State *L)
{
    printf("priv is 0x%p\n", &repl_priv);
    lua_pushlightuserdata(L, &repl_priv);
    
    return 1;
}



static int urj_command(lua_State *L)
{
    repl_private *p = lua_touserdata(L, 1);
    const char *line = lua_tostring(L, 2);
    char *s;
    int rc = 0;
    
    p->message_cur = p->message;
    p->error_message_cur = p->error_message;
#ifdef URJ_REPL_DEBUG
    fprintf(stderr, "p is 0x%p\n", p);
    fprintf(stderr, "chainp is 0x%p\n", p->chain);
    fprintf(stderr, "line is |%s|.\n", line);
    if ((s = strchr(line, ' '))!= NULL) {
	char *opcode = strdup(line);
	s = strchr(opcode, ' ');
	*s++ = '\0';
	fprintf(stderr, "opcode is |%s|.\n", opcode);
	fprintf(stderr, "oprand is |%s|.\n", s);
	free(opcode);
    }
#endif
    
    if ((p->urj_status = urj_parse_line (p->chain, line)) == URJ_STATUS_OK) {
	lua_pushinteger(L, URJ_STATUS_OK), rc++;
	lua_pushnil(L), rc++;
#ifdef URJ_REPL_DEBUG
	fprintf(stderr, "command result %d\n", rc);
#endif
    } else {
	urj_error_t error = urj_error_get();
	const char *error_string = urj_error_string(error);
	lua_pushinteger(L, error), rc++;
	lua_pushstring(L, error_string), rc++;
#ifdef URJ_REPL_DEBUG
	fprintf(stderr, "command result %d\n", rc);
	fprintf(stderr, "command error code is %s.\n", error_string);
#endif
    }

    if (*(p->message)) {
	lua_pushstring(L, p->message), rc++;
#ifdef URJ_REPL_DEBUG
	fputs(">>> message <<<", stderr);
	fputs(p->message, stderr);
#endif
    } else {
	lua_pushnil(L), rc++;
    }
    if (*(p->error_message)) {
	lua_pushstring(L, p->error_message), rc++;
#ifdef URJ_REPL_DEBUG
	fputs(">>> error message <<<", stderr);
	fputs(p->error_message, stderr);
#endif
    } else {
	lua_pushnil(L), rc++;
    }
    *(p->message) = '\0';
    *(p->error_message) = '\0';
    p->message_cur = p->message;
    p->error_message_cur = p->error_message;
    
    return rc;
}


int jtag_lua_repl(urj_chain_t *chain)
{
    lua_State *L;
    char *env;

    if (chain != NULL) {
	repl_priv.chain = chain;
#ifdef URJ_REPL_DEBUG
	fprintf(stderr, "chian is 0x%p.\n", chain);
#endif
    }
    if (repl_priv.message == NULL) {
	repl_priv.message = malloc(URJ_MAX_MESSAGE_SIZE);
	if (repl_priv.message == NULL) {
	    fprintf(stderr, "memory allocation error.\n");
	    return -1;
	}
	repl_priv.message_cur = repl_priv.message;
	*(repl_priv.message) = '\0';
    }
    if (repl_priv.error_message == NULL) {
	repl_priv.error_message = malloc(URJ_MAX_MESSAGE_SIZE);
	if (repl_priv.error_message == NULL) {
	    fprintf(stderr, "memory allocation error.\n");
	    return -1;
	}
	repl_priv.error_message_cur = repl_priv.error_message;
	*(repl_priv.error_message) = '\0';
    }
    if (!(L = luaL_newstate())) {
    	fprintf(stderr, "luaL_newstate() returns NULL\n");
	exit(1);
    }
    luaL_openlibs(L);
    luap_setname (L, "urjtag-lua");
    env = getenv("URJTAG_LUA_COLOR");
#ifdef URJ_REPL_DEBUG
    if (env) printf("env=|%s|\n", env);
#endif
    if ((env == NULL) || (*env == '0')) {
	luap_setcolor (L, 0);
    }
    lua_register(L, "urj_init", urj_repl_init);
    lua_register(L, "urj", urj_command);
    luap_enter(L);
}
