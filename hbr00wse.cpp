/**
* file hbrowse_00.cpp
*  @brief browse headered csv data
*/

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <unistd.h>
// #include <thread>
#include <jthread.hpp>

#include <nana/deploy.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/categorize.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/filesystem/filesystem_ext.hpp>

using std::cerr;
using std::string;
using constr = const string;
using strvec = std::vector<std::string>;
using constrvec = const strvec;
// using std::thread;
using std::jthread;

struct Hbrowse_00; 
using Hbrowse_00p = Hbrowse_00*;

struct HmodelBase {
  constr name_{""};
  constrvec cols_;
  Hbrowse_00p hb_{nullptr};
  HmodelBase(constr& name, constrvec&& cols) : name_(name), cols_(std::move(cols)) {}
  virtual ~HmodelBase() {}
  constrvec& cols() const { return cols_; }
};

class Hbrowse_00 :public nana::form
{
	nana::place	           place_ {*this};
	nana::listbox	       list_  {*this};
	nana::menubar          menu_  {*this};
	nana::panel<false>     view_  {*this},
	                       status_{*this};
	nana::toolbar          tools_ {*this},
	                       nav_   {*this};

	std::string            div_    =
		R"(
				vertical <weight=23 menu>
                         <weight=23 tools>
    					 < vertical <list> |<weight=23 nav>>  
                         <weight=23 status>  
           )";
  HmodelBase& hm_;

public:
  Hbrowse_00(HmodelBase& hm, nana::rectangle r = nana::rectangle{ nana::point{50,10}, nana::size{900,600} }) : form{r}, hm_(hm) {
	place_.div(div_.c_str());
	place_["menu"]  << menu_ ;
	place_["tools"] << tools_ ;
	place_["list"]  << list_ ;
	place_["nav"]   << nav_ ;
	place_["view"]  << view_ ;
	place_["status"]<< status_ ;
	place_.collocate();

	menu_.push_back("File");
	menu_.push_back("Help");

	for (auto& col : hm_.cols())
	  list_.append_header(col, 110);
	show();
	nana::exec();
  }
};

struct Hmodel : public HmodelBase {
  Hmodel(constr& name, constrvec&& cols) : HmodelBase(name, std::move(cols)) {}
  ~Hmodel() {}
  void run() { hb_ = new Hbrowse_00(*this); }
};

int main()
{
  Hmodel hm("hm00", {"col0", "col1", "col2", "col3"}), hm1("hm01", {"1col0", "1col1", "1col2", "1col3", "1col4"});;
  cerr << "constructed\n";
  jthread thr(&Hmodel::run, std::ref(hm)), thr1(&Hmodel::run, std::ref(hm1));
  cerr << "threaded\n";
  sleep(3); thr.join(); thr1.join();
  cerr << "joined\n";
}
