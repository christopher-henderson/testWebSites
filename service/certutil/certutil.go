package certutil

import (
	"errors"
	"io"
	"io/ioutil"
	"log"
	"os"
	"os/exec"
	"path"
	"strings"
	"sync"
)

var executable = "certutil"

var db string

func Init(dbDir string, dist string) {
	db = dbDir
	ldpath := os.Getenv(LIBRARY_PATH)
	ldpath = ldpath + ":" + path.Join(dist, "lib")
	os.Setenv(LIBRARY_PATH, ldpath)
	binPath := os.Getenv("PATH")
	os.Setenv("PATH", binPath+":"+path.Join(dist, "bin"))
}

func Install(cert string) error {
	return nil
}

func Delete(nickame string) error {

}

func Verify(nickname string) error {

}

func Execute(args []string, input ...string) (string, error) {
	cmd := exec.Command(executable, args...)
	wg := sync.WaitGroup{}
	outR, outW := io.Pipe()
	errR, errW := io.Pipe()
	inR, inW := io.Pipe()
	cmd.Stdout = outW
	cmd.Stderr = errW
	cmd.Stdin = inR
	var stderr []byte
	var stdout []byte
	var errErr error
	var outErr error
	wg.Add(3)
	go func() {
		stderr, errErr = ioutil.ReadAll(errR)
		wg.Done()
	}()
	go func() {
		stdout, outErr = ioutil.ReadAll(outR)
		wg.Done()
	}()
	go func() {
		newLineDelimitedInput := strings.Join(input, "\n")
		io.Copy(inW, strings.NewReader(newLineDelimitedInput))
		inW.Close()
		wg.Done()
	}()
	cmdErr := cmd.Run()
	outW.Close()
	errW.Close()
	wg.Wait()
	// @TODO do something better with these
	// It's awkward but these would be big explosions
	if errErr != nil {
		log.Println(errErr, cmdErr)
	}
	if outErr != nil {
		log.Println(outErr)
	}
	if len(stderr) > 0 {
		return "", errors.New(string(stderr))
	}
	return string(stdout), nil
}
