package main

import (
	"fmt"
	"os"
	run "run/command"
)

func main() {
	cmd := os.Args[1]
	BUFF_SIZE := 1024
	outBuf := make([]byte, BUFF_SIZE)
	errBuf := make([]byte, BUFF_SIZE)
	run.Command(cmd, &outBuf, BUFF_SIZE, &errBuf, BUFF_SIZE)

	fmt.Printf("stdOut:\n%s\n", outBuf)
	fmt.Printf("stdErr:\n%s\n", errBuf)
}
