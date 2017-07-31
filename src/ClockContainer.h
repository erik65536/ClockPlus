#pragma once
#include <gtkmm/container.h>
#include "DateTimeProvider.h"
#include "OutlinedLabel.h"

namespace ClockPlus {

class ClockContainer : public Gtk::Container {

private:
  static constexpr auto TIME_POS_FACTOR = 0.8f;

public:
  ClockContainer(
      const std::string& name_time,
      const std::string& name_date,
      const std::string& name_message);
  ~ClockContainer() override ;
  void set_date_time(const DateTimeProvider& date_time);
  void set_message(const std::string& message);
  Gtk::SizeRequestMode get_request_mode_vfunc() const override;
  void get_preferred_width_vfunc(
      int& minimum_width,
      int& natural_width) const override;
  void get_preferred_height_vfunc(
      int& minimum_height,
      int& natural_height) const override;
  void get_preferred_width_for_height_vfunc(
      int height,
      int& minimum_width,
      int& natural_width) const override;
  void get_preferred_height_for_width_vfunc(
      int width,
      int& minimum_height,
      int& natural_height) const override;
  void on_size_allocate(Gtk::Allocation& allocation) override;
  void forall_vfunc(
      gboolean include_internals,
      GtkCallback callback,
      gpointer callback_data) override;
  void on_add(Gtk::Widget* widget) override;
  void on_remove(Gtk::Widget* widget) override;
  GType child_type_vfunc() const override;



private:
  OutlinedLabel m_time;
  OutlinedLabel m_date;
  OutlinedLabel m_message;

};

}