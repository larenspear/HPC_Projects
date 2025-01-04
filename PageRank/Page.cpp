#include "Page.h"
#include <algorithm>
#include <format>
#include <random>

Page::Page(std::string_view title, size_t id) : title(title), id(id) {}

size_t Page::global_id() const {
    return id;
}

std::string Page::as_string() const {
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

void Page::add_link(const std::shared_ptr<Page> &page) {
    if (page && page.get() != this &&
        std::find(links.begin(), links.end(), page) == links.end()) {
        links.push_back(page);
    }
}

std::shared_ptr<Page> Page::random_click() const {
    if (links.empty()) {
        return nullptr;
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, links.size() - 1);
    return links[dist(gen)];
}

