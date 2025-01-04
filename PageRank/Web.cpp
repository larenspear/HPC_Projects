#include "Web.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>

Web::Web(size_t num_pages) {
  for (size_t i = 0; i < num_pages; ++i) {
    pages.push_back(std::make_shared<Page>("Page " + std::to_string(i + 1), i));
  }
}

void Web::create_random_links(size_t avg_links) {
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

void Web::random_walk(std::shared_ptr<Page> start_page, size_t steps) const {
  if (!start_page) {
    return;
  }
  auto current_page = start_page;
  for (size_t i = 0; i < steps; ++i) {
    if (!current_page) {
      break;
    }
    std::cout << "Step " << i + 1 << ": " << current_page->as_string() << "\n";
    current_page = current_page->random_click();
  }
}

void Web::display() const {
  for (const auto &page : pages) {
    std::cout << page->as_string() << "\n";
  }
}

void Web::simulate_walks(size_t steps_per_walk, size_t walks_per_page) const {
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

const std::vector<std::shared_ptr<Page>> &Web::all_pages() const {
  return pages;
}

std::shared_ptr<Page> Web::get_page(size_t index) const {
  if (index < pages.size()) {
    return pages[index];
  } else {
    return nullptr;
  }
}
