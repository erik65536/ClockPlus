#include <cstdio>
#include <array>

#include "ClockContainer.h"

namespace ClockPlus {

ClockContainer::ClockContainer(
    const std::string& name_time,
    const std::string& name_date,
    const std::string& name_message) {
  set_has_window(false);
  set_redraw_on_allocate(false);
  set_vexpand(true);
  set_hexpand(true);

  m_time.set_name(name_time);
  m_time.set_parent(*this);
  m_date.set_name(name_date);
  m_date.set_parent(*this);
  m_message.set_vexpand(true);
  m_message.set_name(name_message);
  m_message.set_parent(*this);
}

ClockContainer::~ClockContainer() {
  m_date.unparent();
  m_time.unparent();
  m_message.unparent();
}

void ClockContainer::set_date_time(const ClockPlus::DateTimeProvider& date_time) {
  std::array<char, 256> buf;

  std::snprintf(
      buf.data(),
      buf.size(),
      "%d:%02d",
      date_time.getHour12h(),
      date_time.getMinute());
  m_time.set_label(buf.data());

  std::snprintf(
      buf.data(),
      buf.size(),
      "%s, %s %d, %02d", date_time.getDayOfWeekShort(),
      date_time.getMonthShort(),
      date_time.getDayOfMonth(),
      date_time.getYearTwoDigit());
  m_date.set_label(buf.data());
}

void ClockContainer::set_message(const std::string& message) {
  m_message.set_label(message);

  if(message.empty()) {
    m_message.hide();
  } else {
    m_message.show();
  }
}

Gtk::SizeRequestMode ClockContainer::get_request_mode_vfunc() const {
  return Gtk::SIZE_REQUEST_CONSTANT_SIZE;
}

void ClockContainer::get_preferred_width_vfunc(
    int& minimum_width,
    int& natural_width) const {
  Gtk::Container::get_preferred_width_vfunc(minimum_width, natural_width);
}

void ClockContainer::get_preferred_height_vfunc(
    int& minimum_height,
    int& natural_height) const {
  Gtk::Container::get_preferred_height_vfunc(minimum_height, natural_height);
}

void ClockContainer::get_preferred_width_for_height_vfunc(
    int height,
    int& minimum_width,
    int& natural_width) const {
  Gtk::Container::get_preferred_width_for_height_vfunc(
      height,
      minimum_width,
      natural_width);
}

void ClockContainer::get_preferred_height_for_width_vfunc(
    int width,
    int& minimum_height,
    int& natural_height) const {
  Gtk::Container::get_preferred_height_for_width_vfunc(
      width,
      minimum_height,
      natural_height);
}

void ClockContainer::on_size_allocate(Gtk::Allocation& allocation) {
  //Gtk::Container::on_size_allocate(allocation);
  set_allocation(allocation);

  if(m_message.is_empty()) {
    m_message.hide();
  } else {
    m_message.show();
  }

  int border = static_cast<int>(get_border_width());
  int width = allocation.get_width();
  int height = allocation.get_height();
  int child_width = width - border - border;

  int minimum_height = 0;
  int natural_height = 0;

  m_date.get_preferred_height(minimum_height, natural_height);
  int date_height = natural_height;

  m_time.get_preferred_height(minimum_height, natural_height);
  int time_height = natural_height;

  int msg_height = 0;
  if(m_message.is_visible()) {
    m_message.get_preferred_height_for_width(
        child_width,
        minimum_height,
        natural_height);
    msg_height = natural_height;
  }

  Gtk::Allocation allo_date;
  allo_date.set_width(child_width);
  allo_date.set_x(border);
  allo_date.set_height(date_height);
  allo_date.set_y(height-date_height-border);
  m_date.size_allocate(allo_date);

  if(m_message.is_visible()) {
    Gtk::Allocation allo_msg;
    allo_msg.set_width(child_width);
    allo_msg.set_x(border);
    allo_msg.set_height(msg_height);
    allo_msg.set_y(border);
    m_message.size_allocate(allo_msg);
  }

  Gtk::Allocation allo_time;
  allo_time.set_width(child_width);
  allo_time.set_x(border);
  allo_time.set_height(time_height);
  //allo_time.set_y(std::min(
  //    (msg_height+height-date_height-time_height)/2,
  //    height-date_height-border-time_height));
  int msg_date_middle = (msg_height+height-date_height-time_height)/2;
  int time_bottom = height-border-
      static_cast<int>((date_height+time_height)*TIME_POS_FACTOR);
  allo_time.set_y(std::min(time_bottom, msg_date_middle));
  m_time.size_allocate(allo_time);
}

void ClockContainer::forall_vfunc(
    gboolean include_internals,
    GtkCallback callback,
    gpointer callback_data) {
  (void)include_internals;
  callback(m_time.gobj(), callback_data);
  callback(m_date.gobj(), callback_data);
  callback(m_message.gobj(), callback_data);
}

void ClockContainer::on_add(Gtk::Widget* widget) {
  widget->set_parent(*this);
}

void ClockContainer::on_remove(Gtk::Widget* widget) {
  widget->unparent();
}

GType ClockContainer::child_type_vfunc() const {
  return G_TYPE_NONE;
}

}