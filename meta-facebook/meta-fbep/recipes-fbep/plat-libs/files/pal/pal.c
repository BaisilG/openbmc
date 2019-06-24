/*
 *
 * Copyright 2015-present Facebook. All Rights Reserved.
 *
 * This file contains code to support IPMI2.0 Specificaton available @
 * http://www.intel.com/content/www/us/en/servers/ipmi/ipmi-specifications.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include "pal.h"

const char pal_fru_list[] = "all, fru";
const char pal_server_list[] = "";
size_t pal_pwm_cnt = 4;
size_t pal_tach_cnt = 8;
const char pal_pwm_list[] = "0..3";
const char pal_tach_list[] = "0..7";

#define LARGEST_DEVICE_NAME 120
#define HWMON_DIR "/sys/class/hwmon/hwmon0"
#define PWM_UNIT_MAX 255

static int
read_device(const char *device, int *value) {
  FILE *fp;
  int rc;

  fp = fopen(device, "r");
  if (!fp) {
    int err = errno;
#ifdef DEBUG
    syslog(LOG_INFO, "failed to open device %s", device);
#endif
    return err;
  }

  rc = fscanf(fp, "%d", value);
  fclose(fp);
  if (rc != 1) {
#ifdef DEBUG
    syslog(LOG_INFO, "failed to read device %s", device);
#endif
    return ENOENT;
  } else {
    return 0;
  }
}

static int
write_device(const char *device, const char *value) {
  FILE *fp;
  int rc;

  fp = fopen(device, "w");
  if (!fp) {
    int err = errno;
#ifdef DEBUG
    syslog(LOG_INFO, "failed to open device for write %s", device);
#endif
    return err;
  }

  rc = fputs(value, fp);
  fclose(fp);

  if (rc < 0) {
#ifdef DEBUG
    syslog(LOG_INFO, "failed to write device %s", device);
#endif
    return ENOENT;
  } else {
    return 0;
  }
}

static int
read_fan_value(const int fan, const char *device, int *value) {
  char device_name[LARGEST_DEVICE_NAME];
  char full_name[LARGEST_DEVICE_NAME];

  snprintf(device_name, LARGEST_DEVICE_NAME, device, fan);
  snprintf(full_name, LARGEST_DEVICE_NAME, "%s/%s", HWMON_DIR, device_name);
  return read_device(full_name, value);
}

static int
write_fan_value(const int fan, const char *device, const int value) {
  char full_name[LARGEST_DEVICE_NAME];
  char device_name[LARGEST_DEVICE_NAME];
  char output_value[LARGEST_DEVICE_NAME];

  snprintf(device_name, LARGEST_DEVICE_NAME, device, fan);
  snprintf(full_name, LARGEST_DEVICE_NAME, "%s/%s", HWMON_DIR, device_name);
  snprintf(output_value, LARGEST_DEVICE_NAME, "%d", value);
  return write_device(full_name, output_value);
}

int
pal_set_fan_speed(uint8_t fan, uint8_t pwm) {
  int unit;
  int ret;

  if (fan >= pal_pwm_cnt) {
    syslog(LOG_INFO, "pal_set_fan_speed: fan number is invalid - %d", fan);
    return -1;
  }

  unit = pwm * PWM_UNIT_MAX / 100;

  ret = write_fan_value(fan/2+1, "pwm%d", unit);
  if (ret < 0) {
    syslog(LOG_INFO, "set_fan_speed: write_fan_value failed");
    return -1;
  }

  return 0;
}

int
pal_get_fan_speed(uint8_t fan, int *rpm) {
  if (fan >= pal_tach_cnt) {
    syslog(LOG_INFO, "get_fan_speed: invalid fan#:%d", fan);
    return -1;
  }

  return read_fan_value(fan+1, "fan%d_input", rpm);
}

int
pal_get_fan_name(uint8_t num, char *name) {
  if (num >= pal_tach_cnt) {
    syslog(LOG_INFO, "get_fan_name: invalid fan#:%d", num);
    return -1;
  }

  sprintf(name, "Fan %d", num);
  return 0;
}

int
pal_get_pwm_value(uint8_t fan_num, uint8_t *value) {
  int val = 0;

  if (fan_num >= pal_pwm_cnt) {
    syslog(LOG_INFO, "pal_get_pwm_value: fan number is invalid - %d", fan_num);
    return -1;
  }

  if (read_fan_value(fan_num/2+1, "pwm%d", &val)) {
    syslog(LOG_INFO, "pal_get_pwm_value: read pwm%d failed", fan_num);
    return -1;
  }

  *value = 100 * val / PWM_UNIT_MAX;

  return 0;
}

int
pal_channel_to_bus(int channel)
{
  switch (channel) {
    case 0:
      return 14; // USB (LCD Debug Board)
    case 1:
      return 1; // MB#1
    case 2:
      return 2; // MB#2
    case 3:
      return 3; // MB#3
    case 4:
      return 4; // MB#4
  }

  // Debug purpose, map to real bus number
  if (channel & 0x80) {
    return (channel & 0x7f);
  }

  return channel;
}

int
pal_get_fruid_path(uint8_t fru, char *path) {

  sprintf(path, FRU_BIN);
  return 0;
}

int
pal_get_fruid_eeprom_path(uint8_t fru, char *path) {

  sprintf(path, FRU_EEPROM);
  return 0;
}

int
pal_get_fru_id(char *str, uint8_t *fru) {
  if (!strcmp(str, "all")) {
    *fru = FRU_ALL;
  } else if (!strcmp(str, "fru")) {
    *fru = FRU_BASE;
  } else {
    syslog(LOG_WARNING, "pal_get_fru_id: Wrong fru#%s", str);
    return -1;
  }

  return 0;
}

int
pal_get_fruid_name(uint8_t fru, char *name) {

  sprintf(name, "Base Board");
  return 0;
}

int
pal_is_fru_ready(uint8_t fru, uint8_t *status) {

  *status = 1;
  return 0;
}
