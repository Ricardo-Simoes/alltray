/*
 * GPL Notice:
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Library General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *
 * Name:
 *
 *    alltray
 *
 *
 * Copyright:
 * 
 *    Jochen Baier, 2004 (email@Jochen-Baier.de)
 *
 *
 * Based on:
 *
 *    steal-xwin.c (acano@systec.com)
 *    kdocker (Girish Ramakrishnan)
 *    libwnck (Havoc Pennington <hp@redhat.com>)
 *    dsimple.c ("The Open Group")
 *    .....lot more, THANX !!!
 *    
*/

#ifndef __GTRAY_H__
#define __GTRAY_H__

#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gdk/gdk.h>
#include <gdk-pixbuf-xlib/gdk-pixbuf-xlib.h>
#include <gtk/gtkplug.h>
#include <stdlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/WinUtil.h>
#include <string.h>
#include <X11/cursorfont.h>

Atom net_client_list_stacking;
Atom wm_name_atom;
Atom wm_icon_atom;
Atom net_wm_icon;
Atom net_close_window;
Atom net_current_desktop;

Atom selection_atom;
Atom manager_atom;
Atom system_tray_opcode_atom;

typedef struct _win_struct {

  GtkWidget *parent_window;
  gint parent_window_x;
  gint parent_window_y;

  GtkWidget *sock;

  Window xlib;
  Window xlib_our;
  GdkWindow *gdk;
  GdkWindow *gdk_our;
  
  gchar *wm_res_class;
  gchar *wm_res_name;
  gchar *wm_name;

  GPid pid;

  GdkPixbuf *icon;

  Window manager_window;
  GdkWindow *manager_window_gdk;
  GtkWidget *tray_window;
  GtkTooltips *tooltip;
  
  gchar *title;
    
  gint last_desktop;
      
  gboolean ignore_splash_window;
    
} win_struct;

GdkFilterReturn child_window_filter (GdkXEvent *xevent, GdkEvent *event, gpointer user_data);

extern gboolean debug;

#endif