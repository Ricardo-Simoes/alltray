/*
 * linux_process_info.c - Functions to get process information on
 * GNU/Linux systems.
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU LGPL v3.0 as published by the Free Software Foundation
 */
static process_info_t *linux_get_process_info(pid_t pid);

static process_info_t *
linux_get_process_info(pid_t pid) {
  FILE *proc_stat = NULL;
  char *proc_stat_file_name = NULL;
  char *buffer = NULL;
  int char_count = 0;
  process_info_t *process_info = NULL;

  proc_stat_file_name = (char *)malloc(1024 * sizeof(char));
  if(proc_stat_file_name == NULL) {
    // Out of memory.
    fprintf(stderr, "AllTray has run out of memory and cannot continue.\n");
    abort();
  }
  proc_stat_file_name = memset(proc_stat_file_name, 0, 1024);

  char_count = sprintf(proc_stat_file_name, "/proc/%d/stat", pid);
  proc_stat = fopen(proc_stat_file_name, "r");
  if(proc_stat == NULL) {
    // Unable to open the file!
    fprintf(stderr, "Debug: Unable to open file %s\n", proc_stat_file_name);
    return(NULL);
  }

  buffer = get_nth_token(proc_stat, 4, ' ');

  process_info = (process_info_t *)malloc(1 * sizeof(process_info_t));
  if(process_info == NULL) {
    // Out of memory.
    fprintf(stderr, "AllTray has run out of memory and cannot continue.\n");
    abort();
  }
  process_info = memset(process_info, 0, sizeof(process_info_t));

  process_info->pid = pid;
  process_info->ppid = atoi(buffer);
  free(buffer);
  buffer = NULL;

  buffer = get_nth_token(proc_stat, 2, ' ');
  process_info->name = (char *)malloc(strlen(buffer) - 2);
  if(process_info->name == NULL) {
    // Out of memory.
    fprintf(stderr, "AllTray has run out of memory and cannot continue.\n");
    abort();
  }
  process_info->name = memset(process_info->name, 0, strlen(buffer) - 2);

  for(char_count = 1; char_count < strlen(buffer) - 1; char_count++) {
    strncat(process_info->name, &buffer[char_count], 1);
  }

  return(process_info);
}
