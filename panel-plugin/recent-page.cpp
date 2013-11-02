/*
 * Copyright (C) 2013 Graeme Gott <graeme@gottcode.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "recent-page.h"

#include "launcher.h"
#include "launcher-model.h"
#include "launcher-view.h"
#include "settings.h"
#include "window.h"

using namespace WhiskerMenu;

//-----------------------------------------------------------------------------

RecentPage::RecentPage(Window* window) :
	ListPage(wm_settings->recent, window),
	m_max_items(10)
{
	// Prevent going over max
	if (wm_settings->recent.size() > m_max_items)
	{
		wm_settings->recent.erase(wm_settings->recent.begin() + m_max_items, wm_settings->recent.end());
	}
}

//-----------------------------------------------------------------------------

void RecentPage::add(Launcher* launcher)
{
	if (!launcher)
	{
		return;
	}

	// Remove item if already in list
	remove(launcher);

	// Prepend to list of items
	LauncherModel model(GTK_LIST_STORE(get_view()->get_model()));
	model.prepend_item(launcher);

	// Prevent going over max
	while (wm_settings->recent.size() > m_max_items)
	{
		model.remove_last_item();
	}
}

//-----------------------------------------------------------------------------

void RecentPage::extend_context_menu(GtkWidget* menu)
{
	GtkWidget* menuitem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

	menuitem = gtk_image_menu_item_new_with_label(_("Clear Recently Used"));
	GtkWidget* image = gtk_image_new_from_stock(GTK_STOCK_CLEAR, GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menuitem), image);
	g_signal_connect(menuitem, "activate", G_CALLBACK(RecentPage::clear_menu_slot), this);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
}

//-----------------------------------------------------------------------------

void RecentPage::clear_menu()
{
	LauncherModel model(GTK_LIST_STORE(get_view()->get_model()));
	for (size_t i = 0, count = wm_settings->recent.size(); i < count; ++i)
	{
		model.remove_first_item();
	}
	get_window()->set_modified();
}

//-----------------------------------------------------------------------------