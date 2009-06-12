[CCode(cheader_filename = "gtop_glue.h")]
namespace AllTray {
	[CCode(cname = "alltray_get_ppid_for")]
	public int get_ppid_for(int pid);

	[CCode(cname = "alltray_get_processes_in_pgrp")]
	public void get_pids_in_pgid(int pgid, out int[] procs);
}