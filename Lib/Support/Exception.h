#ifndef _LIB_SUPPORT_EXCEPTION_H
#define _LIB_SUPPORT_EXCEPTION_H


struct Exception : public std::exception {
  Exception(std::string ss) : s(ss) {}
  ~Exception() throw () {}

  const char *what() const throw() { return s.c_str(); }

private:
  std::string s;
};

#endif  // _LIB_SUPPORT_EXCEPTION_H
