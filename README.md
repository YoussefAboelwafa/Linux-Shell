![photo_2023-03-06_18-05-08](https://user-images.githubusercontent.com/96186143/223166068-0d80e35b-b46b-485c-8a49-aff1c204a369.jpg)
# Linux Shell
This is a Unix shell program </br>
A shell is simply a program that conveniently allows you to run other programs
This shell supports the following commands:

1. The internal shell command `exit` which terminates the shell
2. A command with no arguments
    * **Example**: `ls`, `cp`, `rm` 
    * **System calls**: `fork()`, `execvp()`, `exit()`, `waitpid()`
3. A command with arguments
    * **Example**: `ls –l`
4. A command, with or without arguments, executed in the background using &.
    * **Example**: `gedit &`to run in the background or `gedit` to block terminal until finish
    * **Details**: In the case of `gedit &`, the shell must execute the command and return immediately, not blocking until the command finishes.
    * **Concepts**: Background execution, signals & signal handlers
5. Shell builtin commands
    * **Commands**: `cd` & `echo`
    * **Details**:</br>
             `cd ~` </br>
             `cd ..`</br>
             `cd <absolute_path>`</br>
             `cd <relative_path_to_current_working_directory>`</br>
             `echo "wow"` => wow</br>
             `export x=5`</br>
             `echo "Hello $x"` => Hello 5</br>
6. Expression evaluation
    * **Commands**: `export`
    * **Details**: Set values to variables and print variables values. No mathematical operations is needed.
    * **Export Details**: Accept input of two forms, either a string without spaces, or a full string inside double quotations.
    * **Example**:</br>
         `export x=-l` </br>
         `ls $x` => Will perform `ls -l`</br>
         `export y`="Hello world" </br>
         `echo "$y"` => Hello world </br>
