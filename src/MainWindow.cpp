#include <random>
#include "MainWindow.h"

namespace ClockPlus {

using namespace std::string_literals;

const std::string& MainWindow::getDefaultCss() {
  static const std::string css =
      "#"s + NAME + "{"
      "transition-property:background-image,background-color;"
      "transition-duration:1s;"
      "background-size:cover;"
      "background-position:center center;"
      "}"
      "#"s + NAME_TIME + "{"
      "font-family:'Open Sans Semibold';"
      "font-size:250px;"
      "}"
      "#"s + NAME_DATE + "{"
      "font-family:'Open Sans Semibold';"
      "font-size:80px;"
      "}"
      "#"s + NAME_MESSAGE + "{"
      "font-family:'Open Sans Semibold';"
      "font-size:60px"
      "}"s;

  return css;
}

std::string MainWindow::getBaseCss() {
  try {
    std::string css = Glib::file_get_contents(
        Glib::build_filename(getConfigDir(), CSS_FILE));

    if(css.empty()) {
      throw Glib::FileError::TRYAGAIN;
    }
    return css;

  } catch(Glib::FileError& error) {
    (void)error;
    return getDefaultCss();
  }
}

MainWindow::MainWindow()
: m_css(Gtk::CssProvider::create()),
  m_backgrounds(&m_date_time) {
  set_title(TITLE);
  set_default_size(WIDTH, HEIGHT);
  set_resizable(false);
  set_name(NAME);
  //fullscreen();

  m_clock = std::make_unique<ClockContainer>(
      NAME_TIME,
      NAME_DATE,
      NAME_MESSAGE);
  m_clock->set_hexpand(true);
  m_clock->set_vexpand(true);
  m_clock->set_border_width(CLOCK_BORDER);
  m_clock->set_date_time(m_date_time);
  add(*m_clock);

  get_style_context()->add_provider_for_screen(
      get_screen(),
      m_css,
      0xffffffff);

  onCssChanged();
  onBackgroundsChanged();

  m_date_time.setDateHandler(std::bind(&MainWindow::onDateChanged, this));
  m_date_time.setMinuteHandler(std::bind(&MainWindow::onMinuteChanged, this));
  m_backgrounds.setCssHandler(std::bind(&MainWindow::onCssChanged, this));
  m_backgrounds.setBackgroundsHandler(
      std::bind(&MainWindow::onBackgroundsChanged, this));

  show_all_children();
}

void MainWindow::on_size_allocate(Gtk::Allocation& allocation) {
  Gtk::ApplicationWindow::on_size_allocate(allocation);
}

void MainWindow::onDateChanged() {
  m_backgrounds.onDateChanged();
}

void MainWindow::onMinuteChanged() {
  m_clock->set_date_time(m_date_time);

  auto period = m_backgrounds.getPeriod();
  auto minutes = m_date_time.getHour() * 60 + m_date_time.getMinute();

  if(minutes % period == 0) {
    removeClass(m_class.current());
    addClass(m_class.next());
  }
}

void MainWindow::onCssChanged() {
  std::string css = getBaseCss();
  css.append(m_backgrounds.getCss());
  m_css->load_from_data(css);
}

void MainWindow::onBackgroundsChanged() {
  removeClass(m_class.current());
  m_class.set(m_backgrounds.getBackgrounds());
  addClass(m_class.current());
}

void MainWindow::removeClass(const std::string* id) {
  if(id == nullptr) {
    return;
  }

  get_style_context()->remove_class(Background::classBackground(*id));
  m_clock->get_style_context()->remove_class(Background::classLabel(*id));
}

void MainWindow::addClass(const std::string* id) {
  if(id == nullptr) {
    return;
  }

  get_style_context()->add_class(Background::classBackground(*id));
  m_clock->get_style_context()->add_class(Background::classLabel(*id));
}

void MainWindow::RandomClass::set(const std::vector<std::string>& classes) {
  auto id_ptr = current();

  if(id_ptr == nullptr) {
    m_classes.assign(classes.begin(), classes.end());
    shuffle(id_ptr);
  } else {
    auto id = *id_ptr;
    m_classes.assign(classes.begin(), classes.end());
    shuffle(&id);
  }
}

const std::string* MainWindow::RandomClass::current() {
  if(m_classes.empty()) {
    return nullptr;
  }

  return &m_classes[m_order.back()];
}

const std::string* MainWindow::RandomClass::next() {
  if(m_classes.empty()) {
    return nullptr;
  }

  if(m_order.size() == 1) {
    auto id = current();
    shuffle(id);
  } else {
    m_order.pop_back();
  }

  return current();
}

void MainWindow::RandomClass::shuffle(const std::string* id) {
  m_order.clear();

  for(size_t i = 0; i < m_classes.size(); ++i) {
    m_order.push_back(i);
  }

  auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rand(seed);
  std::shuffle(m_order.begin(), m_order.end(), rand);

  if(id != nullptr && *current() == *id && m_order.size() > 1) {
    auto i = std::uniform_int_distribution<int>(0, static_cast<int>(
        m_order.size()-2))(rand);
    std::iter_swap(m_order.begin()+i, m_order.end()-1);
  }
}

}