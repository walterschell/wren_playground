import "posix" for Shell

class uname {
    static uname() {
        var shell = Shell.create()
        shell.write_stdin("uname -a\n")
        return shell.read_stdout()
    }
}