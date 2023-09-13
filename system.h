#pragma once

#include <stdio.h>

static char client_buffer[256];

int init();
int deinit(); 

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
