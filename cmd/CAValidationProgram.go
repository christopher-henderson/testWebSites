package main

import (
	"crypto/x509"
	"encoding/base64"
	"encoding/pem"
	"log"
	"net/http"
	"regexp"

	"github.com/mozilla/OneCRL-Tools/certdata"
)

var stripper *regexp.Regexp

func init() {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
	stripper = regexp.MustCompile("('|'|\n|-----BEGIN CERTIFICATE-----|-----END CERTIFICATE-----)")
}

func Cert() {
	resp, err := http.Get("https://validec.entrust.net/")
	if err != nil {
		log.Panic(err)
	}
	log.Println(resp.TLS.VerifiedChains)
	resp, err = http.Get("https://expiredec.entrust.net/")
	// if err != nil {
	// 	log.Panic(err)
	// }
	log.Println(resp.TLS.VerifiedChains)
}

func Get() []*x509.Certificate {
	r, err := http.Get(certdata.URL)
	if err != nil {
		log.Panic(err)
	}
	defer r.Body.Close()
	c, err := certdata.ParseToNormalizedForm(r.Body)
	if err != nil {
		log.Panic(err)
	}
	ret := make([]*x509.Certificate, len(c))
	for i, cert := range c {
		if len(cert.PEM) == 0 {
			continue
		}
		if i == 0 {
			log.Println(cert.PEM)
		}
		block, _ := pem.Decode([]byte(cert.PEM))
		if block == nil {
			log.Println(cert.PEM)
			log.Panic("fuck this noise \n")
		}
		cc, err := x509.ParseCertificate(block.Bytes)
		if err != nil {
			log.Println(stripper.ReplaceAllString(cert.PEM, ""))
			log.Panic(err)
		}
		ret[i] = cc
	}
	return ret

}

func main() {
	things := Get()
	log.Println(base64.StdEncoding.EncodeToString(things[0].SubjectKeyId))
}
