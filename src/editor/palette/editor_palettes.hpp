/*
   Copyright (C) 2003 - 2013 by David White <dave@whitevine.net>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef EDITOR_PALETTES_H_INCLUDED
#define EDITOR_PALETTES_H_INCLUDED

#include "../editor_display.hpp"
#include "common_palette.hpp"
#include "tristate_button.hpp"

namespace editor {

template<class Item>
class editor_palette : public common_palette {

public:

	editor_palette(editor_display &gui, const config& /*cfg*/
			, size_t item_size, size_t item_width, mouse_action** active_mouse_action)
		: groups_()
		, gui_(gui)
		, item_size_(item_size)
		, item_width_(item_width)
//TODO avoid magic number
		, item_space_(item_size + 5)
		, palette_y_(0)
		, palette_x_(0)
		, group_map_()
		, item_map_()
		, nitems_(0)
		, nmax_items_(0)
		, items_start_(0)
		, non_core_items_()
		, active_group_()
		, selected_fg_item_()
		, selected_bg_item_()
		, active_mouse_action_(active_mouse_action)
		, buttons_()
	{
	};

	virtual handler_vector handler_members();

	void set_start_item(size_t index) { items_start_ = index; };

	size_t start_num(void) { return items_start_; };

	/** Menu expanding for palette group list */
	void expand_palette_groups_menu(std::vector< std::pair<std::string, std::string> >& items);

	void set_group(size_t index);

	const std::vector<item_group>& get_groups() const { return groups_; };

	virtual void draw(bool force);

	void next_group() {
		set_group( (active_group_index() +1) % (groups_.size() -1) );
	};
	void prev_group() {
		set_group( (active_group_index() -1) % (groups_.size() -1) );
	};

	/**
	 * Update the size of this widget.
	 *
	 * Use if the size_specs have changed.
	 */
	void adjust_size(const SDL_Rect& target);

	virtual bool scroll_up();
	virtual bool can_scroll_up();
	virtual bool scroll_down();
	virtual bool can_scroll_down();

	virtual const config active_group_report();

	void swap();

	/** Return the currently selected foreground/background item. */
	const Item& selected_fg_item() const { return item_map_.find(selected_fg_item_)->second; };
	const Item& selected_bg_item() const { return item_map_.find(selected_bg_item_)->second; };

private:

	/** TODO */
	size_t active_group_index();

	/** Scroll the editor-palette to the top. */
	void scroll_top();

	/** Scroll the editor-palette to the bottom. */
	void scroll_bottom();

	virtual void draw_item(const Item& item, surface& item_image, std::stringstream& tooltip) = 0;

	virtual const std::string& get_id(const Item& item) = 0;

	/** Setup the internal data structure. */
	virtual void setup(const config& cfg) = 0;

	virtual const std::string& active_group_id() {return active_group_;};

	/** Return the number of items in the palette. */
	size_t num_items();

	//void draw_old(bool);

protected:
	/**
	 * Sets a group active id
	 *
	 * This can result in no visible
	 * selected items.
	 */
	virtual void set_group(const std::string& id);
	const std::vector<std::string>& active_group() { return group_map_[active_group_]; };

	/** Select a foreground item. */
	virtual void select_fg_item(const std::string& item_id);
	virtual void select_bg_item(const std::string& item_id);

	/**
	 * The editor_groups as defined in editor-groups.cfg.
	 *
	 * Note the user must make sure the id's here are the same as the
	 * editor_group in terrain.cfg.
	 */
	std::vector<item_group> groups_;

	editor_display &gui_;

	int item_size_;
	int item_width_;
	int item_space_;

private:
	unsigned int palette_y_;
	unsigned int palette_x_;

protected:
	std::map<std::string, std::vector<std::string> > group_map_;

	std::map<std::string, Item> item_map_;
	size_t nitems_, nmax_items_, items_start_;
    std::set<std::string> non_core_items_;

private:
	std::string active_group_;
	std::string selected_fg_item_;
	std::string selected_bg_item_;

    mouse_action** active_mouse_action_;
    std::vector<gui::tristate_button> buttons_;
};


} //end namespace editor
#endif // EDITOR_PALETTES_H_INCLUDED

