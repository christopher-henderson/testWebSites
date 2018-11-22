#include <string>
#include <iostream>

#include "mozpkix/pkixtypes.h"
#include "mozpkix/Result.h"
#include "mozpkix/Time.h"
#include "mozpkix/pkix.h"
#include "nspr/prerror.h"
#include "nspr/prtypes.h"
#include "nss/nss.h"

using namespace mozilla::pkix;

class MyWittleTwustDomain : public TrustDomain {

public:
  MyWittleTwustDomain() {}
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
  Result GetCertTrust(EndEntityOrCA endEntityOrCA, const CertPolicyId& policy, Input candidateCertDER, /*out*/ TrustLevel& trustLevel) {
    std::cout << "GetCertTrust" << std::endl;
		return Success;
	}

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
    std::cout << "FindIssuer" << std::endl;
    bool keepGoing;
    auto result = checker.Check(encodedIssuerName, nullptr, keepGoing);
    if (result != Success) {
      return result;
    }
    if (!keepGoing) {
      return Success;
    }
    std::cout << "the checker goes " << MapResultToName(result) << std::endl;
    std::cout << "the checker thinks that I should " << (keepGoing ? "keep going" : "stop") << std::endl;
  	return Success;
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
    std::cout << "IsChainValid" << std::endl;
    auto i = 0;
    auto der = certChain.GetDER(i);
    while (der != nullptr) {
      std::cout << "a thing!" << std::endl;
      i++;
      der = certChain.GetDER(i);
    }
  	return Success;
  }

  Result CheckRevocation(EndEntityOrCA endEntityOrCA,
                                 const CertID& certID, Time time,
                                 Duration validityDuration,
                                 /*optional*/ const Input* stapledOCSPresponse,
                                 /*optional*/ const Input* aiaExtension) {
   std::cout << "CheckRevocation" << std::endl;
   return Success;
  }

  // Check that the given digest algorithm is acceptable for use in signatures.
  //
  // Return Success if the algorithm is acceptable,
  // Result::ERROR_CERT_SIGNATURE_ALGORITHM_DISABLED if the algorithm is not
  // acceptable, or another error code if another error occurred.
  Result CheckSignatureDigestAlgorithm(DigestAlgorithm digestAlg,
                                               EndEntityOrCA endEntityOrCA,
                                               Time notBefore) {
    std::cout << "CheckSignatureDigestAlgorithm" << std::endl;
  	return Success;
  }

  // Check that the RSA public key size is acceptable.
  //
  // Return Success if the key size is acceptable,
  // Result::ERROR_INADEQUATE_KEY_SIZE if the key size is not acceptable,
  // or another error code if another error occurred.
  Result CheckRSAPublicKeyModulusSizeInBits(EndEntityOrCA endEntityOrCA, unsigned int modulusSizeInBits) {
    std::cout << "CheckRSAModulus" << std::endl;
  	return Success;
  }

  // Verify the given RSA PKCS#1.5 signature on the given digest using the
  // given RSA public key.
  //
  // CheckRSAPublicKeyModulusSizeInBits will be called before calling this
  // function, so it is not necessary to repeat those checks here. However,
  // VerifyRSAPKCS1SignedDigest *is* responsible for doing the mathematical
  // verification of the public key validity as specified in NIST SP 800-56A.
  Result VerifyRSAPKCS1SignedDigest(const SignedDigest& signedDigest, Input subjectPublicKeyInfo) {
    std::cout << "VerifyRSA" << std::endl;
  	return Success;
  }

  // Check that the given named ECC curve is acceptable for ECDSA signatures.
  //
  // Return Success if the curve is acceptable,
  // Result::ERROR_UNSUPPORTED_ELLIPTIC_CURVE if the curve is not acceptable,
  // or another error code if another error occurred.
  Result CheckECDSACurveIsAcceptable(EndEntityOrCA endEntityOrCA,
                                             NamedCurve curve) {
    std::cout << "CheckECDSA" << std::endl;
  	return Success;
  }

  // Verify the given ECDSA signature on the given digest using the given ECC
  // public key.
  //
  // CheckECDSACurveIsAcceptable will be called before calling this function,
  // so it is not necessary to repeat that check here. However,
  // VerifyECDSASignedDigest *is* responsible for doing the mathematical
  // verification of the public key validity as specified in NIST SP 800-56A.
  Result VerifyECDSASignedDigest(const SignedDigest& signedDigest, Input subjectPublicKeyInfo) {
    std::cout << "VerifyECDSA" << std::endl;
  	return Success;
  }

  // Check that the validity duration is acceptable.
  //
  // Return Success if the validity duration is acceptable,
  // Result::ERROR_VALIDITY_TOO_LONG if the validity duration is not acceptable,
  // or another error code if another error occurred.
  Result CheckValidityIsAcceptable(Time notBefore, Time notAfter,
                                           EndEntityOrCA endEntityOrCA,
                                           KeyPurposeId keyPurpose) {
    std::cout << "CheckValidty" << std::endl;
  	return Success;
  }

  // For compatibility, a CA certificate with an extended key usage that
  // contains the id-Netscape-stepUp OID but does not contain the
  // id-kp-serverAuth OID may be considered valid for issuing server auth
  // certificates. This function allows TrustDomain implementations to control
  // this setting based on the start of the validity period of the certificate
  // in question.
  Result NetscapeStepUpMatchesServerAuth(Time notBefore, /*out*/ bool& matches) {
    std::cout << "NetscapeSteupUp" << std::endl;
  	return Success;
}

  // Some certificate or OCSP response extensions do not directly participate
  // in the verification flow, but might still be of interest to the clients
  // (notably Certificate Transparency data, RFC 6962). Such extensions are
  // extracted and passed to this function for further processing.
  void NoteAuxiliaryExtension(AuxiliaryExtension extension, Input extensionData) {
  	std::cout << "NoteAux" << std::endl;
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
  Result DigestBuf(Input item, DigestAlgorithm digestAlg, /*out*/ uint8_t* digestBuf, size_t digestBufLen) {
    std::cout << "DigestBuf" << std::endl;
  	return Success;
  }

};

const uint8_t entrust[] = {
0x30, 0x82, 0x05, 0xad, 0x30, 0x82, 0x05, 0x32, 0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x10, 0x62,
0xd8, 0x16, 0xe0, 0x32, 0xf0, 0x87, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x56, 0xa0, 0x57, 0x91, 0x30,
0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x30, 0x81, 0xba, 0x31, 0x0b,
0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x16, 0x30, 0x14, 0x06,
0x03, 0x55, 0x04, 0x0a, 0x13, 0x0d, 0x45, 0x6e, 0x74, 0x72, 0x75, 0x73, 0x74, 0x2c, 0x20, 0x49,
0x6e, 0x63, 0x2e, 0x31, 0x28, 0x30, 0x26, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x1f, 0x53, 0x65,
0x65, 0x20, 0x77, 0x77, 0x77, 0x2e, 0x65, 0x6e, 0x74, 0x72, 0x75, 0x73, 0x74, 0x2e, 0x6e, 0x65,
0x74, 0x2f, 0x6c, 0x65, 0x67, 0x61, 0x6c, 0x2d, 0x74, 0x65, 0x72, 0x6d, 0x73, 0x31, 0x39, 0x30,
0x37, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x30, 0x28, 0x63, 0x29, 0x20, 0x32, 0x30, 0x31, 0x36,
0x20, 0x45, 0x6e, 0x74, 0x72, 0x75, 0x73, 0x74, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x20, 0x2d,
0x20, 0x66, 0x6f, 0x72, 0x20, 0x61, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x7a, 0x65, 0x64, 0x20,
0x75, 0x73, 0x65, 0x20, 0x6f, 0x6e, 0x6c, 0x79, 0x31, 0x2e, 0x30, 0x2c, 0x06, 0x03, 0x55, 0x04,
0x03, 0x13, 0x25, 0x45, 0x6e, 0x74, 0x72, 0x75, 0x73, 0x74, 0x20, 0x43, 0x65, 0x72, 0x74, 0x69,
0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69,
0x74, 0x79, 0x20, 0x2d, 0x20, 0x4c, 0x31, 0x4a, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x38, 0x30, 0x36,
0x32, 0x35, 0x31, 0x33, 0x31, 0x35, 0x35, 0x37, 0x5a, 0x17, 0x0d, 0x31, 0x39, 0x30, 0x36, 0x32,
0x35, 0x31, 0x33, 0x34, 0x35, 0x35, 0x30, 0x5a, 0x30, 0x81, 0xc9, 0x31, 0x0b, 0x30, 0x09, 0x06,
0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x43, 0x41, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04,
0x08, 0x13, 0x07, 0x4f, 0x6e, 0x74, 0x61, 0x72, 0x69, 0x6f, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03,
0x55, 0x04, 0x07, 0x13, 0x06, 0x4b, 0x61, 0x6e, 0x61, 0x74, 0x61, 0x31, 0x13, 0x30, 0x11, 0x06,
0x0b, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82, 0x37, 0x3c, 0x02, 0x01, 0x03, 0x13, 0x02, 0x55, 0x53,
0x31, 0x19, 0x30, 0x17, 0x06, 0x0b, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82, 0x37, 0x3c, 0x02, 0x01,
0x02, 0x13, 0x08, 0x4d, 0x61, 0x72, 0x79, 0x6c, 0x61, 0x6e, 0x64, 0x31, 0x16, 0x30, 0x14, 0x06,
0x03, 0x55, 0x04, 0x0a, 0x13, 0x0d, 0x45, 0x6e, 0x74, 0x72, 0x75, 0x73, 0x74, 0x2c, 0x20, 0x49,
0x6e, 0x63, 0x2e, 0x31, 0x1d, 0x30, 0x1b, 0x06, 0x03, 0x55, 0x04, 0x0f, 0x13, 0x14, 0x50, 0x72,
0x69, 0x76, 0x61, 0x74, 0x65, 0x20, 0x4f, 0x72, 0x67, 0x61, 0x6e, 0x69, 0x7a, 0x61, 0x74, 0x69,
0x6f, 0x6e, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x05, 0x13, 0x09, 0x44, 0x31, 0x35,
0x35, 0x37, 0x36, 0x35, 0x37, 0x32, 0x31, 0x1c, 0x30, 0x1a, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13,
0x13, 0x76, 0x61, 0x6c, 0x69, 0x64, 0x65, 0x63, 0x2e, 0x65, 0x6e, 0x74, 0x72, 0x75, 0x73, 0x74,
0x2e, 0x6e, 0x65, 0x74, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02,
0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x1d,
0x07, 0xbb, 0x95, 0x46, 0x80, 0x52, 0x02, 0x30, 0x47, 0xd1, 0x22, 0x2c, 0x99, 0x61, 0x91, 0xbc,
0x7e, 0x1d, 0xf4, 0xab, 0xdb, 0x63, 0x37, 0x26, 0xf4, 0x03, 0xeb, 0xe7, 0xe7, 0x62, 0x0a, 0x9d,
0xe7, 0x97, 0xba, 0x13, 0x55, 0x04, 0xe3, 0x97, 0xe4, 0xf7, 0x65, 0x29, 0xa8, 0xf8, 0xb5, 0x08,
0x46, 0xd7, 0xad, 0xda, 0x66, 0x58, 0xab, 0xb5, 0x67, 0x8e, 0xf4, 0x07, 0xd7, 0x0f, 0x19, 0xa3,
0x82, 0x03, 0x07, 0x30, 0x82, 0x03, 0x03, 0x30, 0x1e, 0x06, 0x03, 0x55, 0x1d, 0x11, 0x04, 0x17,
0x30, 0x15, 0x82, 0x13, 0x76, 0x61, 0x6c, 0x69, 0x64, 0x65, 0x63, 0x2e, 0x65, 0x6e, 0x74, 0x72,
0x75, 0x73, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x30, 0x82, 0x01, 0x7f, 0x06, 0x0a, 0x2b, 0x06, 0x01,
0x04, 0x01, 0xd6, 0x79, 0x02, 0x04, 0x02, 0x04, 0x82, 0x01, 0x6f, 0x04, 0x82, 0x01, 0x6b, 0x01,
0x69, 0x00, 0x75, 0x00, 0x55, 0x81, 0xd4, 0xc2, 0x16, 0x90, 0x36, 0x01, 0x4a, 0xea, 0x0b, 0x9b,
0x57, 0x3c, 0x53, 0xf0, 0xc0, 0xe4, 0x38, 0x78, 0x70, 0x25, 0x08, 0x17, 0x2f, 0xa3, 0xaa, 0x1d,
0x07, 0x13, 0xd3, 0x0c, 0x00, 0x00, 0x01, 0x64, 0x37, 0x31, 0xa4, 0xc9, 0x00, 0x00, 0x04, 0x03,
0x00, 0x46, 0x30, 0x44, 0x02, 0x20, 0x21, 0xbd, 0x0f, 0xc2, 0x30, 0x9c, 0x34, 0xe7, 0x21, 0x26,
0x0f, 0x19, 0xcb, 0x0c, 0xff, 0xd0, 0xbc, 0x26, 0xda, 0xf1, 0x0e, 0x25, 0x44, 0x9a, 0xa3, 0xa0,
0x6a, 0x35, 0x42, 0x2c, 0xc9, 0xa6, 0x02, 0x20, 0x49, 0x47, 0x1d, 0xec, 0x41, 0xd3, 0xa2, 0xc4,
0xc9, 0x75, 0x83, 0x5c, 0xee, 0xd4, 0xa8, 0xeb, 0x56, 0x4f, 0xab, 0xaa, 0x91, 0xd2, 0x4d, 0xf1,
0x5e, 0x13, 0xcf, 0x04, 0xfd, 0x07, 0x8a, 0x38, 0x00, 0x77, 0x00, 0xdd, 0xeb, 0x1d, 0x2b, 0x7a,
0x0d, 0x4f, 0xa6, 0x20, 0x8b, 0x81, 0xad, 0x81, 0x68, 0x70, 0x7e, 0x2e, 0x8e, 0x9d, 0x01, 0xd5,
0x5c, 0x88, 0x8d, 0x3d, 0x11, 0xc4, 0xcd, 0xb6, 0xec, 0xbe, 0xcc, 0x00, 0x00, 0x01, 0x64, 0x37,
0x31, 0xa4, 0xbe, 0x00, 0x00, 0x04, 0x03, 0x00, 0x48, 0x30, 0x46, 0x02, 0x21, 0x00, 0x97, 0x1c,
0xbf, 0x93, 0x10, 0x7d, 0xc1, 0xa2, 0x22, 0x7d, 0x89, 0xfe, 0x11, 0x5e, 0x79, 0xd2, 0x9b, 0xc0,
0x0b, 0x78, 0xd4, 0x7d, 0x2f, 0xdb, 0x8e, 0xc2, 0x9b, 0x72, 0xc0, 0x47, 0xf5, 0xc4, 0x02, 0x21,
0x00, 0xd2, 0x1d, 0x32, 0x4e, 0x47, 0x3e, 0x9f, 0x66, 0xef, 0x2e, 0x2c, 0x58, 0x69, 0x57, 0x9d,
0xb9, 0xa7, 0x0e, 0xe2, 0x1d, 0x58, 0x76, 0x75, 0x29, 0xf7, 0x82, 0x97, 0x2d, 0x78, 0xe6, 0xe1,
0x0d, 0x00, 0x77, 0x00, 0xbb, 0xd9, 0xdf, 0xbc, 0x1f, 0x8a, 0x71, 0xb5, 0x93, 0x94, 0x23, 0x97,
0xaa, 0x92, 0x7b, 0x47, 0x38, 0x57, 0x95, 0x0a, 0xab, 0x52, 0xe8, 0x1a, 0x90, 0x96, 0x64, 0x36,
0x8e, 0x1e, 0xd1, 0x85, 0x00, 0x00, 0x01, 0x64, 0x37, 0x31, 0xa5, 0x3b, 0x00, 0x00, 0x04, 0x03,
0x00, 0x48, 0x30, 0x46, 0x02, 0x21, 0x00, 0xf0, 0x81, 0xa9, 0x5a, 0x92, 0xbc, 0x01, 0x08, 0x5c,
0xc3, 0xe5, 0xa1, 0x5d, 0x2d, 0x7c, 0x11, 0x04, 0x76, 0x34, 0x6b, 0x02, 0x18, 0x86, 0xe3, 0x4e,
0xdc, 0x3e, 0xa6, 0x30, 0xb7, 0xd0, 0x05, 0x02, 0x21, 0x00, 0xb9, 0x07, 0xf8, 0x47, 0x9a, 0x2c,
0x30, 0xb9, 0x03, 0xdd, 0x78, 0x71, 0xbe, 0xb4, 0xc3, 0xbe, 0x70, 0xfe, 0x3d, 0x43, 0x9f, 0xf0,
0xfc, 0xcf, 0x7a, 0xfb, 0x16, 0x92, 0xec, 0x4d, 0xa9, 0xc4, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x1d,
0x0f, 0x01, 0x01, 0xff, 0x04, 0x04, 0x03, 0x02, 0x07, 0x80, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d,
0x25, 0x04, 0x16, 0x30, 0x14, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x01, 0x06,
0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x02, 0x30, 0x63, 0x06, 0x08, 0x2b, 0x06, 0x01,
0x05, 0x05, 0x07, 0x01, 0x01, 0x04, 0x57, 0x30, 0x55, 0x30, 0x23, 0x06, 0x08, 0x2b, 0x06, 0x01,
0x05, 0x05, 0x07, 0x30, 0x01, 0x86, 0x17, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x6f, 0x63,
0x73, 0x70, 0x2e, 0x65, 0x6e, 0x74, 0x72, 0x75, 0x73, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x30, 0x2e,
0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x02, 0x86, 0x22, 0x68, 0x74, 0x74, 0x70,
0x3a, 0x2f, 0x2f, 0x61, 0x69, 0x61, 0x2e, 0x65, 0x6e, 0x74, 0x72, 0x75, 0x73, 0x74, 0x2e, 0x6e,
0x65, 0x74, 0x2f, 0x6c, 0x31, 0x6a, 0x2d, 0x65, 0x63, 0x31, 0x2e, 0x63, 0x65, 0x72, 0x30, 0x33,
0x06, 0x03, 0x55, 0x1d, 0x1f, 0x04, 0x2c, 0x30, 0x2a, 0x30, 0x28, 0xa0, 0x26, 0xa0, 0x24, 0x86,
0x22, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x63, 0x72, 0x6c, 0x2e, 0x65, 0x6e, 0x74, 0x72,
0x75, 0x73, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x2f, 0x6c, 0x65, 0x76, 0x65, 0x6c, 0x31, 0x6a, 0x2e,
0x63, 0x72, 0x6c, 0x30, 0x4a, 0x06, 0x03, 0x55, 0x1d, 0x20, 0x04, 0x43, 0x30, 0x41, 0x30, 0x36,
0x06, 0x0a, 0x60, 0x86, 0x48, 0x01, 0x86, 0xfa, 0x6c, 0x0a, 0x01, 0x02, 0x30, 0x28, 0x30, 0x26,
0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x02, 0x01, 0x16, 0x1a, 0x68, 0x74, 0x74, 0x70,
0x3a, 0x2f, 0x2f, 0x77, 0x77, 0x77, 0x2e, 0x65, 0x6e, 0x74, 0x72, 0x75, 0x73, 0x74, 0x2e, 0x6e,
0x65, 0x74, 0x2f, 0x72, 0x70, 0x61, 0x30, 0x07, 0x06, 0x05, 0x67, 0x81, 0x0c, 0x01, 0x01, 0x30,
0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0xc3, 0xf9, 0x45, 0x03,
0xbe, 0xc8, 0xf9, 0x0b, 0x3c, 0x45, 0x35, 0xf3, 0xeb, 0x72, 0xec, 0xe7, 0xe8, 0xeb, 0x94, 0x9b,
0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0xfe, 0x27, 0xb3, 0xa1, 0x4b,
0xa8, 0x2c, 0x90, 0x26, 0xa7, 0x98, 0xe1, 0xa7, 0xc7, 0x5d, 0x03, 0x3d, 0x21, 0x7c, 0x01, 0x30,
0x09, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x04, 0x02, 0x30, 0x00, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86,
0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x03, 0x69, 0x00, 0x30, 0x66, 0x02, 0x31, 0x00, 0x90, 0x2d,
0x44, 0x00, 0x05, 0x3e, 0xf8, 0x4e, 0x67, 0x47, 0xcc, 0x0b, 0x41, 0xf7, 0xbb, 0x8d, 0x17, 0x9f,
0x99, 0xcb, 0x5e, 0x70, 0xce, 0x49, 0xa5, 0x29, 0xe4, 0x52, 0x39, 0x70, 0x88, 0x7f, 0x60, 0xb9,
0x60, 0xa8, 0xec, 0x81, 0xd9, 0x11, 0xd4, 0xe7, 0x73, 0x65, 0x77, 0x29, 0x8c, 0xd8, 0x02, 0x31,
0x00, 0xdf, 0x59, 0xc7, 0x4f, 0x86, 0x87, 0x4d, 0x5d, 0x90, 0xb8, 0x97, 0x59, 0x4b, 0x3c, 0xe3,
0xda, 0x4f, 0x33, 0xcf, 0xc3, 0xab, 0x95, 0xaa, 0xaa, 0x08, 0x4d, 0x09, 0x5c, 0xea, 0xbd, 0x6d,
0x75, 0x19, 0x24, 0x0f, 0xa2, 0xbd, 0x8a, 0x2f, 0xd7, 0xf0, 0x4b, 0x70, 0xa0, 0x5a, 0x7b, 0xb3,
0xcd};

const char * cert_db_dir = "/Users/chris/mozilla-central/security/nss/tests/libpkix/sample_apps/";
const char * my_dir = "/Users/chris/Documents/Contracting/mozilla/testWebSites/bin/lok/tar/ogar";

void init_or_die(const char * directory) {
	auto status = NSS_Init(directory);
	if (status != SECSuccess) {
		std::cout << "failed to init cert directory at " << directory << std::endl;
		PRErrorCode err_code = PR_GetError();
		auto err_text = std::unique_ptr<char>(new char);
		auto bytes_read = PR_GetErrorText(err_text.get());
		if (bytes_read != 0) {
			std::cout << "The NSS INIT goes: " << err_text << std::endl;
		} else {
			std::cout << "got status " << err_code << " from NSS_Init but couldn't get the error text" << std::endl;
		}
		exit(err_code);
	}
}

int main(int argc, char const *argv[]) {	
	init_or_die(my_dir);
  	MyWittleTwustDomain trustDomain;
  	auto result = BuildCertChain(trustDomain, Input(entrust), Now(),
                             EndEntityOrCA::MustBeEndEntity,
                             KeyUsage::noParticularKeyUsageRequired,
                             KeyPurposeId::id_kp_serverAuth,
                             CertPolicyId::anyPolicy,
                             nullptr);
  	if (result != Success) {
    	std::cout << "well yeah, duh " << MapResultToName(result) << std::endl;
    	return 1;
  	}
	return 0;
}