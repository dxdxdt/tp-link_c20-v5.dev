import os

def get_file_path(prompt):
    while True:
        file_path = input(prompt)
        if os.path.isfile(file_path):
            return file_path
        else:
            print("Invalid file path. Please try again.")

def fabricate_tftp_image(in_file, out_file, skip, zeros):
    ZERO = b'\x00'

    try:
        with open(in_file, 'rb') as fin:
            fin.seek(0, os.SEEK_END)
            fin_size = fin.tell()
            if fin_size < skip:
                print(f"Size of {in_file} is smaller than {skip}")
                return 2

            fin.seek(skip)
            load_size = fin_size - skip

            buf = fin.read(load_size)

        with open(out_file, 'xb') as fout:
            fout.write(ZERO * zeros)
            fout.write(buf)

        return 0

    except Exception as e:
        print(f"Error: {e}")
        return 2

if __name__ == "__main__":
    in_file = get_file_path("Enter the path for <in file>: ")
    out_file = get_file_path("Enter the path for <out file>: ")

    try:
        skip = int(input("Enter the value for <skip>: "))
        zeros = int(input("Enter the value for <zero>: "))
    except ValueError:
        print("Invalid input for <skip> or <zero>")
        sys.exit(1)

    result = fabricate_tftp_image(in_file, out_file, skip, zeros)
    sys.exit(result)
