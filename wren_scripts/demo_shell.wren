import "posix" for Shell

var shell = Shell.create()
shell.write_stdin("uname -a\n")
var uname = shell.read_stdout()
System.print("uname is " + uname)