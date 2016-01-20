# xlfparser-c
[![Build Status](https://travis-ci.org/ajiwo/xmdsclient.svg?branch=master)](https://travis-ci.org/ajiwo/xmdsclient)

[xmds](http://xibo.org.uk/manual-tempel/en/xmds.html) (Xibo Media Distribution Service) client.

## Usage example.
see [example](example/main.c)

## Dependencies
* [curl](http://curl.haxx.se)
* [libxml2](http://www.xmlsoft.org)

## CMS Compatibility
Tested with xibo-cms-1.7.4, xmds version 4, on Ubuntu 14.04 Host.

## Build and Install
* clone
```shell
    git clone https://github.com/ajiwo/xmdsclient
```
* build
```shell
    cd xmdsclient
    mkdir build
    cd build
    cmake ..
    make
```
* install
```shell
    sudo make install
```
