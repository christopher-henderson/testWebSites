package certutil

import "crypto/x509"

type Certificate struct {
	PEM                 x509.Certificate
	NSSValidationResult string
	NSSError            error
}
