/*
 * Debug Example Plugin
 *
 * Copyright (C) 2007, John Bailey <rekkanoryo@cpw.pidgin.im>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02111-1301, USA.
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* We're including glib.h again for the gboolean type. */
#include <glib.h>
#include <libpurple/blist.h>
#include <libpurple/account.h>
#include <libpurple/connection.h>

/* This will prevent compiler errors in some instances and is better explained in the
 * how-to documents on the wiki */
#ifndef G_GNUC_NULL_TERMINATED
#if __GNUC__ >= 4
#define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
#else
#define G_GNUC_NULL_TERMINATED
#endif
#endif

/* This is the required definition of PURPLE_PLUGINS as required for a plugin,
 * but we protect it with an #ifndef because config.h may define it for us
 * already and this would cause an unneeded compiler warning. */
#ifndef PURPLE_PLUGINS
#define PURPLE_PLUGINS
#endif

/* Here we're including the necessary libpurple headers for this plugin.  Note
 * that we're including them in alphabetical order.  This isn't necessary but
 * we do this throughout our source for consistency. */
#include "version.h"
#include "prpl.h"
#include "plugin.h"
#include "account.h"
#include "blist.h"
#include "buddyicon.h"
#include "debug.h"
#include "accountopt.h"

#include "impconnection.c"
/* It's more convenient to type PLUGIN_ID all the time than it is to type
 * "core-debugexample", so define this convenience macro. */
#define PLUGIN_ID "prpl-imparty"

/* Common practice in third-party plugins is to define convenience macros for
 * many of the fields of the plugin info struct, so we'll do that for the
 * purposes of demonstration. */
#define PLUGIN_AUTHOR "Manus Reload <manus.reload@gmail.com>"

static const gchar *
imparty_list_icon(PurpleAccount *account, PurpleBuddy *buddy) {
    return "irc";
}

static void imparty_login(PurpleAccount *account) {
    purple_debug_info("imparty", "login\n");
    imparty_connect(account);
}

static void imparty_set_status(PurpleAccount *acct, PurpleStatus *status) {
    purple_debug_info("imparty", "imparty_set_status\n");

}

static void imparty_close(PurpleConnection *pc) {
    purple_debug_info("imparty", "imparty_close\n");
    imparty_close_connection(pc);
}

static void imparty_set_buddy_icon(PurpleConnection *gc,
        PurpleStoredImage *img) {
    purple_debug_info("imparty", "setting %s's buddy icon to %s\n",
            gc->account->username,
            img ? purple_imgstore_get_filename(img) : "(null)");
}

static GList *imparty_status_types(PurpleAccount *acct) {
    GList *types = NULL;
    PurpleStatusType *type;

    purple_debug_info("nullprpl", "returning status types for %s\n",
            acct->username);

    type = purple_status_type_new_with_attrs(PURPLE_STATUS_AVAILABLE,
            "online", NULL, TRUE, TRUE, FALSE,
            "message", ("Message"), purple_value_new(PURPLE_TYPE_STRING),
            NULL);
    types = g_list_prepend(types, type);

    type = purple_status_type_new_with_attrs(PURPLE_STATUS_AWAY,
            "away", NULL, TRUE, TRUE, FALSE,
            "message", ("Message"), purple_value_new(PURPLE_TYPE_STRING),
            NULL);
    types = g_list_prepend(types, type);

    type = purple_status_type_new_with_attrs(PURPLE_STATUS_OFFLINE,
            "offline", NULL, TRUE, TRUE, FALSE,
            "message", ("Message"), purple_value_new(PURPLE_TYPE_STRING),
            NULL);
    types = g_list_prepend(types, type);

    return g_list_reverse(types);
}

char * imparty_status_text(PurpleBuddy *b) {
    IMPConnection * conn = (IMPFriend *) b->account->gc->proto_data;

    char * status = g_hash_table_lookup(conn->buddies, b->name);
    if (status != NULL) {
        if (strlen(status) > 0) {
            return g_strdup(status);
        }
    }
    return NULL;

}

static int imparty_send_im(PurpleConnection *gc, const char *who,
        const char *message, PurpleMessageFlags flags) {
    const char *from_username = gc->account->username;
    purple_debug_info("imparty", "sending message from %s to %s: %s\n",
            from_username, who, message);

    return imparty_client_send_im((IMPConnection *) gc->proto_data, who, message);
}
//const gchar *imparty_list_icon(PurpleAccount *account, PurpleBuddy *buddy);
////void imparty_login(PurpleAccount *account);
//void imparty_close(PurpleConnection *pc);

/* As we've covered before, libpurple calls this function, if present, when it
 * loads the plugin.  Here we're using it to show off the capabilities of the
 * debug API and just blindly returning TRUE to tell libpurple it's safe to
 * continue loading. */
static gboolean
plugin_load(PurplePlugin *plugin) {
    /* Define these for convenience--we're just using them to show the
     * similarities of the debug functions to the standard printf(). */
    //    gint i = 256;
    //    gfloat f = 512.1024;
    //    const gchar *s = "example string";
    //
    //    /* Introductory message */
    //    purple_debug_info(PLUGIN_ID,
    //            "Called plugin_load.  Beginning debug demonstration\n");
    //
    //    /* Show off the debug API a bit */
    //    purple_debug_misc(PLUGIN_ID,
    //            "MISC level debug message.  i = %d, f = %f, s = %s\n", i, f, s);
    //
    //    purple_debug_info(PLUGIN_ID,
    //            "INFO level debug message.  i = %d, f = %f, s = %s\n", i, f, s);
    //
    //    purple_debug_warning(PLUGIN_ID,
    //            "WARNING level debug message.  i = %d, f = %f, s = %s\n", i, f, s);
    //
    //    purple_debug_error(PLUGIN_ID,
    //            "ERROR level debug message.  i = %d, f = %f, s = %s\n", i, f, s);
    //
    //    purple_debug_fatal(PLUGIN_ID,
    //            "FATAL level debug message. i = %d, f = %f, s = %s\n", i, f, s);

    /* Now just return TRUE to tell libpurple to finish loading. */
    return TRUE;
}

static PurplePluginProtocolInfo prpl_info = {
    OPT_PROTO_CHAT_TOPIC, // options
    NULL, // user_splits
    NULL, // protocol_options
    {
        "png,jpeg", 0, 0, 64, 64, 0, PURPLE_ICON_SCALE_DISPLAY
    }, // icon_spec
    imparty_list_icon, // list_icon
    NULL, // list_emblem
    imparty_status_text, // status_text
    NULL, // tooltip_text
    imparty_status_types, // status_types
    NULL, // blist_node_menu
    NULL, // chat_info
    NULL, // chat_info_defaults
    imparty_login, // login
    imparty_close, // close
    imparty_send_im, // send_im
    NULL, // set_info
    NULL, // send_typing
    NULL, // get_info
    imparty_set_status, // set_status
    NULL, // set_idle
    NULL, // change_passwd
    NULL, // add_buddy
    NULL, // add_buddies
    NULL, // remove_buddy
    NULL, // remove_buddies
    NULL, // add_permit
    NULL, // add_deny
    NULL, // rem_permit
    NULL, // rem_deny
    NULL, // set_permit_deny
    NULL, // join_chat
    NULL, // reject_chat
    NULL, // get_chat_name
    NULL, // chat_invite
    NULL, // chat_leave
    NULL, // chat_whisper
    NULL, // chat_send
    NULL, // keepalive
    NULL, // register_user
    NULL, // get_cb_info
    NULL, // get_cb_away
    NULL, // alias_buddy
    NULL, // group_buddy
    NULL, // rename_group
    NULL, // buddy_free
    NULL, // convo_closed
    NULL, // normalize
    imparty_set_buddy_icon, // set_buddy_icon
    NULL, // remove_group
    NULL, // get_cb_real_name
    NULL, // set_chat_topic
    NULL, // find_blist_chat
    NULL, // roomlist_get_list
    NULL, // roomlist_cancel
    NULL, // roomlist_expand_category
    NULL, // can_receive_file
    NULL, // send_file
    NULL, // new_xfer
    NULL, // offline_message
    NULL, // whiteboard_prpl_ops
    NULL, // send_raw
    NULL, // roomlist_room_serialize
    NULL, // unregister_user
    NULL, // send_attention
    NULL, // get_attention_types
    sizeof (PurplePluginProtocolInfo), // struct_size
    NULL, // account_text_table
    NULL, // initiate_media
    NULL, // media_caps
    NULL, // get_moods
    NULL, // set_public_alias
    NULL // get_public_alias
};

static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC, /* magic number */
    PURPLE_MAJOR_VERSION, /* purple major */
    PURPLE_MINOR_VERSION, /* purple minor */
    PURPLE_PLUGIN_PROTOCOL, /* plugin type */
    NULL, /* UI requirement */
    0, /* flags */
    NULL, /* dependencies */
    PURPLE_PRIORITY_DEFAULT, /* priority */

    PLUGIN_ID, /* id */
    "IMParty", /* name */
    DISPLAY_VERSION, /* version */
    "Plugin to chat whit IMP", /* summary */
    "IMParty plugin for Pidgin", /* description */
    PLUGIN_AUTHOR, /* author */
    "http://imparty.no-ip.org/", /* homepage */

    plugin_load, /* load */
    NULL, /* unload */
    NULL, /* destroy */

    NULL, /* ui info */
    &prpl_info, /* extra info */
    NULL, /* prefs info */
    NULL, /* actions */
    NULL, /* reserved */
    NULL, /* reserved */
    NULL, /* reserved */
    NULL /* reserved */
};

static void
init_plugin(PurplePlugin *plugin) {
    purple_debug_info("imparty", "init\n");

    /*
        PurpleAccountOption *option;
        PurplePluginInfo *info = plugin->info;
        PurplePluginProtocolInfo *prpl_info = info->extra_info;
     */

    //
    //    option = purple_account_option_string_new("Host", "host", "belugapods.com");
    //    prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);
    //
    //    option = purple_account_option_int_new("Initial Messages fetched", "seedFetchCount", 30);
    //    prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);
    //
    //    option = purple_account_option_string_new("Display Photo Link as", "photo", "(Photo)");
    //    prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);
    //
    //    option = purple_account_option_string_new("Display Location Link as", "location", "(*)");
    //    prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);
    //
    //    option = purple_account_option_bool_new("Automatically Check for Software Updates", "checkVersion", TRUE);
    //    prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);
    //
    //    option = purple_account_option_bool_new("Notify me of Beta Versions", "allowBetaVersions", FALSE);
    //    prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);
    //
    //    option = purple_account_option_bool_new("Display Debug Messages", "debugMsgs", FALSE);
    //    prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);
}

PURPLE_INIT_PLUGIN(imp, init_plugin, info)

