# TP-Link Archer C20 v5 Hacking Notes
Everything I made porting OpenWRT to TP-Link Archer C20 v5.

## Disclaimer
I DO NOT work for TP-Link. Use this work AT YOUR OWN RISK as stated in [COPYING](COPYING).

## Tools
* [mktftpimg](mktftpimg/mktftpimg.c): A simple C program that fabricates a TFTP image out of a stock image
* [mkowrtimg-archerc20v5](mkowrtimg-archerc20v5): A bash script that automates "image fabrication" for release

## Why Use Stock Image to Make OpenWRT Image?
Summary: U-Boot code TP-Link made too bad.

TP-Link devlopers made this incredible feature called "dual u-boot". Unfortunately, they vandalised the code base in the process. TP-Link's work will never make it to the main u-boot code base. Integrating TP-Link's U-Boot into OpenWRT build process is too much work and not feasible. We're stuck with using stock U-Boot binary.

Kudos to [Linaro1985](https://github.com/Linaro1985/openwrt). If it wasn't for his work, it would have taken me more than it should have. Refer to his [commit](https://github.com/Linaro1985/openwrt/commit/4a34b4b48d77c3e25b760ebabe2b9eada1cb4412) on making factory and tftp images. [mkowrtimg-archerc20v5](mkowrtimg-archerc20v5) is just an automation of the instruction described in the commit log.

## How to Build New Release
1. Merge [tl_archer_c20v5](https://github.com/ashegoulding/openwrt/tree/tl_archer_c20v5) branch with the new release tag(say, [v19.07.3](https://github.com/openwrt/openwrt/releases/tag/v19.07.3)). It will merge automatically most of time. If not, good luck!
```
# HEAD: tl_archer_c20v5
git merge v19.07.3
```
2. Download config from official release. For example,
```
wget https://downloads.openwrt.org/releases/19.07.3/targets/ramips/mt76x8/config.buildinfo
mv config.buildinfo .config
```
3. Run `make menuconfig` to sync `.config` with local dependencies. Just run the command, save and exit. Or, you might want to build just one target(C20 v5). This config will build all the other mt76x8 targets as well
4. Run `make`
5. Delete all images except for sysupgrade image. Use [mkowrtimg-archerc20v5](mkowrtimg-archerc20v5) to fabricate the tftp and factory images. You'll need a stock image from TP-Link
```
# PWD: bin/targets/ramips/mt76x8
rm -f *-tftp-recovery.bin *-factory.bin
mkowrtimg-archerc20v5 openwrt-<VERSION>-ramips-mt76x8-tplink_c20-v5-squashfs- <PATH TO STOCK IMAGE> <SYSUPGRADE IMAGE>
# Rebuild checksums
sha256sum -b *.bin > sha256sums
```
6. Delete `packages` directory, maybe?
7. Rename `bin/targets/ramips/mt76x8` directory, zip it, release it!
