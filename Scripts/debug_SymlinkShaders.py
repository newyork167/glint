import platform
import argparse
import sys
import os

class SymlinkArgParser(argparse.ArgumentParser):
    """ Arg parser for getting all the conan stuff where it needs to get to """
    def __init__(self):
        super().__init__()

    def parse_arguments(self):
        if len(sys.argv) == 1:
            self.print_help()
            sys.exit(1)

        # Required Arguments
        self.add_argument('-s', '--source', action='store', dest='shader_src', required=True, help='Shader Source Path')
        self.add_argument('-d', '--dest', action='store', dest='shader_dst', required=True, help=' Shader Destination Link')

        return self.parse_args()

    def error(self, message):
        sys.stderr.write('error: %s\n' % message)
        self.print_help()
        sys.exit(2)

if __name__ == '__main__':
    args = SymlinkArgParser().parse_arguments()
    print(f"Symlinking from {args.shader_src} to {args.shader_dst}")

    if platform.system() == "Windows":
        import _winapi
        _winapi.CreateJunction(args.shader_src, args.shader_dst)
    else:
        os.symlink(args.shader_src, args.shader_dst)