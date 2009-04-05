/*
 * x11_glue.c - X11 "glue" code for AllTray
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Fondation
 */
#include <glib/glib.h>
#include <X11/Xlib.h>

static Atom _net_wm_pid;

gulong
alltray_get_x11_window_from_pid(GPid pid, Display *d) {
  Window root_win = XDefaultRootWindow(d);
  _net_wm_pid = XInternAtom(d, "_NET_WM_PID", True);

  if(_net_wm_pid == None) {
    // Error: this atom isn't defined?!  What are they _doing_?
    g_critical("AllTray can't find _NET_WM_PID!");
    return(0);
  }

  return(find_window(pid, d, root_win));
}

static gulong
find_window(GPid pid, Display *d, Window w) {
  Atom type;
  int format;
  int status;
  gulong nItems;
  gulong bytesAfter;
  guchar *prop_ret = null;

  status = XGetWindowProperty(d, w, 0, 1, False, XA_CARDINAL,
			      &type, &format, &nItems, &bytesAfter, &prop_ret);
  if(status == Success) {
    if(prop_ret != 0) {
      GPid returned_pid = *((unsigned long *)prop_ret);
      if(returned_pid == pid) return(w);
    }
  } else {
    return(0);
  }

  /*
   * If we are still here, we need to continue searching...
   */
  Window root, parent;
  Window *child;
  guint child_count;

  if(XQueryTree(d, w, &root, &parent, &child, &child_count) != 0) {
    for(guint i = 0; i < child_count; i++) {
      guint retval = search(child[i]);
      if(retval != 0) return(retval);
    }
  }

  return(0);
}
