package run

import (
	"C"
	"bytes"
	"os/exec"
)

func Command(_cmd string, outBuf *[]byte, outBufLen int, errBuf *[]byte, errBufLen int) int {
	if len(_cmd) == 0 {
		return -1
	}

	var cmd *exec.Cmd
	var stdout, stderr bytes.Buffer

	cmd = exec.Command("sh", "-c", _cmd)

	cmd.Stdout = &stdout
	cmd.Stderr = &stderr

	if error := cmd.Run(); error != nil {
		return -1
	}

	if _, error := stdout.Read(*outBuf); error != nil {
		return -1
	}

	if _, error := stderr.Read(*errBuf); error != nil {
		return -1
	}

	return 127
}
