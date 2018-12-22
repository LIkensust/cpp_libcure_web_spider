#include <iostream>
#include <curl/curl.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
using namespace std;
    /*
    1. 调用curl_global_init()初始化libcurl
    2. 调用 curl_easy_init()函数得到 easy interface型指针
    3. 调用curl_easy_setopt设置传输选项
    4. 根据curl_easy_setopt设置的传输选项，实现回调函数以完成用户特定任务
    5. 调用curl_easy_perform（）函数完成传输任务
    6. 调用curl_easy_cleanup（）释放内存*/

//ibcurl主要提供了两种发送http请求的方式，分别是Easy interface方式和multi interface方式
////前者是采用阻塞的方式发送单条数据，后者采用组合的方式可以一次性发送多条数据
// 
// //Easy interface方式最基本的用法如下
// //1、在主线程中调用curl_global_init(CURL_GLOBAL_ALL)初始化
// //2、调用curl_easy_init获取一个句柄；
// //3、调用curl_easy_setopt函数设置此次传输的一些基本参数，如url地址、http头、cookie信息、发送超时时间等，其中，CURLOPT_URL是必设的选项；
// //4、设置完成后，调用curl_easy_perform函数发送数据；
// //5、数据发送完毕后，调用curl_easy_cleanup清空句柄；
// //6、调用curl_global_cleanup()做清理工作。

class HttpCurl
{
public:
    HttpCurl()
        :conn(nullptr)
    {}

    ~HttpCurl()
    {
        curl_easy_cleanup(conn);
    }

    bool HttpCurlInit(string& context)
    {
        CURLcode code;
        code = curl_global_init(CURL_GLOBAL_ALL);
        if(CURLE_OK != code)
        {
            cout<<"global初始化失败"<<endl;
            return false;
        }
        conn = curl_easy_init();
        if(NULL == conn)
        {
            cout<<"CURL初始化失败"<<endl;
            return false;
        }
        code = curl_easy_setopt(conn,CURLOPT_WRITEFUNCTION,HttpCurl::Write);
        if(CURLE_OK != code)
        {
            cout<<"setopt::option失败"<<endl;
            return false;
        }
        code = curl_easy_setopt(conn,CURLOPT_WRITEDATA,&context);
        if(CURLE_OK != code)
        {
            cout<<"setopt::context失败"<<endl;
        }
        return true;
    }

    bool SetUrl(string& url)
    {
        CURLcode code;
        code = curl_easy_setopt(conn,CURLOPT_URL,url.c_str());
        if(CURLE_OK != code)
        {
            cout<<"url设置失败"<<endl;
            return false;
        }
        return true;
    }

    bool GetHttpResponse()
    {
        CURLcode code;
        code = curl_easy_perform(conn);
        if(CURLE_OK != code)
        {
            cout<<"response获取失败"<<endl;
            return false;
        }
        return true;
    }

    static size_t Write(void* data,size_t size,size_t nmemb,string& context);
private:
    CURL* conn;
};

size_t HttpCurl::Write(void* data,size_t size,size_t nmemb,string& context)
{
    long sizes = size*nmemb;
    string temp((char*)data,sizes);
    context += temp;
    return sizes;
}
