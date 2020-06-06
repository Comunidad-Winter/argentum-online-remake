#include "common_sdl_exception.h"

SdlException::SdlException(const char* fmt, ...) noexcept {
    //For SERCOM
    myerrno = 0;
    va_list args;
    va_start(args, fmt);
    
    int s = vsnprintf(msg_error, BUF_LEN, fmt, args);
    va_end(args);
    
    //To avoid to print "Success" in the message error
    if (myerrno)
      strncpy(msg_error+s, strerror(myerrno), BUF_LEN-s);
    
    msg_error[BUF_LEN-1] = 0;
}

SdlException::~SdlException() {}

const char* SdlException::what() const noexcept {
  return msg_error;
}
