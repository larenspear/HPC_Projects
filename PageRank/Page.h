#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include <string>
#include <string_view>
#include <vector>

class Page {
private:
  std::string title;
  size_t id;
  std::vector<std::shared_ptr<Page>> links;

public:
  Page(std::string_view title, size_t id = 0);
  size_t global_id() const;
  std::string as_string() const;
  void add_link(const std::shared_ptr<Page> &page);
  std::shared_ptr<Page> random_click() const;
};

#endif
