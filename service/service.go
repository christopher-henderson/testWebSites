package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"os/exec"
	"path"
	"strings"

	"github.com/mozilla/OneCRL-Tools/certdataDiffCCADB"

	"service/certutil"
)

type Result struct {
	Results []string `json:"result"`
}

func toCertList(r io.Reader) []string {
	buf, err := ioutil.ReadAll(r)
	if err != nil {
		log.Panic("asds")
	}
	return strings.SplitAfter(string(buf), "-----END CERTIFICATE-----")
}

func writeProvidedChainToDisk(cert string) string {
	f, err := ioutil.TempFile("/Users/chris/Documents/Contracting/mozilla/testWebSites/getcerts/certChains", "")
	if err != nil {
		err = errors.New(err.Error())
		log.Panicln(err)
	}
	defer f.Close()
	_, err = fmt.Fprintln(f, cert)
	if err != nil {
		log.Panicln(err)
	}
	return f.Name()
}

func install(cert string) string {
	// $CERTUTIL -A -n letsencrypt.org -t "P,p,p" -i wwwletsencryptorg.crt  -d testdb
	fname := writeProvidedChainToDisk(certdataDiffCCADB.NormalizePEM(cert))
	nickname := path.Base(fname)
	cmd := exec.Command(certutilBin, "-A", "-n", nickname, "-t", `P,p,p`, "-i", fname, "-d", db)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(cert)
		log.Println(cmd.Args)
		log.Println(fname)
		log.Println(string(out))
		log.Panicln(err)
	}
	log.Println(cmd.Args)
	return fname
}

func uninstall(nickname string) {
	cmd := exec.Command(certutilBin, "-D", "-n", nickname, "-d", db)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(cmd.Args)
		log.Println(string(out))
		log.Panicln(err)
	}
}

func verify(nickname string) string {
	// -V -n letsencrypt.org -u V -d testdb/
	cmd := exec.Command(certutilBin, "-V", "-n", nickname, "-u", "V", "-d", db)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(cmd.Args)
		log.Println(string(out))
		log.Panicln(err)
	}
	return string(out)
}

func handler(resp http.ResponseWriter, req *http.Request) {
	defer req.Body.Close()
	certs := toCertList(req.Body)
	var r Result
	// lol no fixit
	for _, cert := range certs[:len(certs)-1] {
		nickname := install(cert)
		defer uninstall(path.Base(nickname))
		result := verify(path.Base(nickname))
		r.Results = append(r.Results, result)
	}
	err := json.NewEncoder(resp).Encode(r)
	if err != nil {
		log.Panicln(err)
	}
}

func initDB() {
	if err := os.RemoveAll(db); err != nil {
		log.Panicln(err)
	}
	if err := os.Mkdir(db, 0777); err != nil {
		log.Panicln(err)
	}
	cmd := exec.Command(certutilBin, "-N", "-d", db)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(cmd.Args)
		log.Println(string(out))
		log.Panicln(err)
	}
}

const certutilBin = "/Users/chris/Documents/Contracting/mozilla/testWebSites/dist/Debug/bin/certutil"
const db = "/Users/chris/Documents/Contracting/mozilla/testWebSites/testdb"

const DB = "/Users/chris/Documents/Contracting/mozilla/testWebSites/testdb"
const DIST = "/Users/chris/Documents/Contracting/mozilla/testWebSites/dist/Debug"

func main() {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
	certutil.Init(DB, DIST)
	out, err := certutil.Execute([]string{"-V", "-n", "expiredec.entrust.net", "-u", "V", "-d", DB})
	log.Println(out)
	log.Println(err)
	// os.Setenv("DYLD_LIBRARY_PATH", os.ExpandEnv("$DYLD_LIBRARY_PATH:/Users/chris/Documents/Contracting/mozilla/testWebSites/dist/Debug/lib"))
	// log.Println(os.Getenv("DYLD_LIBRARY_PATH"))
	// r := mux.NewRouter()
	// r.Methods("POST").HandleFunc("/verify_provided_chain", ProductHandler)
	// initDB()
	// http.ListenAndServe("0.0.0.0:8000", http.HandlerFunc(handler))
}
