#ifndef URL_H
#define URL_H

#include <string>

class URL {
private:
  std::string root_url;
  // TODO: File to store urls

public:
  std::string cur_url;
  URL(std::string address) : cur_url(address){};

  std::string get_root_address() { return root_url; }
  void set_root_address(std::string address) {root_url = address;}
  // int record_url();
};

#endif // URL_H