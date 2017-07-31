#pragma once
#include "Common.h"
#include <gtkmm/widget.h>

namespace ClockPlus {

class OutlinedLabel : public Gtk::Widget {

private:
  class Color {

  private:
    constexpr static double DARK_R = 0.0;
    constexpr static double DARK_G = 0.0;
    constexpr static double DARK_B = 0.0;
    constexpr static double LIGHT_R = 1.0;
    constexpr static double LIGHT_G = 1.0;
    constexpr static double LIGHT_B = 1.0;

  public:
    Color() = default;
    Color(double r, double g, double b) noexcept
    : m_r{r}, m_g{g}, m_b{b} {}
    double getRed() const noexcept {
      return m_r;
    }
    double getGreen() const noexcept {
      return m_g;
    }
    double getBlue() const noexcept {
      return m_b;
    }
    void set(const Gdk::RGBA& color) noexcept {
      m_r = color.get_red();
      m_g = color.get_green();
      m_b = color.get_blue();
    }
    Color getOutline() const noexcept {
      auto lum = std::sqrt(0.209 * m_r * m_r +
          0.587 * m_g * m_g +
          0.144 * m_b * m_b);
      return lum <= 0.5 ?
          Color(LIGHT_R, LIGHT_G, LIGHT_B) :
          Color(DARK_R, DARK_G, DARK_B);
    }

  private:
    double m_r = 0.0;
    double m_g = 0.0;
    double m_b = 0.0;

  };

private:
  static constexpr czstring OBJECT = "outlinedlabel";
  static constexpr double STROKE_FACTOR = 1.0/30.0;
  static constexpr double STROKE_MIN = 1.5;

public:
  OutlinedLabel();
  virtual ~OutlinedLabel() = default;
  void set_label(const std::string& text);
  bool is_empty() const;

protected:
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
  void on_realize() override;
  void on_unrealize() override;
  void on_map() override;
  void on_unmap() override;
  void on_style_updated() override;
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
  Glib::RefPtr<Pango::Layout> m_layout;
  Color m_fill;
  Color m_stroke;
  double m_stroke_width;

};

}