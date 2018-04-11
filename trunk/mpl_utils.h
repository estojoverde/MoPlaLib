#ifndef MPL_UTILS_H
#define MPL_UTILS_H


#include <cmath>

#define S_aux(x) #x
#define S_aux_(x) S_aux(x)
#define S__LINE__ S_aux_(__LINE__)

#define MPL_ERROR_MESSAGE2(output, fmt, ...) fprintf(output, ("\n[ERROR][%s:%d( %s )]\n---  " fmt "  ---\n\n"), __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__) //error message

#define MPL_ERROR_MESSAGE(fmt, ...) MPL_ERROR_MESSAGE2(stderr, fmt, ##__VA_ARGS__)


#define MPL_WARNING_MESSAGE2(output, fmt, ...) fprintf(output, ("\n[WARNING][%s:%d( %s )]\n---  " fmt "  ---\n\n"), __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__) //error message

#define MPL_WARNING_MESSAGE(fmt, ...) MPL_WARNING_MESSAGE2(stderr, fmt, ##__VA_ARGS__)


#define MPL_INFORMATION_MESSAGE2(output, fmt, ...) fprintf(output, ("\n[INFORMATION][%s:%d( %s )]\n---  " fmt "  ---\n\n"), __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__) //error message

#define MPL_INFORMATION_MESSAGE(fmt, ...) MPL_INFORMATION_MESSAGE2(stdout, fmt, ##__VA_ARGS__)



#define MPL_SGN_ERROR_MSG_START emit signal_errorMessage(QString("\n----\n[ERROR] ") +
#define MPL_SGN_ERROR_MSG_END + "\n  File: " +__FILE__ + ":" + S__LINE__ + "\n  Func: " + __PRETTY_FUNCTION__ + "\n----\n" );

#define MPL_SGN_LOG_MSG_START emit signal_logMessage(QString("") +
#define MPL_SGN_LOG_MSG_END );




#define MPL_MAX2(x1, x2) ( (x1)>(x2) ? (x1) : (x2) ) //maximum value between two elements - 1 comparison

#define MPL_MAX3(x1, x2, x3) ( MPL_MAX2( (MPL_MAX2(x1, x2))  , (x3) ) ) //maximum value between three elements - 2 comparisons


#define MPL_MAX_MODULE2(x1, x2) ( fabs(x1)>fabs(x2) ? fabs(x1) : fabs(x2) ) //maximum module-value between two elements - 1 comparison

#define MPL_MAX_MODULE3(x1, x2, x3) ( MPL_MAX_MODULE2( (MPL_MAX_MODULE2((x1), (x2))), (x3) ) ) //maximum module-value between three elements - 2 comparisons



#define MPL_ROUND_INT(x) (int)(x+0.5) //round to nearest


#define MPL_ReturnCode_OK 0

#define MPL_ReturnCode_GeneralError (-404)





#endif // MPL_UTILS_H

