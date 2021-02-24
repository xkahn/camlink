# Camlink

A small library to force removal of invalid colorspaces exported by the Elgato Cam Link 4K.

## Table of Contents
- [Setup](#setup)
  - [Prerequisites](#prerequisites)
  - [Building](#building)
  - [Installing](#installing)
  - [Uninstalling](#uninstalling)
- [Usage](#usage)
- [Examples](#examples)
  - [Video4Linux2 CTL Application](#video4linux2-ctl-application)
  - [Zoom](#zoom)
  - [Flatpak Zoom](#flatpak-zoom)
  - [Firefox](#firefox)
  - [Univesal](#universal)
  - [Getting this to work in a desktop environment](#getting-this-to-work-in-a-desktop-environment)
- [Troubleshooting](#troubleshooting)
- [Alternatives](#alternatives)

## Setup

### Prerequisites

You will need `gcc` and `make` to build.

[Top][toc]

### Building

Run `mkdir build && make`

[Top][toc]

### Installing

Simply run `sudo make install`. The `camlink.so` file will be copied to the path `/usr/local/lib/camlink/camlink.so`

[Top][toc]

### Uninstalling

Run `sudo make uninstall`.

[Top][toc]

## Usage

This library is run as a `LD_PRELOAD` to other applications. It intercepts all `ioctl` calls
and determines if the call is intended to query the colorspaces available on a camlink.

If so, it rejects all return values that are NOT `YUYV 4:2:2`. This should make colors work
correctly on the Camlink in Linux.

[Top][toc]

## Examples

### Video4Linux2 CTL application:

```
$ LD_PRELOAD=/usr/local/lib/camlink/camlink.so v4l2-ctl -d /dev/video2 --list-formats-ext
```

[Top][toc]

### Zoom
```
$ LD_PRELOAD=/usr/local/lib/camlink/camlink.so zoom
```

[Top][toc]

### Flatpak Zoom
```
$ flatpak override us.zoom.Zoom --filesystem=/usr/local/lib/camlink/camlink.so
$ flatpak run --env=LD_PRELOAD=/usr/local/lib/camlink/camlink.so us.zoom.Zoom
```

[Top][toc]

### Firefox
```
$ LD_PRELOAD=/usr/local/lib/camlink/camlink.so firefox
```

[Top][toc]

### Universal
```
export LD_PRELOAD=/usr/local/lib/camlink/camlink.so
```

[Top][toc]

### Getting this to work in a desktop environment

Say you use Google Chrome. You click on the icon in your desktop environment, and it locates the .desktop file. (located @ `/usr/share/applications/google-chrome.desktop` on my machine)

Inside that file is an `Exec=` line that specifies how to the browser should be started. On my machine, the line is:

```
Exec=/usr/bin/google-chrome-stable %U
```

You need to edit that line, and you need that line to STAY edited even if you update Google Chrome. The solution is to make a local copy of this file in your home directory:

```
$ cp /usr/share/applications/google-chrome.desktop ~/.local/share/applications/
```

Then edit the new file @ `~/.local/share/applications/google-chrome.desktop` and change the exec line to include the LD\_PRELOAD call. Something like this:

```
Exec=env LD_PRELOAD=/usr/local/lib/camlink/camlink.so /usr/bin/google-chrome-stable %U 
```

[Top][toc]

## Troubleshooting
If running the browser on the command line DOES NOT WORK -- even after fully quitting it first:

Then the issue is different; likely the `LD_PRELOAD` path is incorrect. The examples assume that you have installed the camlink library. If you have not, instead use the path where your camlink.so file can be found. For example: `LD_PRELOAD=/home/yourname/path/to/camlink/sources/build/camlink.so`

[Top][toc]

## This is a Cam Link 4K bug. What does Elgato support say?

I contacted Elgato about the issue and got this reply:

> Thank you for contacting Elgato Technical support.
> 
> I understand you are in need of some information regarding running a Cam Link 4K on a Linux computer. I will be happy to help!
> 
> As it turns out, Linux OS is not supported. Elgato products were designed to work with Windows and Mac only. 
> 
> I understand this may not be the information your were expecting to hear and for that I apologize.

So they will not be helpful. That doesn't mean you shouldn't contact them about the issue! Manufacterers need to know how their product is being used.

[Top][toc]

## Alternatives

There are several other possible fixes for the color issue for the Cam Link 4K:

* [Hack the firmware to stop returning invalid colorspaces](https://assortedhackery.com/patching-cam-link-to-play-nicer-on-linux/)
* [Use a dummy video source and ffmpeg to convert the color to something different](https://www.reddit.com/r/linuxhardware/comments/dzqmvq/did_anyone_tried_an_elgato_cam_link_4k_on_gnulinux/fjdsx96/)
* Build an exception into the v4l2 driver for the Camlink to ignore the extra colorspaces

[Top][toc]

[toc]: #table-of-contents
