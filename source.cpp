#include "source.h"

// https://curl.haxx.se/libcurl/c/getinmemory.html
struct MemoryStruct {
    char *memory;
    size_t size;
};

struct bot {
    const char *token;
};

struct bot *_bot=0;
int _last_update=9999999;

static char *_update_keys[3] = {"ok", "result"};
static char *_message_keys[4] = {"message", "message_id", "from"};
static char *_author_keys_nousrnm[5] = {"id", "is_bot", "first_name", "language_code"};
static char *_private_chat_keys_nousrnm[4] = {"id", "first_name", "type"};
static char *_publicgroup_chat_keys[5] = {"id", "title", "username", "type"};
static char *_privategroup_chat_keys[4] = {"id", "title", "type"};

static char const *baseURL = "https://api.telegram.org/bot";

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
static void write_message_author(cJSON **, struct updates *, int);
static void write_source_publicgroupchat(cJSON **, struct updates *, int, int);
static void write_source_groupchat(cJSON **, struct updates *);
static void _write(char **, cJSON *, cJSON **, int, int);

//-----------------------------------------------------------

static cJSON *getitem(const cJSON * const object, const char * const string)
{
    return cJSON_GetObjectItemCaseSensitive(object, string);
}

//-----------------------------------------------------------

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == 0) {
        printf("not enough memory (realloc returned 0)\n");
        return 0;
    }
    
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

//-----------------------------------------------------------

char *call(char const *w, char const *parameters)
{
    gcurl=curl_easy_init();

    char _request_buffer[1024];
	char errorno[1024];
    struct curl_slist *headers=0;
    struct MemoryStruct chunk;
 
    chunk.memory=(char *)malloc(1);  /* will be grown as needed by the realloc above */ 
    chunk.size=0;    /* no data at this point */ 
    
    if(gcurl) {
		curl_easy_setopt(gcurl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(gcurl, CURLOPT_SSL_VERIFYPEER, 0L);

        curl_easy_setopt(gcurl, CURLOPT_FOLLOWLOCATION, 1L);
		//curl_easy_setopt(gcurl, CURLOPT_POST, 1);//Mio


        if(parameters != 0) curl_easy_setopt(gcurl, CURLOPT_POSTFIELDS, parameters);

        curl_easy_setopt(gcurl, CURLOPT_WRITEFUNCTION, &WriteMemoryCallback);
        curl_easy_setopt(gcurl, CURLOPT_WRITEDATA, &chunk);
		//curl_easy_setopt(gcurl, CURLOPT_POSTFIELDSIZE, strlen(parameters));//Mio
        
        sprintf(_request_buffer, "%s%s/%s", baseURL, _bot->token, w);

		//Mio
		OutputDebugStringA(_request_buffer);
		OutputDebugStringA(parameters);

        curl_easy_setopt(gcurl, CURLOPT_URL, _request_buffer); 
        res = curl_easy_perform(gcurl);



		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));

			sprintf(errorno, "Mio: Error No: %d", res);
			OutputDebugStringA(errorno);
		}
        
        curl_easy_cleanup(gcurl);
    }
    return chunk.memory;
}

//-----------------------------------------------------------

void setup_bot(const char *token) 
{
    _bot = (bot *)malloc(sizeof(struct bot));
    
    _bot->token = token;
}

//-----------------------------------------------------------

char *compile_post_parameters(struct parameters *params)
{
    char *buf = (char *)malloc(1024);

    *buf = 0;
    int i=0;
    while((params[i].actualvalue) && (params[i].keyword) && (params[i].usable == 1)) {
        if(*(buf) == 0)
            sprintf(buf, "%s=%s", params[i].keyword, params[i].actualvalue);
        else
            sprintf(buf, "%s&%s=%s", buf, params[i].keyword, params[i].actualvalue);

        i++;
    }
    return buf;
}

//-----------------------------------------------------------

void make_param(struct parameters *params, char const *keyword, char const *value, int index)
{
    params[index].keyword=(unsigned char *)keyword;
    params[index].actualvalue=(unsigned char *)value;
    params[index].usable=1;
}

//-----------------------------------------------------------

void get_updates(struct updates *update) 
{
    struct parameters *param = (parameters *)malloc(3*sizeof(struct parameters));

    char offset[100];
    _last_update++;
    
    sprintf(offset, "%d", _last_update);
    make_param(param, "offset", offset, 0);
    make_param(param, "timeout", "10", 1);

    char const *params = compile_post_parameters(param);
    char *response = call("getUpdates", params);

    cJSON *jsondata = cJSON_Parse(response);
    cJSON *success=getitem(jsondata, "ok");
    cJSON *result=getitem(jsondata, "result");
    cJSON *from_section=0;
    cJSON *chat_section=0;
    cJSON *update_id=0;
    cJSON *message=0;
    cJSON *title=0;
    cJSON *tmp=0;

    param=0;
    free(param);
    if(success->valueint == 1) {
        cJSON *results=getitem(jsondata, _update_keys[1]);
        cJSON_ArrayForEach(result, results) {

            update_id = getitem(result, "update_id");
            _last_update = update_id->valueint;
            message=getitem(result, *_message_keys);
            cJSON *_tmp_message_keys[10];

            for(int i = 0; i < sizeof(_message_keys)/sizeof(*_message_keys); i++) {
                if(_message_keys[i] == (char *)"message") 
                    _tmp_message_keys[i] = getitem(message, _message_keys[i+1]);
                    /* printf("skipped\n");*/  
                else
                    _tmp_message_keys[i] = getitem(message, _message_keys[i]);
            }

            update->result.message.id = _tmp_message_keys[0]->valueint;

            from_section = _tmp_message_keys[2];
            chat_section = getitem(message, "chat");

            title = getitem(chat_section, "title");

            if(!title) {
                tmp = getitem(chat_section, "username");

				if (tmp) {
					_write(_author_keys_nousrnm, from_section, _tmp_message_keys,
						sizeof(_author_keys_nousrnm) / sizeof(*_author_keys_nousrnm),
						1
						);
				}
				else {
					_write(_author_keys_nousrnm, from_section, _tmp_message_keys,
						sizeof(_author_keys_nousrnm) / sizeof(*_author_keys_nousrnm),
						0
						);
				}

                if(tmp) write_message_author(_tmp_message_keys,update,1);
                else    write_message_author(_tmp_message_keys,update,0);
                                
				if (tmp) {
					_write(_private_chat_keys_nousrnm, chat_section, _tmp_message_keys,
						sizeof(_private_chat_keys_nousrnm) / sizeof(*_private_chat_keys_nousrnm),
						1
						);
				}
				else {
					_write(_private_chat_keys_nousrnm, chat_section, _tmp_message_keys,
						sizeof(_private_chat_keys_nousrnm) / sizeof(*_private_chat_keys_nousrnm),
						0
						);
				}

                /* Don't use valuelong here.
                    If the message is from a private chat, the identifier from chat section will be user's id,
                    and simple integer can handle it. 
                */
                if(tmp) write_source_publicgroupchat(_tmp_message_keys,update,1,1);
                else    write_source_publicgroupchat(_tmp_message_keys,update,1,0);
                

            }
            else {
                tmp = getitem(chat_section, "username");
                if(tmp) {        
                    _write(_publicgroup_chat_keys, chat_section, _tmp_message_keys, 
                        sizeof(_publicgroup_chat_keys)/sizeof(*_publicgroup_chat_keys),
                        0
                    );

                    if(tmp) write_source_publicgroupchat(_tmp_message_keys,update,0,1);
                    else    write_source_publicgroupchat(_tmp_message_keys,update,1,0);     
                }
                else {                   
                    _write(_privategroup_chat_keys, chat_section, _tmp_message_keys, 
                        sizeof(_privategroup_chat_keys)/sizeof(*_privategroup_chat_keys),
                        0
                    );
                    write_source_groupchat(_tmp_message_keys,update);
                }

                _write(_author_keys_nousrnm, from_section, _tmp_message_keys, 
                    sizeof(_author_keys_nousrnm)/sizeof(*_author_keys_nousrnm),
                    0
                );

                write_message_author(_tmp_message_keys,update,0);
            }
            
            cJSON *date = getitem(message, "date");
            cJSON *text = getitem(message, "text");

            if(text)
                update->result.message.text = text->valuestring;
            if(date) 
                update->result.message.date = date->valueint;
        }
    }
    else {
        char *err = cJSON_Print(jsondata);
        printf("Error: %s\n", err);
        return;
    }
}

//-----------------------------------------------------------

void send_message(long chat_id, char const *text) 
{
    struct parameters *param = (parameters *)malloc(2*sizeof(struct parameters));

    char chat[250];
    sprintf(chat, "%ld", chat_id);
    make_param(param, "chat_id", chat, 0);
    make_param(param, "text", text, 1);
	make_param(param, "parse_mode", "HTML", 2);

    char const *params = compile_post_parameters(param);
    call("sendMessage", params);

}

//-----------------------------------------------------------

static void _write(char **dict, cJSON *src, cJSON **dest, int indexc, int user)
{
    if(user==1) {
        if(dict[1] == (char *)"is_bot") {
           // indexc = sizeof(dict)/sizeof(*dict);
            char *lastkeyword = dict[indexc-2];
            dict[indexc-2] = "username";
            dict[indexc-1] = lastkeyword;
        }
        else if(dict[1] == (char *)"first_name") {
            //int indexc = sizeof(dict)/sizeof(*dict);
            char *lastkeyword = dict[indexc-2];
            dict[indexc-2] = "username";
            dict[indexc-1] = lastkeyword;
        }
    }
    for(int i=0;dict[i]!=0;i++)
        dest[i] = getitem(src,dict[i]);
}

//-----------------------------------------------------------

static void write_message_author(cJSON **src, struct updates *update, int username) 
{

    update->result.message.from.id = src[0]->valueint;
    update->result.message.from.is_bot = src[1]->valueint;
    update->result.message.from.firstname = src[2]->valuestring;

    if(username==1) {
        update->result.message.from.username = src[3]->valuestring;
        update->result.message.from.language_code = src[4]->valuestring;
    }
    if(username==0){
        update->result.message.from.language_code = src[3]->valuestring;
    }
}

//-----------------------------------------------------------

static void write_source_publicgroupchat(cJSON **src, struct updates *update, int user, int username)
{
    if(user == 0) 
        update->result.message.chat.id = src[0]->valuelong;
    if(user == 1)
        update->result.message.chat.id = src[0]->valueint;

    update->result.message.chat.firstname = src[1]->valuestring;
    if(username == 1) {
        update->result.message.chat.username = src[2]->valuestring;
        update->result.message.chat.type = src[3]->valuestring;
    }
    else {
        update->result.message.chat.type = src[2]->valuestring;
    }
}

//-----------------------------------------------------------

static void write_source_groupchat(cJSON **src, struct updates *update)
{
    update->result.message.chat.id = src[0]->valuelong;
    update->result.message.chat.title = src[1]->valuestring;
    update->result.message.chat.type = src[2]->valuestring;
}
