#ifndef URL_H
#define URL_H

#include <string>

class URL {
private:
  std::string root_address;
  // TODO: File to store urls

public:
  std::string cur_address;
  URL(std::string address) : cur_address(address){};

  std::string get_root_address() { return root_address; }
  void set_root_address(std::string address) {root_address = address;}
  // int record_url();
};

#endif // URL_H