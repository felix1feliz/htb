# HTB

HTB is a tool for manually writing binary files. It only works with linux. The file format for htb follows these rules:
 - Bytes must be writen as groups of two ascii characters representing the hexadecimal value of the byte
 - There must not be any non-hexadecimal characters outside of comments
 - Comments can be written between '<' and '>'

Here is an example of a htb file:

```
< This is a comment >
<< This is also a comment >>
0123 45 6789ab
```

## Instalation

You can install htb by cloning the repo and runing `sudo make install` or `sudo make clean install`. The 'INSTALLFOLDER' variable controls where the binary is installed. If you wish to simply compile, you can just run `make`.

## Usage

htb \[OPTIONS] <SOURCE>

Options:
 -h         Prints this
 -o <PATH>  Specifies the output file
 -e         Gives output execute permission
