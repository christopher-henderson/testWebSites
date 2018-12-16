package certutil

import (
	"bytes"
	"crypto/x509"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"os/exec"
	"path"

	"github.com/pkg/errors"
)

const (
	NewCertificateDatabase = "-N"
	NoPassword             = "--empty-password"
	CertDbDirectory        = "-d"

	InstallCert        = "-A"
	CertName           = "-n"
	TrustArgs          = "-t"
	TrustSSLServerOnly = "P,p,p"

	Verify    = "-V"
	CertUsage = "-u"
	SSLServer = "V"
)

var executable = "certutil"
var db string

func Init(dbDir string, dist string) error {
	db = dbDir
	ldpath := os.Getenv(LIBRARY_PATH)
	ldpath = ldpath + ":" + path.Join(dist, "lib")
	os.Setenv(LIBRARY_PATH, ldpath)
	binPath := os.Getenv("PATH")
	os.Setenv("PATH", binPath+":"+path.Join(dist, "bin"))
	help, err := execute([]string{})
	if !bytes.Contains(help, []byte("certutil - Utility to manipulate NSS certificate databases")) {
		return errors.Wrap(err, fmt.Sprintf("Failed to load '%s' given the PATH=%s\n$ certutil\n%s", executable, os.Getenv("PATH"), string(help)))
	}
	return nil
}

type Certutil struct {
	tmpDir string
}

func NewCertutil() (certutil Certutil, err error) {
	tmpDir, err := ioutil.TempDir("", "")
	if err != nil {
		return
	}
	certutil.tmpDir = tmpDir
	out, err := execute([]string{NewCertificateDatabase, NoPassword, CertDbDirectory, certutil.tmpDir})
	if err != nil {
		log.Println(string(out))
	}
	return
}

func (c Certutil) Install(cert *x509.Certificate) ([]byte, error) {
	return execute([]string{
		InstallCert,
		TrustArgs, TrustSSLServerOnly,
		CertName, cert.Subject.String(),
		CertDbDirectory, c.tmpDir,
	}, cert.Raw...)
}

func (c Certutil) Verify(cert *x509.Certificate) ([]byte, error) {
	return execute([]string{
		Verify,
		CertName, cert.Subject.String(),
		CertUsage, SSLServer,
		CertDbDirectory, c.tmpDir,
	})
}

func (c Certutil) Delete() {
	os.RemoveAll(c.tmpDir)
}

func execute(args []string, input ...byte) ([]byte, error) {
	cmd := exec.Command(executable, args...)
	cmd.Stdin = bytes.NewBuffer(input)
	out, err := cmd.CombinedOutput()
	return bytes.TrimSpace(out), err
}
