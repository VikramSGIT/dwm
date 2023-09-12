#pragma once

#include <stdio.h>

static FILE* batcap_file;
static FILE* batstate_file;
static FILE* mem_file;

static const char* batcap = "/sys/class/power_supply/BAT1/capacity";
static const char* batstate = "/sys/class/power_supply/BAT1/status";
static const char* mem = "/proc/meminfo";
static const char* netstatus = "nmcli | awk \'/connected to/ {print $4}\'";
static const char* bright = "/home/dedshot/Projects/timesup/scripts/system_status.sh getbright";
static const char* incbright = "/home/dedshot/Projects/timesup/scripts/system_status.sh incbright";
static const char* decbright = "/home/dedshot/Projects/timesup/scripts/system_status.sh decbright";
static const char* vol = "/home/dedshot/Projects/timesup/scripts/system_status.sh getvol";
static const char* decvol = "/home/dedshot/Projects/timesup/scripts/system_status.sh decvol";
static const char* incvol = "/home/dedshot/Projects/timesup/scripts/system_status.sh incvol";
static const char* togglemute = "/home/dedshot/Projects/timesup/scripts/system_status.sh togglemute";
static const char* getmute = "/home/dedshot/Projects/timesup/scripts/system_status.sh getmute";

static const char* SERVER_IP = "127.0.0.1";
static const int PORT = 12345;

static char client_buffer[256];

void init();
void deinit(); 

void get_status();

int toggle_cb();
int get_cb();

int inc_vol();
int dec_vol();
int toggle_mute();
int get_mute();
int get_vol();

int inc_bright();
int dec_bright();
int get_bright();

int toggle_rr();
int get_rr();
