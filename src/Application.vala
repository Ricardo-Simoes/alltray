/*
 * Application.vala - Application-based window management for AllTray
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;

namespace AllTray {
	public errordomain ApplicationError {
		FAILED
	}

	public class Application : GLib.Object {
		private unowned List<Wnck.Window> _windows;
		private Wnck.Application? _wnckApp;
		private Gtk.StatusIcon _appIcon;
		// private Sexy.Tooltip _appIconTooltip;
		private Process _process;
		private bool _appVisible;

		public Wnck.Application wnck_app {
			get {
				return(_wnckApp);
			}
		}

		public Application(Process p) {
			_process = p;

			/*
			 * Register interest in learning of new applications on
			 * the display, and then we'll be able to perform the rest
			 * of the setup after we get notification of the
			 * application.
			 */
			Program.WnckScreen.application_opened += maybe_setup;
			Program.WnckScreen.application_closed += bye_wnck_app;
		}

		private void bye_wnck_app(Wnck.Screen scr, Wnck.Application app) {
			if(app == _wnckApp) {
				Debug.Notification.emit(Debug.Subsystem.Application,
										Debug.Level.Information,
										"WHOA - The app went away‽  "+
										"Looking for it to come back...");

				scr.window_opened -= maybe_update_window_count;
				scr.window_closed -= maybe_update_window_count;

				Program.WnckScreen.application_opened += maybe_setup;
			}
		}

		private void maybe_setup(Wnck.Screen scr, Wnck.Application app) {
			StringBuilder msg = new StringBuilder();
			msg.append_printf("maybe_setup called: %d == %d? %s",
							  app.get_pid(), (int)_process.get_pid(),
							  (app.get_pid() ==
							   (int)_process.get_pid()).to_string());

			Debug.Notification.emit(Debug.Subsystem.Application,
									Debug.Level.Information,
									msg.str);

			if(app.get_pid() != (int)_process.get_pid()) return;

			scr.application_opened -= maybe_setup;

			_wnckApp = app;
			_appVisible = true;
			_windows = _wnckApp.get_windows();

			scr.window_opened += maybe_update_window_count;
			scr.window_closed += maybe_update_window_count;

			if(_appIcon == null) create_icon();
			_appIcon.visible = true;

			// Force an update to catch circumstances where the app
			// disappears and reappears (e.g., The GIMP).
			update_icon_image(_wnckApp);
		}

		private void maybe_update_window_count(Wnck.Screen scr,
											   Wnck.Window win) {
			StringBuilder sb = new StringBuilder();
			int wincount = _wnckApp.get_n_windows();
			string plural = (wincount != 1 ? "s" : "");

			sb.append_printf("%s - %d window%s", _wnckApp.get_name(),
							 _wnckApp.get_n_windows(), plural);
			
			_appIcon.set_tooltip(sb.str);
		}

		private void create_icon() {
			_appIcon = new Gtk.StatusIcon.from_pixbuf(_wnckApp.get_icon());

			_appIcon.set_tooltip(_wnckApp.get_name());

			_appIcon.activate += on_icon_click;
			_wnckApp.icon_changed += update_icon_image;
			_wnckApp.name_changed += update_icon_name;
		}

		private void update_icon_image(Wnck.Application app) {
			unowned Gdk.Pixbuf new_icon = app.get_icon();

			_appIcon.set_from_pixbuf(new_icon);

			if(new_icon == null) {
				_appIcon.visible = false;
				return;
			}
		}

		private void update_icon_name(Wnck.Application app) {
			_appIcon.set_tooltip(app.get_name());
		}

		private void on_icon_click(Gtk.StatusIcon icon) {
			if(icon.blinking == true) icon.blinking = false;
			toggle_visibility();
		}

		private void toggle_visibility() {
			_appVisible = !_appVisible;

			StringBuilder msg = new StringBuilder();

			foreach(Wnck.Window w in _windows) {
				msg.truncate(0);
				msg.append_printf("Setting window 0x%08lx visibility to %s",
								  w.get_xid(), _appVisible.to_string());

				if(_appVisible) {
					TimeVal tv = TimeVal();
					tv.get_current_time();

					w.unminimize((uint32)tv.tv_sec);
					w.set_skip_tasklist(false);
				} else {
					w.minimize();
					w.set_skip_tasklist(true);
				}
				Debug.Notification.emit(Debug.Subsystem.Application,
										Debug.Level.Information,
										msg.str);
			}
		}
	}
}