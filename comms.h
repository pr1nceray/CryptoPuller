#include <bits/types/FILE.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <jsoncpp/json/json.h>

#include "string.h"
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>

#include <string>
#include <iostream>
#include <fstream>

using namespace std;


class curl_exception : public exception{
    public:
    string msg = "Error in Interface";
    curl_exception(){

    }

    curl_exception(const string &err_in): msg(err_in){
    }
    curl_exception(const char * err_in): msg(err_in){
    }
    string what(){
        return msg;
    }
    private:

};

    size_t write_file_class(void* data, size_t sz, size_t nmemb, string * userp){
        size_t r_sz = sz * nmemb;
        (*userp) = (string)((char *)data); 
        return r_sz;
    }


class COM{
    public:
    /*
    
    Initializes curl, and sets up options.
    Header data is set_header(), while options are in set_opts()
    File handling is done in init_file().
    
    */
    COM(string & url, string & file_name_in, string & api_key_in){
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if(!curl){ 
            curl_exception init_err("Error during curl initialization; curl not initialized");
            throw init_err;
            return;
        }
        set_up_opts(url);
        //should switch to member initialization list.
        base_url = url;
        file_name = file_name_in;
        api_key = api_key_in;
    }

    ~COM(){
        curl_easy_cleanup(curl);
    }


    //message with sandbox api key.
    void send_test_msg(){
        string next_url = base_url + "/v1/cryptocurrency/listings/latest";
        curl_easy_setopt(curl, CURLOPT_URL, next_url.c_str()); 

        curl_slist * list = nullptr;
        set_test_header(list);
        CURLcode code = curl_easy_perform(curl);

        if(code != CURLE_OK){
            cout << "Error getting info.";
            curl_slist_free_all(list);
            return;
        }
        curl_slist_free_all(list);

        parse_json(buffer);
        buffer.clear();

        ofstream file = init_file(file_name);
        file << val.toStyledString();
        file.close();
    }   

    //grabs xmr
    void send_msg(){
        curl_slist * list = nullptr;
        set_xmr(list);
        CURLcode code = curl_easy_perform(curl);

        if(code != CURLE_OK){
            cout << "Error getting info.";
            curl_slist_free_all(list);
            return;
        }
        parse_json(buffer);
        buffer.clear();

        ofstream file = init_file(file_name);
        file << val.toStyledString();
        file.close();
        //zero out and free slist after sending msg.
        curl_slist_free_all(list);
    }

    private:
    //curl vars
    string base_url;
    CURL * curl = nullptr;
    string api_key;

    //variables for json reading
    string buffer = "";
    Json::Reader read;
    Json::Value val;

    //variables for files
    int file_name_offset = 0;
    string file_name;


    void parse_json(string & buffer){
        bool is_parsed = read.parse(buffer,val);
        if(!is_parsed){
            throw curl_exception("Unable to turn get req into a string");
        }
    }

    void set_test_header(curl_slist * list){ //needs more header data,.
        //need better error handling if append fails!
        string api_key_temp = "X-CMC_PRO_API_KEY: " + api_key;
        list = curl_slist_append(list, "Accept: application/json");
        list = curl_slist_append(list, api_key_temp.c_str());

        if(list == nullptr){
            throw curl_exception("Error during setting our key");
            return;
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER,list);
    }

    void set_xmr(curl_slist * list){
        list = curl_slist_append(list, "Accept: application/json");
        list = curl_slist_append(list, "Accept-Encoding: deflate, gzip ");
        string next_url = base_url + "/v2/cryptocurrency/quotes/latest";
        next_url += "/items?id=1";//get bitcoin got now!
        curl_easy_setopt(curl, CURLOPT_URL,next_url.c_str()); 
    }   

    void set_up_opts(string & url){
        curl_easy_setopt(curl,CURLOPT_VERBOSE,true);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,&buffer);  
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_file_class);  
    }

    ofstream init_file(string & file_name){
        string file_name_offseted = file_name + std::to_string(file_name_offset) + ".json";      

        ofstream file;
        file.open(file_name_offseted);

        if(!file.is_open()){
            string msg = "could not write to file : ";
            curl_exception file_err(msg.append(file_name));
            throw  curl_exception(msg.append(file_name));
        }

        file_name_offset++;
        //this will overflow after it gets large enough, but i will deal with this when i get to it!

        return file;
    }


};