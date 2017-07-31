#include "Common.h"
#include <gtkmm.h>

namespace ClockPlus {

const std::string& getConfigDir() {
  static const std::string config = Glib::build_filename(
      Glib::get_user_config_dir(),
      APPLICATION_NAME);
  return config;
}

}
