import subprocess 
import platform
import argparse
import sys
import os

try:
    import conan
except ImportError:
    print("Conan is not installed! Go to https://conan.io/downloads.html to download or use `pip install conan`", file=sys.stderr)


class ConanArgParser(argparse.ArgumentParser):
    """ Arg parser for getting all the conan stuff where it needs to get to """
    def __init__(self):
        super().__init__()

    def parse_arguments(self):
        if len(sys.argv) == 1:
            self.print_help()
            sys.exit(1)

        # Required Arguments
        self.add_argument('-c', '--conan-file', action='store', dest='conan_file', required=True, help='Conan File Path')
        self.add_argument('-b', '--build-dir', action='store', dest='conan_build_dir', required=True, help='Build Directory')

        # Optional - Will be built out if not provided
        self.add_argument('-t', '--build-type', action='store', dest='conan_build_type', default='Debug', help='Build Type (Debug/Release)')
        self.add_argument('-v', '--vendor-dir', action='store', dest='conan_vendor_dir', help='Output Directory for Vendor Imports')

        # Print Help
        self.add_argument('-h', '--help', action='store', dest='conan_print_help', help='Shows Help')

        return self.parse_args()

    def error(self, message):
        sys.stderr.write('error: %s\n' % message)
        self.print_help()
        sys.exit(2)


def do_it():
    # Get arguments and print help if asked for
    args = ConanArgParser().parse_arguments()
    if args.conan_print_help:
        ConanArgParser().print_help()
        sys.exit(0)

    # If no vendor path is provided, dump it into Glint/Vendors
    vendor_path = args.conan_vendor_dir if args.conan_vendor_dir else f"{args.conan_file}/Glint/Vendors/"

    if platform.system() == "Windows":
	    subprocess.run('conan', 'install', args.conan_file, '-s', f"build_type={args.conan_build_type}", '--no-imports', f"-if={args.conan_build_dir}")
	    subprocess.run('conan', 'imports', f"-if={args.conan_build_dir}", '-imf', vendor_path, args.conan_file)

if __name__ == '__main__':
    os.system('conan install ./conanfile.py')