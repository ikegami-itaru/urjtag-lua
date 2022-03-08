# urjtag-lua

Urjta-lua is the fork of the urjtag, adding lua repl for urjtag command line prompt.

## build
It may difficult if your environment does not have pkg-config.
1. Install lua5.3 ( or later ) and lua5.3 ( or later ) development package.
1. Get lua-prompt from https://github.com/ikegami-itaru/luaprompt and build.
- Lua-prompt is fork from https://github.com/dpapavas/luaprompt with memory leak patch https://github.com/nefelim/luaprompt/commit/3079b72a350638159006483a4c154b3a628c9c32 adding luaprompt.pc for pkg-config.
3. Install lua-prompt.
1. Install some packages to build urjtag. For example in Ubuntu or debian,
```
sudo apt-get update -y
sudo apt-get install -y build-essential bison flex automake autoconf autopoint git gettext libusb-1.0-0 libusb-1.0-0-dev libreadline-dev autopoint libftdi1 libftdi1-dev python3-dev readline-common libreadline-dev
```
5. Get source for urjtag-lua.
1. Execute `autogen.sh`.
1. `./configure --enable-lua`  with your favorite option. `--enable-readline` is not need because readline is already linked with lua.
1. `make`


## environment
If you want to colorize repl, set environment variable `URJTAG_LUA_COLOR=1`
​
## lua commands for urjtag
### urj_init()
to initialize urjtag and lua, call urj_init()
```
> r = urj_init()
```
where r is the resource for manipurating urjtag.
### urj()
All of urjtag commands are executed through urj(). 
urj() has two argument, one is the resource for urjtag and another is the urjtag command line string.
```
> r = urj_init()
> rc, rc_str, msg, err_msg = urj(r, "cable ft2232 vid=xxx pid=xxx")
```
`rc` is the return code. 0 is success. Other codes are described in `urjtag/include/urjtag/error.h`.
`rc_str` is the error string. refer to `urjtag/src/global/log-error.c`.
`msg` is the message that original urjtag outputs into `stdout`.
`err_msg` is the message that original urjtag outputs into `stderr`.
​
​
## TODO
- test if urjtag multi-statement work.
- cleanup code.
- test for other platform.
