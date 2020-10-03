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
        daemon = enabled
        listen = http://localhost:12345
    }
}

stats {
    port = getenv("STATS_PORT")
    internal = 10s
}
EOF
```

2. Execute `./findcode` with the created file:
```sh
$ ./findcode testing.sample
testing.sample:     server
testing.sample: configuration
testing.sample: stats
```

3. Execute with `-n` to show line number where the block started and ended:
```sh
$ ./findcode -n testing.sample
testing2.sample:2:5:     server
testing2.sample:1:6: configuration
testing2.sample:8:11: stats
```

## Build

We used Makefile and make to compile the source code

```
$ make
$ ./findcode <file>
```
