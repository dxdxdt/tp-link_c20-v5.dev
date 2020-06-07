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

## How to Make TFTP Images w/ mktftpimg
Many people complained after bricking their non-US hw variants by flashing them with my build. Since I don't have access to other C20 v5 variants, it's hard to determine whether it's my fault or not. I made **mktftpimg** so you can make a stock tftp image.

Again, I'm not responsible for any damages. I personally broke my C20 v5 once by flashing it too many times. Apparently, flashing your too many times in a short period of time can damage your hw. EXERCISE TRIGGER DISCIPLINE.

### You will need

1. **mktftpimg** binary. Build it yourself or download it from [releases](https://github.com/ashegoulding/tp-link_c20-v5.dev/releases)
2. A stock image of your variant. Just google your variant and download it from TP-Link's website

### On Linux

```
./mktftpimg Archer_C20v5_US_0.9.1_4.16_up_boot[181213-rel33336].bin tp_recovery.bin 512 196608
```

### On Windows(cmd.exe)

```
mktftpimg.exe Archer_C20v5_US_0.9.1_4.16_up_boot[181213-rel33336].bin tp_recovery.bin 512 196608
```

### Notes
`Archer_C20v5_US_0.9.1_4.16_up_boot[181213-rel33336].bin` is the name of the stock image. Change it to your stock image.

**512** is the size of header(`sizeof(struct fw_header)` is 512: [exb A](https://github.com/openwrt/openwrt/blob/master/tools/firmware-utils/src/mktplinkfw.c), [exb B](https://github.com/openwrt/openwrt/blob/master/tools/firmware-utils/src/mktplinkfw2.c)). The actual binary that the hardware loads starts after the header.

**196608**(0x30000) is the size of the first boot loader region. The TFTP recovery program skips 196608 bytes of the downloaded image when copying into the flash. Stupid design if you ask me. This number(0x30000) appears on the serial output when performing TFTP reset. 0x30000 is probably the size of the first boot loader for all variants. But you can only find this out if you have the serial console. If you think it's not for your variant, well. Shotgun it(trial and error). I highly doubt it's other than 0x30000, though.

The first boot loader never touches its region(0x0 - 0x30000). The first boot loader will always be there to help you(unless you manually modified it). As long as the first boot loader is intact, you can debrick your hw. Cheers.
