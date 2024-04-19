from os import remove, system
from sys import argv


def main():
    if len(argv) < 2:
        print("Usage: python compileAsm32.py <file.asm>")
        return

    filename = argv[1]
    if filename[-4:] != ".asm":
        print("Invalid file extension")
        return

    filename = filename[:-4]
    system(f"nasm -f elf -g {filename}.asm -o {filename}.o")
    system(f"ld -m elf_i386 -g {filename}.o -o {filename}")
    remove(f"{filename}.o")


if __name__ == '__main__':
    main()
