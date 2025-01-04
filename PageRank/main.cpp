#include "Page.h"
#include "Web.h"
#include <format>
#include <iostream>

int main() {
  auto homepage = std::make_shared<Page>("My Home Page");
  auto utexas = std::make_shared<Page>("University Home Page");
  homepage->add_link(utexas);
  auto searchpage = std::make_shared<Page>("google");
  homepage->add_link(searchpage);

  for (int i = 0; i < 20; i++) {
    homepage->add_link(std::make_shared<Page>(std::format("Page {}", i)));
  }

  std::cout << homepage->as_string() << '\n';

  for (int iclick = 0; iclick < 20; iclick++) {
    auto newpage = homepage->random_click();
    std::cout << "To: " << newpage->as_string() << '\n';
  }

  int netsize = 10;
  int avg_links = 3;
  size_t steps_per_walk = 10;
  size_t walks_per_page = 1000;
  Web internet(netsize);

  internet.create_random_links(avg_links);
  std::cout << "Generated web" << '\n';
  auto start_page = internet.get_page(0);
  internet.simulate_walks(steps_per_walk, walks_per_page);

  return 0;
}
