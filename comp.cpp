// #include <ctime>-stdlib=libc++
// #include "Time.h"
// using namespace std;
// #include <ctime>
// #include "mozpkix/Result.h"
// #include <ctime>
#include "nss/nss.h"
#include "nspr/prtypes.h"
#include "mozpkix/pkixtypes.h"
#include "mozpkix/Result.h"
#include "mozpkix/Time.h"

using namespace mozilla::pkix;

class MyWittleTrustDomain : protected TrustDomain {

public:
  MyWittleTrustDomain() {}
	// Determine the level of trust in the given certificate for the given role.
  // This will be called for every certificate encountered during path
  // building.
  //
  // When policy.IsAnyPolicy(), then no policy-related checking should be done.
  // When !policy.IsAnyPolicy(), then GetCertTrust MUST NOT return with
  // trustLevel == TrustAnchor unless the given cert is considered a trust
  // anchor *for that policy*. In particular, if the user has marked an
  // intermediate certificate as trusted, but that intermediate isn't in the
  // list of EV roots, then GetCertTrust must result in
  // trustLevel == InheritsTrust instead of trustLevel == TrustAnchor
  // (assuming the candidate cert is not actively distrusted).
  Result GetCertTrust(EndEntityOrCA endEntityOrCA,
                              const CertPolicyId& policy,
                              Input candidateCertDER,
                              /*out*/ TrustLevel& trustLevel) {
		return Result::ERROR_UNTRUSTED_CERT;
	}

  class IC : public TrustDomain::IssuerChecker {
   public:
    // potentialIssuerDER is the complete DER encoding of the certificate to
    // be checked as a potential issuer.
    //
    // If additionalNameConstraints is not nullptr then it must point to an
    // encoded NameConstraints extension value; in that case, those name
    // constraints will be checked in addition to any any name constraints
    // contained in potentialIssuerDER.
    Result Check(Input potentialIssuerDER,
                         /*optional*/ const Input* additionalNameConstraints,
                         /*out*/ bool& keepGoing) {
    	return Result::ERROR_UNTRUSTED_CERT;
	}
  };

  // Search for a CA certificate with the given name. The implementation must
  // call checker.Check with the DER encoding of the potential issuer
  // certificate. The implementation must follow these rules:
  //
  // * The implementation must be reentrant and must limit the amount of stack
  //   space it uses; see the note on reentrancy and stack usage below.
  // * When checker.Check does not return Success then immediately return its
  //   return value.
  // * When checker.Check returns Success and sets keepGoing = false, then
  //   immediately return Success.
  // * When checker.Check returns Success and sets keepGoing = true, then
  //   call checker.Check again with a different potential issuer certificate,
  //   if any more are available.
  // * When no more potential issuer certificates are available, return
  //   Success.
  // * Don't call checker.Check with the same potential issuer certificate more
  //   than once in a given call of FindIssuer.
  // * The given time parameter may be used to filter out certificates that are
  //   not valid at the given time, or it may be ignored.
  //
  // Note on reentrancy and stack usage: checker.Check will attempt to
  // recursively build a certificate path from the potential issuer it is given
  // to a trusted root, as determined by this TrustDomain. That means that
  // checker.Check may call any/all of the methods on this TrustDomain. In
  // particular, there will be call stacks that look like this:
  //
  //    BuildCertChain
  //      [...]
  //        TrustDomain::FindIssuer
  //          [...]
  //            IssuerChecker::Check
  //              [...]
  //                TrustDomain::FindIssuer
  //                  [...]
  //                    IssuerChecker::Check
  //                      [...]
  //
  // checker.Check is responsible for limiting the recursion to a reasonable
  // limit.
  //
  // checker.Check will verify that the subject's issuer field matches the
  // potential issuer's subject field. It will also check that the potential
  // issuer is valid at the given time. However, if the FindIssuer
  // implementation has an efficient way of filtering potential issuers by name
  // and/or validity period itself, then it is probably better for performance
  // for it to do so.
  Result FindIssuer(Input encodedIssuerName, IssuerChecker& checker, Time time) {
  	return Result::ERROR_UNTRUSTED_CERT;
  }

  // Called as soon as we think we have a valid chain but before revocation
  // checks are done. This function can be used to compute additional checks,
  // especially checks that require the entire certificate chain. This callback
  // can also be used to save a copy of the built certificate chain for later
  // use.
  //
  // This function may be called multiple times, regardless of whether it
  // returns success or failure. It is guaranteed that BuildCertChain will not
  // return Success unless the last call to IsChainValid returns Success.
  // Further,
  // it is guaranteed that when BuildCertChain returns Success the last chain
  // passed to IsChainValid is the valid chain that should be used for further
  // operations that require the whole chain.
  //
  // Keep in mind, in particular, that if the application saves a copy of the
  // certificate chain the last invocation of IsChainValid during a validation,
  // it is still possible for BuildCertChain to fail, in which case the
  // application must not assume anything about the validity of the last
  // certificate chain passed to IsChainValid; especially, it would be very
  // wrong to assume that the certificate chain is valid.
  //
  // certChain.GetDER(0) is the trust anchor.
  Result IsChainValid(const DERArray& certChain, Time time, const CertPolicyId& requiredPolicy) {
  	return Result::ERROR_UNTRUSTED_CERT;
  }

  Result CheckRevocation(EndEntityOrCA endEntityOrCA,
                                 const CertID& certID, Time time,
                                 Duration validityDuration,
                                 /*optional*/ const Input* stapledOCSPresponse,
                                 /*optional*/ const Input* aiaExtension) {
                                 return Result::ERROR_UNTRUSTED_CERT;
                             }

  // Check that the given digest algorithm is acceptable for use in signatures.
  //
  // Return Success if the algorithm is acceptable,
  // Result::ERROR_CERT_SIGNATURE_ALGORITHM_DISABLED if the algorithm is not
  // acceptable, or another error code if another error occurred.
  Result CheckSignatureDigestAlgorithm(DigestAlgorithm digestAlg,
                                               EndEntityOrCA endEntityOrCA,
                                               Time notBefore) {
  	return Result::ERROR_UNTRUSTED_CERT;
  }

  // Check that the RSA public key size is acceptable.
  //
  // Return Success if the key size is acceptable,
  // Result::ERROR_INADEQUATE_KEY_SIZE if the key size is not acceptable,
  // or another error code if another error occurred.
  Result CheckRSAPublicKeyModulusSizeInBits(
      EndEntityOrCA endEntityOrCA, unsigned int modulusSizeInBits) {
  	return Result::ERROR_UNTRUSTED_CERT;
  }

  // Verify the given RSA PKCS#1.5 signature on the given digest using the
  // given RSA public key.
  //
  // CheckRSAPublicKeyModulusSizeInBits will be called before calling this
  // function, so it is not necessary to repeat those checks here. However,
  // VerifyRSAPKCS1SignedDigest *is* responsible for doing the mathematical
  // verification of the public key validity as specified in NIST SP 800-56A.
  Result VerifyRSAPKCS1SignedDigest(const SignedDigest& signedDigest,
                                            Input subjectPublicKeyInfo) {
  	return Result::ERROR_UNTRUSTED_CERT;
  }

  // Check that the given named ECC curve is acceptable for ECDSA signatures.
  //
  // Return Success if the curve is acceptable,
  // Result::ERROR_UNSUPPORTED_ELLIPTIC_CURVE if the curve is not acceptable,
  // or another error code if another error occurred.
  Result CheckECDSACurveIsAcceptable(EndEntityOrCA endEntityOrCA,
                                             NamedCurve curve) {
  	return Result::ERROR_UNTRUSTED_CERT;
  }

  // Verify the given ECDSA signature on the given digest using the given ECC
  // public key.
  //
  // CheckECDSACurveIsAcceptable will be called before calling this function,
  // so it is not necessary to repeat that check here. However,
  // VerifyECDSASignedDigest *is* responsible for doing the mathematical
  // verification of the public key validity as specified in NIST SP 800-56A.
  Result VerifyECDSASignedDigest(const SignedDigest& signedDigest,
                                         Input subjectPublicKeyInfo) {
  	return Result::ERROR_UNTRUSTED_CERT;
  }

  // Check that the validity duration is acceptable.
  //
  // Return Success if the validity duration is acceptable,
  // Result::ERROR_VALIDITY_TOO_LONG if the validity duration is not acceptable,
  // or another error code if another error occurred.
  Result CheckValidityIsAcceptable(Time notBefore, Time notAfter,
                                           EndEntityOrCA endEntityOrCA,
                                           KeyPurposeId keyPurpose) {
  	return Result::ERROR_UNTRUSTED_CERT;
  }

  // For compatibility, a CA certificate with an extended key usage that
  // contains the id-Netscape-stepUp OID but does not contain the
  // id-kp-serverAuth OID may be considered valid for issuing server auth
  // certificates. This function allows TrustDomain implementations to control
  // this setting based on the start of the validity period of the certificate
  // in question.
  Result NetscapeStepUpMatchesServerAuth(Time notBefore,
                                                 /*out*/ bool& matches) {
  	return Result::ERROR_UNTRUSTED_CERT;
}

  // Some certificate or OCSP response extensions do not directly participate
  // in the verification flow, but might still be of interest to the clients
  // (notably Certificate Transparency data, RFC 6962). Such extensions are
  // extracted and passed to this function for further processing.
  void NoteAuxiliaryExtension(AuxiliaryExtension extension,
                                      Input extensionData) {
  	//
  }

  // Compute a digest of the data in item using the given digest algorithm.
  //
  // item contains the data to hash.
  // digestBuf points to a buffer to where the digest will be written.
  // digestBufLen will be the size of the digest output (20 for SHA-1,
  // 32 for SHA-256, etc.).
  //
  // TODO: Taking the output buffer as (uint8_t*, size_t) is counter to our
  // other, extensive, memory safety efforts in mozilla::pkix, and we should
  // find a way to provide a more-obviously-safe interface.
  Result DigestBuf(Input item, DigestAlgorithm digestAlg,
                           /*out*/ uint8_t* digestBuf, size_t digestBufLen) {
  	return Result::ERROR_UNTRUSTED_CERT;
  }

};

int main(int argc, char const *argv[])
{	
	NSS_Init("lok/tar/ogar");
	auto td = std::unique_ptr<MyWittleTrustDomain>(new MyWittleTrustDomain);
	return 0;
}