/*
 * $Id$
 *
 * Copyright (C) 2021, Ike
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 */

#ifndef URJ_REPL_H
#define URJ_REPL_H
#ifdef HAVE_LUA_REPL

typedef struct {
    urj_chain_t *chain;
    int urj_status;
    char *message;
    char *message_cur;
    char *error_message;
    char *error_message_cur;
} repl_private;

#define URJ_MAX_MESSAGE_SIZE 4096

extern repl_private repl_priv;

#define urj_repl_set_status(x) do { repl_priv.status = x; }while(0)
/* #define urj_repl_set_message(x) do { \ */
/* 	strncpy(repl_priv.message, x, URJ_MAX_MESSAGE_SIZE); \ */
/* 	repl_priv.mssage[URJ_MAX_MESSAGE_SIZE - 2] = '\n'; \ */
/* 	repl_priv.mssage[URJ_MAX_MESSAGE_SIZE - 1] = '\0'; \ */
/*     }while(0) */
/* #define urj_repl_set_error_message(x) do { \ */
/* 	strncpy(repl_priv.error_message, x, URJ_MAX_MESSAGE_SIZE); \ */
/* 	repl_priv.error_mssage[URJ_MAX_MESSAGE_SIZE - 2] = '\n'; \ */
/* 	repl_priv.error_mssage[URJ_MAX_MESSAGE_SIZE - 1] = '\0'; \ */
/*     }while(0) */

#define urj_repl_message()		(repl_priv.message_cur)
#define urj_repl_error_message()	(repl_priv.error_message_cur)
#define urj_repl_message_inc(x)		do { \
	if ((repl_priv.message_cur + (x) - repl_priv.message) < URJ_MAX_MESSAGE_SIZE) \
	    repl_priv.message_cur += (x); \
    }while(0)

#define urj_repl_error_message_inc(x)	do { \
	if ((repl_priv.error_message_cur + (x) - repl_priv.error_message) < URJ_MAX_MESSAGE_SIZE) \
	    repl_priv.error_message_cur += (x);	\
    }while(0)

#define is_urj_repl_message_in_range()	((repl_priv.message_cur - repl_priv.message) <= URJ_MAX_MESSAGE_SIZE)
#define is_urj_repl_error_message_in_range()	((repl_priv.error_message_cur - repl_priv.error_message) <= URJ_MAX_MESSAGE_SIZE)

#define urj_tracate_message()	do { \
	const char *trunc_msg = "(truncated)\n"; \
	strcpy(repl_priv.message + URJ_MAX_MESSAGE_SIZE - sizeof(trunc_msg) - 2, trunc_msg); \
    }while(0)

#define urj_tracate_error_message()	do { \
	const char *trunc_msg = "(truncated)\n";	\
	strcpy(repl_priv.error_message + URJ_MAX_MESSAGE_SIZE - sizeof(trunc_msg) - 2, trunc_msg); \
    }while(0)



int jtag_lua_repl(urj_chain_t *);

#endif	/* HAVE_LUA_REPL */
#endif	/* URJ_REPL_H */
