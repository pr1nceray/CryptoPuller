#include <cstddef>
#include <cstdio>
#include <curl/curl.h>
#include <curl/easy.h>
#include <jsoncpp/json/json.h>

#include "string.h"
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <new>
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
        *userp = ((char *)data); //set userp (buffer) to data
        return r_sz;
    }


class COM{
    public:

    COM(string & url, string & file_name){
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if(!curl){ //need to throw error here tbh.
            curl_exception init_err("Error during curl initialization; curl not initialized");
            throw init_err;
            return;
        }
        init_file(file_name);
        set_up_opts(url);
        set_header();
    }


    ~COM(){
        curl_easy_cleanup(curl);
    }

    void send_msg(){
        CURLcode code = curl_easy_perform(curl);
        if(code != CURLE_OK){
            file << "Error getting info.";
        }
        parse_json(buffer);
        buffer.clear();
    }   

    void set_header(){ //eeds more header data,.
        list = curl_slist_append(list, "Accept: application/json");
        list = curl_slist_append(list, "X-CMC_PRO_API_KEY: b54bcf4d-1bca-4e8e-9a24-22ff2c3d462c");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER,list);
    }

    private:
    CURL * curl = nullptr;
    ofstream file;
    string buffer = "";
    string json_unparsed;
    curl_slist * list = nullptr;
    Json::Reader read;



    void parse_json(string & buffer){
           Json::Value val;
            bool is_parsed = read.parse(buffer,val);
            if(!is_parsed){
                throw curl_exception("Unable to turn get req into a string");
                return;
            }
        std::cout << val.toStyledString();
    }


    void set_up_opts(string & url){
        curl_easy_setopt(curl,CURLOPT_VERBOSE,true);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,&buffer);  
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_file_class);  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
    }

    void init_file(string & file_name){
        file.open(file_name);

        if(!file.is_open()){
            string msg = "could not write to file : ";
            curl_exception file_err(msg.append(file_name));
            throw  curl_exception(msg.append(file_name));
        }
    }


};