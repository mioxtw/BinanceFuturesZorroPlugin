#pragma once
#include <curl/curl.h>

#include "cJSON.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static CURL *gcurl;
static CURLcode res;

struct chat {
    long id;
	char *type;
	char *title;
	char *username;
    char *firstname;
    int allmembersareadmins;
};

struct parameters {
    unsigned char *keyword;
    void *actualvalue;
    int usable;
};

struct user {
    char *firstname;
    char *language_code;
    long id;
    char *username;
    int is_bot;
};

struct message {
    int id;
    char *text;
    struct user from;
    struct chat chat;
    int date;
};

struct result {
    struct message message;
};

struct updates {
    int ok;

    struct result result;
};

char *call(char const *w, char const *parameters);
void make_param(struct parameters *params, char const *keyword, char const *value, int index);
char *compile_post_parameters(struct parameters *params);
void setup_bot(const char *token);
void get_updates(struct updates *update);
void send_message(long chat_id, char const *text);
