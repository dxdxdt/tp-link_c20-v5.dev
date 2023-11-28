import sys
import shutil
import os

def get_file_path(prompt):
    while True:
        file_path = input(prompt)
        if os.path.isfile(file_path):
            return file_path
        else:
            print("Invalid file path. Please try again.")

def usage():
    print(f"{os.path.basename(__file__)} <prefix> <stock img> <openwrt img>")

if len(sys.argv) != 1:
    usage()
    sys.exit(1)

PREFIX = get_file_path("Enter the path for <prefix>: ")
IMG_STOCK = get_file_path("Enter the path for <stock img>: ")
IMG_OWRT = get_file_path("Enter the path for <openwrt img>: ")

with open(f"{PREFIX}-tftp-recovery.bin", 'wb') as zero_file:
    zero_file.write(b'\x00' * 196608)

shutil.copyfile(IMG_STOCK, f"{PREFIX}-factory.bin")

with open(f"{PREFIX}-tftp-recovery.bin", 'ab') as tftp_recovery_file:
    with open(f"{PREFIX}-factory.bin", 'rb') as factory_file:
        factory_file.seek(512)
        tftp_recovery_file.write(factory_file.read())

    with open(IMG_OWRT, 'rb') as owrt_file:
        tftp_recovery_file.write(owrt_file.read())

with open(f"{PREFIX}-factory.bin", 'ab') as factory_file:
    with open(IMG_OWRT, 'rb') as owrt_file:
        factory_file.write(owrt_file.read())
