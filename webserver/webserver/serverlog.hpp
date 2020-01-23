//
//  serverlog.hpp
//  webserver
//
//  Created by dong da kuan on 2020/1/22.
//  Copyright © 2020年 dong da kuan. All rights reserved.
//

#ifndef serverlog_hpp
#define serverlog_hpp

#include <syslog.h>
#define IDENT "WEBSERVER"

#define LOG_MAXSIZE 2048

#define LOGI(...) \
do{\
char _LOGI_BUF_[LOG_MAXSIZE] = {0}; \
openlog(IDENT, LOG_ODELAY, LOG_LOCAL0); \
sprintf(_LOGI_BUF_,__VA_ARGS__); \
syslog(LOG_INFO, "[%s]: %s",__func__,_LOGI_BUF_); \
closelog();\
}while(0)

#define LOGD(...) \
do{\
char _LOGD_BUF_[LOG_MAXSIZE] = {0}; \
openlog(IDENT, LOG_ODELAY, LOG_LOCAL0); \
sprintf(_LOGD_BUF_,__VA_ARGS__); \
syslog(LOG_DEBUG, "[%s]: %s",__func__,_LOGD_BUF_); \
closelog(); \
}while(0)

#define LOGW(...) \
do{\
char _LOGW_BUF_[LOG_MAXSIZE]  = {0}; \
openlog(IDENT, LOG_ODELAY, LOG_LOCAL0); \
sprintf(_LOGW_BUF_,__VA_ARGS__); \
syslog(LOG_WARNING, "[%s]: %s",__func__,_LOGW_BUF_); \
closelog(); \
}while(0)

#define LOGE(...) \
do{\
char _LOGE_BUF_[LOG_MAXSIZE] = {0}; \
openlog(IDENT, LOG_ODELAY, LOG_LOCAL0); \
sprintf(_LOGE_BUF_,__VA_ARGS__); \
syslog(LOG_ERR, "[%s]: %s",__func__,_LOGE_BUF_); \
closelog(); \
}while(0)

#define LOGMERGE(...) \
do{\
char _LOGEMERGE_BUF_[LOG_MAXSIZE] = {0}; \
openlog(IDENT, LOG_ODELAY, LOG_LOCAL0); \
sprintf(_LOGEMERGE_BUF_,__VA_ARGS__); \
syslog(LOG_EMERG, "[%s]: %s",__func__,_LOGEMERGE_BUF_); \
closelog(); \
}while(0)

#endif /* serverlog_hpp */
