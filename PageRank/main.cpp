#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <format>

class Page {
  private:
    std::string title;
    std::vector<std::shared_ptr<Page>> links;

  public:

    Page(const std::string& title) : title(title) {}

    std::string as_string() const {
      std::string result = std::format("Page title: {}\n", title);
      for(const auto& link : links){
        result += " - " + link->title + '\n';
      }
      if(links.empty()){
        result += "No Links\n";
      }
      return result;
    }

    void add_link(const std::shared_ptr<Page>& page){
      links.push_back(page);
    }

    std::shared_ptr<Page> click(size_t index) const {
      if(index < links.size()){
        return links[index];
      } else {
        return nullptr;
      }
    }

    std::shared_ptr<Page> random_click() const {
      if (links.empty()) {
          std::cerr << "No links available to click.\n";
          return nullptr;
      }
      size_t random_index = std::rand() % links.size();
      return links[random_index];
    }

};

int main(){

  auto homepage = std::make_shared<Page>("My Home Page");
  auto utexas = std::make_shared<Page>("University Home Page");
  homepage->add_link(utexas);
  auto searchpage = std::make_shared<Page>("google");
  homepage->add_link(searchpage);

  for(int i = 0; i < 20; i++){
    homepage->add_link(std::make_shared<Page>(std::format("Page {}", i)));
  }

  std::cout << homepage->as_string() << '\n';

  for(int iclick = 0; iclick < 20; iclick++){
    auto newpage = homepage->random_click();
    std::cout << "To: " << newpage->as_string() << '\n';
  }

  return 0;

}