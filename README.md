# findcode

findcode displays information about CODEBLOCKs in each file. The CODEBLOCK are
defined as <name> { ... }.

## Usage

Currently the 'findcode' simply outputs the codeblock name and which line the
block was open '{' and closed '}'.

See the following example:

1. Create a `testing.cfg` file that uses a `block{}` structure
```sh
$ cat > testing.cfg << EOF
configuration {
    server {
        daemon
    }
}
EOF
```

2. Execute `./findcode` with the created file:
```sh
$ ./findcode testing.cfg
testing.cfg:2:4: server
testing.cfg:1:5: configuration
```

## Build

We used Makefile and make to compile the source code

```
$ make
$ ./findcode <file>
```
