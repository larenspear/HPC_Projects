#ifndef WEB_H
#define WEB_H

#include "Page.h"
#include <memory>
#include <vector>

class Web {
private:
  std::vector<std::shared_ptr<Page>> pages;

public:
  Web(size_t num_pages);
  void create_random_links(size_t avg_links);
  void random_walk(std::shared_ptr<Page> start_page, size_t steps) const;
  void display() const;
  void simulate_walks(size_t steps_per_walk, size_t walks_per_page) const;
  const std::vector<std::shared_ptr<Page>> &all_pages() const;
  std::shared_ptr<Page> get_page(size_t index) const;
};

#endif
