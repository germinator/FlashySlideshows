#ifndef AMMSERVERLIB_H_INCLUDED
#define AMMSERVERLIB_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

enum TypesOfRequests
{
    NONE=0,
    HEAD,
    //Asks for the response identical to the one that would correspond to a GET request, but without the response body. This is useful for retrieving meta-information written in response headers, without having to transport the entire content.
    GET,
    //Requests a representation of the specified resource. Requests using GET should only retrieve data and should have no other effect. (This is also true of some other HTTP methods.)[1] The W3C has published guidance principles on this distinction, saying, "Web application design should be informed by the above principles, but also by the relevant limitations."[11] See safe methods below.
    POST,
    //Submits data to be processed (e.g., from an HTML form) to the identified resource. The data is included in the body of the request. This may result in the creation of a new resource or the updates of existing resources or both.
    PUT,
    //Uploads a representation of the specified resource.
    DELETE,
    //Deletes the specified resource.
    TRACE,
    //Echoes back the received request, so that a client can see what (if any) changes or additions have been made by intermediate servers.
    OPTIONS,
    //Returns the HTTP methods that the server supports for specified URL. This can be used to check the functionality of a web server by requesting '*' instead of a specific resource.
    CONNECT,
    //Converts the request connection to a transparent TCP/IP tunnel, usually to facilitate SSL-encrypted communication (HTTPS) through an unencrypted HTTP proxy.[12]
    PATCH,
    //Is used to apply partial modifications to a resource.[13]
    BAD
};


#define MAX_QUERY 512
#define MAX_RESOURCE 512
#define MAX_FILE_PATH 1024


struct HTTPRequest
{
   int  requestType; //See enum TypesOfRequests
   char resource[MAX_RESOURCE+1];
   char verified_local_resource[MAX_FILE_PATH+1];
   char GETquery[MAX_QUERY+1];
   char POSTquery[4*MAX_QUERY+1];
   unsigned char authorized;
   unsigned char keepalive;
   unsigned char supports_compression;

   //RANGE DATA
   unsigned long range_start;
   unsigned long range_end;

   /*! IMPORTANT update FIELDS_TO_CLEAR_FROM_HTTP_REQUEST when I add something here.. */
   char * ETag; //<-   *THIS SHOULD BE CLEARED AFTER USAGE*
   char * Cookie; //<-   *THIS SHOULD BE CLEARED AFTER USAGE*
   char * Host; //<-     *THIS SHOULD BE CLEARED AFTER USAGE*
   char * Referer; //<-  *THIS SHOULD BE CLEARED AFTER USAGE*
   char * UserAgent;//<- *THIS SHOULD BE CLEARED AFTER USAGE*
   char * ContentType; //<- for POST requests *THIS SHOULD BE CLEARED AFTER USAGE*
   unsigned long ContentLength; //<- for POST requests
   //Languages etc here..!
};
/*! IMPORTANT @@@ */
#define FIELDS_TO_CLEAR_FROM_HTTP_REQUEST 6
/*! IMPORTANT @@@*/



enum RHScenarios
{
   SAME_PAGE_FOR_ALL_CLIENTS = 0 ,
   DIFFERENT_PAGE_FOR_EACH_CLIENT
};


struct AmmServer_RH_Context
{

   unsigned int RH_Scenario;


   unsigned long MAX_content_size;
   unsigned long content_size;

   unsigned int last_callback;
   unsigned int callback_every_x_msec;
   char callback_cooldown;
   void * prepare_content_callback;

   char web_root_path[MAX_FILE_PATH];
   char resource_name[MAX_RESOURCE];

   char * content;

   char * GET_request;
   unsigned int GET_request_length;

   char * POST_request;
   unsigned int POST_request_length;
};


struct AmmServer_RequestOverride_Context
{
   char requestHeader[64]; //Initial request ( GET , HEAD , CONNECT )
   struct HTTPRequest * request;
   void * request_override_callback;
};


struct AmmServer_Instance_Settings
{
    //Configuration of instance..
    //------------------------------------------------
    //A ) Password protection..
    int PASSWORD_PROTECTION;
    char * USERNAME;
    char * PASSWORD;
    char * BASE64PASSWORD;
    //------------------------------------------------

    int BINDING_PORT;
};



struct AmmServer_Instance
{
    struct AmmServer_Instance_Settings settings;


    //Cache Items..
    unsigned long loaded_cache_items_Kbytes;
    unsigned int loaded_cache_items;
    void * cache; /*Actually struct cache_item * but declared as a void pointer here */

    int files_open;

    //Server state
    int serversock;
    int server_running;
    int pause_server;
    int stop_server;

    //Thread holders..
    int CLIENT_THREADS_STARTED;
    int CLIENT_THREADS_STOPPED;

    pthread_t server_thread_id;
    pthread_t * threads_pool;


    unsigned int prespawn_turn_to_serve,prespawn_jobs_started,prespawn_jobs_finished;
    void * prespawned_pool; //Actually struct PreSpawnedThread * but declared as a void pointer here
};




enum AmmServInfos
{
    AMMINF_ACTIVE_CLIENTS=0,
    AMMINF_ACTIVE_THREADS
};


enum AmmServSettings
{
    AMMSET_PASSWORD_PROTECTION=0,
    AMMSET_TEST
};


enum AmmServStrSettings
{
    AMMSET_USERNAME_STR=0,
    AMMSET_PASSWORD_STR,
    AMMSET_TESTSTR
};

char * AmmServer_Version();

struct AmmServer_Instance * AmmServer_Start(char * ip,unsigned int port,char * conf_file,char * web_root_path,char * templates_root_path);
int AmmServer_Stop(struct AmmServer_Instance * instance);
int AmmServer_Running(struct AmmServer_Instance * instance);


int AmmServer_AddRequestHandler(struct AmmServer_Instance * instance,struct AmmServer_RequestOverride_Context * context,char * request_type,void * callback);

int AmmServer_AddResourceHandler(struct AmmServer_Instance * instance,struct AmmServer_RH_Context * context, char * resource_name , char * web_root, unsigned int allocate_mem_bytes,unsigned int callback_every_x_msec,void * callback,unsigned int scenario);
int AmmServer_RemoveResourceHandler(struct AmmServer_Instance * instance,struct AmmServer_RH_Context * context,unsigned char free_mem);

int AmmServer_GetInfo(struct AmmServer_Instance * instance,unsigned int info_type);

int AmmServer_GetIntSettingValue(struct AmmServer_Instance * instance,unsigned int set_type);
int AmmServer_SetIntSettingValue(struct AmmServer_Instance * instance,unsigned int set_type,int set_value);

int AmmServer_POSTArg (struct AmmServer_Instance * instance,struct AmmServer_RH_Context * context,char * var_id_IN,char * var_value_OUT,unsigned int max_var_value_OUT);
int AmmServer_GETArg  (struct AmmServer_Instance * instance,struct AmmServer_RH_Context * context,char * var_id_IN,char * var_value_OUT,unsigned int max_var_value_OUT);
int AmmServer_FILES   (struct AmmServer_Instance * instance,struct AmmServer_RH_Context * context,char * var_id_IN,char * var_value_OUT,unsigned int max_var_value_OUT);

int _POST (struct AmmServer_Instance * instance,struct AmmServer_RH_Context * context,char * var_id_IN,char * var_value_OUT,unsigned int max_var_value_OUT);
int _GET  (struct AmmServer_Instance * instance,struct AmmServer_RH_Context * context,char * var_id_IN,char * var_value_OUT,unsigned int max_var_value_OUT);
int _FILES(struct AmmServer_Instance * instance,struct AmmServer_RH_Context * context,char * var_id_IN,char * var_value_OUT,unsigned int max_var_value_OUT);

int AmmServer_DoNOTCacheResourceHandler(struct AmmServer_Instance * instance,struct AmmServer_RH_Context * context);
int AmmServer_DoNOTCacheResource(struct AmmServer_Instance * instance,char * resource_name);

char * AmmServer_GetStrSettingValue(struct AmmServer_Instance * instance,unsigned int set_type);
int AmmServer_SetStrSettingValue(struct AmmServer_Instance * instance,unsigned int set_type,char * set_value);

struct AmmServer_Instance *  AmmServer_StartAdminInstance(char * ip,unsigned int port);

int AmmServer_SelfCheck(struct AmmServer_Instance * instance);

#ifdef __cplusplus
}
#endif

#endif // AMMSERVERLIB_H_INCLUDED
