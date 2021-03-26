/*
#             (C) 2020 Benjamin Kahn <xkahn@zoned.net>
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdarg.h>
#include <string.h>
#include <dlfcn.h>
#include <linux/videodev2.h>

#ifndef RTLD_NEXT
#define RTLD_NEXT ((void *) -1l)
#endif

#define REAL_LIBC RTLD_NEXT

typedef int request_t;

typedef void (*sighandler_t)(int);

static int hook_fd = -1;

int ioctl (int fd, request_t request, ...){	
  
  static int (*func_ioctl) (int, request_t, void *) = NULL;
  va_list args;
  void *argp;
  int ri;
  
  /* Get the original definition of ioctl */
  if (! func_ioctl) {
    func_ioctl = (int (*) (int, request_t, void *)) dlsym (REAL_LIBC, "ioctl");
  }
  va_start (args, request);
  argp = va_arg (args, void *);
  va_end (args);
  
  if (fd != hook_fd) {

    /* We only care about device colorspace ioctls */
    if ((int) request == (int) VIDIOC_ENUM_FMT) {
      struct v4l2_capability cap;
      int ret;
      
      ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);

      /* and only if the device is a camlink */
      if (ret == 0 && strcmp ((char *) cap.card, "Cam Link 4K: Cam Link 4K") == 0) { 
	int index = ((struct v4l2_fmtdesc *) argp)->index;
	ri = func_ioctl (fd, request, argp);

	/* We only want the first colorspace answer, ignore all the others */
	while (ri != -1 && ((struct v4l2_fmtdesc *) argp)->index != 0) {
	  ((struct v4l2_fmtdesc *) argp)->index = ++index;
	  ri = func_ioctl (fd, request, argp);
	}
	
	return ri;
      }
    }
    ri = func_ioctl (fd, request, argp);
    return ri;
  }
  
  /* Capture the ioctl() calls */
  return func_ioctl (hook_fd, request, argp);
}
