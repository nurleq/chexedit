chexedit readme
```
This code provides a way to edit the memory of a running Windows process using the `cmd.exe` shell. The user is prompted for
an address in hex and a value, and then the corresponding byte in memory is replaced with the new value.

Here's how you can use this code:

1. Compile the source file into an executable.
2. Run the executable, but do not execute it directly using double quotes. Instead, redirect the input to the process by
passing its executable name followed by a pipe and the input.

Example:
```bash
cmd.exe | input.txt
```
Where `input.txt` contains the hexadecimal addresses of the memory locations you want to edit, one address per line, along
with the new values for those addresses.