#include "OutlinedLabel.h"

namespace ClockPlus {

OutlinedLabel::OutlinedLabel()
: Glib::ObjectBase(OBJECT),
  m_layout(Pango::Layout::create(get_pango_context())) {
  set_has_window(false);
}

void OutlinedLabel::set_label(const std::string& text) {
  m_layout->set_text(text);
  queue_draw();
}

bool OutlinedLabel::is_empty() const {
  return m_layout->get_text().empty();
}

Gtk::SizeRequestMode OutlinedLabel::get_request_mode_vfunc() const {
  return get_vexpand() ?
      Gtk::SIZE_REQUEST_HEIGHT_FOR_WIDTH :
      Gtk::SIZE_REQUEST_CONSTANT_SIZE;
}

void OutlinedLabel::get_preferred_width_vfunc(
    int& minimum_width,
    int& natural_width) const {
  m_layout->set_width(-1);
  m_layout->set_height(-1);

  int width;
  int height;
  m_layout->get_pixel_size(width, height);

  minimum_width = 0;
  natural_width = width;
}

void OutlinedLabel::get_preferred_height_vfunc(
    int& minimum_height,
    int& natural_height) const {
  m_layout->set_width(-1);
  m_layout->set_height(-1);

  int width;
  int height;
  m_layout->get_pixel_size(width, height);

  minimum_height = 0;
  natural_height = height;
}

void OutlinedLabel::get_preferred_width_for_height_vfunc(
    int height,
    int& minimum_width,
    int& natural_width) const {
  m_layout->set_width(-1);
  m_layout->set_height(height * PANGO_SCALE);

  int ignore;
  int width;
  m_layout->get_pixel_size(width, ignore);

  minimum_width = width;
  natural_width = width;
}

void OutlinedLabel::get_preferred_height_for_width_vfunc(
    int width,
    int& minimum_height,
    int& natural_height) const {
  m_layout->set_width(width * PANGO_SCALE);
  m_layout->set_height(-1);

  int ignore;
  int height;
  m_layout->get_pixel_size(ignore, height);

  minimum_height = height;
  natural_height = height;
}

void OutlinedLabel::on_size_allocate(Gtk::Allocation& allocation) {
  Gtk::Widget::on_size_allocate(allocation);
  m_layout->set_width(allocation.get_width() * PANGO_SCALE);
  m_layout->set_height(allocation.get_height() * PANGO_SCALE);
}

void OutlinedLabel::on_map() {
  Gtk::Widget::on_map();
}

void OutlinedLabel::on_unmap() {
  Gtk::Widget::on_unmap();
}

void OutlinedLabel::on_realize() {
  Gtk::Widget::on_realize();
}

void OutlinedLabel::on_unrealize() {
  Gtk::Widget::on_unrealize();
}

void OutlinedLabel::on_style_updated() {
  Gtk::Widget::on_style_updated();

  auto style = get_style_context();

  auto font = style->get_font(Gtk::STATE_FLAG_NORMAL);
  m_layout->set_font_description(font);
  m_layout->set_alignment(Pango::ALIGN_CENTER);

  m_stroke_width = font.get_size();
  if(!font.get_size_is_absolute()) {
    m_stroke_width /= Pango::SCALE;
  }
  m_stroke_width *= STROKE_FACTOR;
  m_stroke_width = std::max(m_stroke_width, STROKE_MIN);

  auto color = style->get_color(Gtk::STATE_FLAG_NORMAL);
  m_fill.set(color);
  m_stroke = m_fill.getOutline();
}

bool OutlinedLabel::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  m_layout->add_to_cairo_context(cr);

  cr->set_source_rgb(m_fill.getRed(), m_fill.getGreen(), m_fill.getBlue());
  cr->fill_preserve();

  cr->set_line_width(m_stroke_width);
  cr->set_source_rgb(
      m_stroke.getRed(),
      m_stroke.getGreen(),
      m_stroke.getBlue());
  cr->stroke();

  return true;
}

}