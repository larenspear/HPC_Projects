#include <algorithm>
#include <format>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <vector>

class Page {
private:
  std::string title{};
  size_t id{0};
  std::vector<std::shared_ptr<Page>> links{};

public:
  Page(std::string_view title, size_t id = 0) : title(title), id(id) {}

  size_t global_id() const { return id; }

  std::string as_string() const {
    std::string result = std::format("Page title: {}\n", title);
    if (links.empty()) {
      result += "No Links\n";
    } else {
      for (const auto &link : links) {
        result += std::format(" - {}\n", link->title);
      }
    }
    return result;
  }

  void add_link(const std::shared_ptr<Page> &page) {
    if (page && page.get() != this &&
        std::find(links.begin(), links.end(), page) == links.end()) {
      links.push_back(page);
    }
  }

  std::shared_ptr<Page> random_click() const {
    if (links.empty()) {
      return nullptr;
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, links.size() - 1);
    return links[dist(gen)];
  }
};

class Web {
private:
  std::vector<std::shared_ptr<Page>> pages;

public:
  Web(size_t num_pages) {
    for (size_t i = 0; i < num_pages; ++i) {
      pages.push_back(
          std::make_shared<Page>("Page " + std::to_string(i + 1), i));
    }
  }

  void create_random_links(size_t avg_links) {
    if (pages.empty()) {
      return;
    }
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<size_t> dist_links(0, 2 * avg_links);
    for (const auto &page : pages) {
      size_t num_links = dist_links(eng);
      std::shuffle(pages.begin(), pages.end(), eng);
      for (size_t j = 0; j < num_links; ++j) {
        if (pages[j] != page) {
          page->add_link(pages[j]);
        }
      }
    }
    for (size_t i = 0; i < pages.size(); ++i) {
      if (pages[i]->random_click() == nullptr) {
        size_t random_index = (i + 1) % pages.size();
        pages[i]->add_link(pages[random_index]);
      }
    }
  }

  void random_walk(std::shared_ptr<Page> start_page, size_t steps) const {
    if (!start_page) {
      return;
    }
    auto current_page = start_page;
    for (size_t i = 0; i < steps; ++i) {
      if (!current_page) {
        break;
      }
      std::cout << "Step " << i + 1 << ": " << current_page->as_string()
                << "\n";
      current_page = current_page->random_click();
    }
  }

  void display() const {
    for (const auto &page : pages) {
      std::cout << page->as_string() << "\n";
    }
  }

  void simulate_walks(size_t steps_per_walk, size_t walks_per_page) const {

    if (pages.empty()) {
      return;
    }

    std::vector<int> landing_counts(pages.size(), 0);

    for (const auto &start_page : pages) {
      for (size_t i = 0; i < walks_per_page; ++i) {
        auto current_page = start_page;
        for (size_t step = 0; step < steps_per_walk; ++step) {
          if (!current_page) {
            break;
          }
          current_page = current_page->random_click();
        }
        if (current_page) {
          landing_counts[current_page->global_id()]++;
        }
      }
    }

    std::cout << "\nLanding counts:\n";
    for (size_t i = 0; i < landing_counts.size(); ++i) {
      std::cout << "Page " << i << ": " << landing_counts[i] << " landings\n";
    }

    int total_landings =
        std::accumulate(landing_counts.begin(), landing_counts.end(), 0);
    std::cout << "\nNormalized Landing Probabilities:\n";
    for (size_t i = 0; i < landing_counts.size(); ++i) {
      std::cout << "Page " << i << ": "
                << static_cast<double>(landing_counts[i]) / total_landings
                << "\n";
    }
  }

  const std::vector<std::shared_ptr<Page>> &all_pages() const { return pages; }

  std::shared_ptr<Page> get_page(size_t index) const {
    if (index < pages.size()) {
      return pages[index];
    } else {
      return nullptr;
    }
  }
};

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