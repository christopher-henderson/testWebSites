
const Cc = Components.classes;
const Ci = Components.interfaces;
const certdb = Cc["@mozilla.org/security/x509certdb;1"].getService(Ci.nsIX509CertDB);

let { FileUtils } = Cu.import("resource://gre/modules/FileUtils.jsm", {});
let { NetUtil } = Cu.import("resource://gre/modules/NetUtil.jsm", {});


print("baals");
let db = certdb.getCerts();
let e = db.asPKCS7Blob();


function readFile() {
	var file = Cc["@mozilla.org/file/directory_service;1"].
	           getService(Ci.nsIProperties).
	           get("TmpD", Ci.nsIFile);
	file.append("test-file.tmp");
	file.createUnique(Ci.nsIFile.NORMAL_FILE_TYPE, 0666);

	// Then, we need an output stream to our output file.
	var ostream = Cc["@mozilla.org/network/file-output-stream;1"].
	              createInstance(Ci.nsIFileOutputStream);
	ostream.init(file, -1, -1, 0);

	// Finally, we need an input stream to take data from.
	const TEST_DATA = "this is a test string";
	let istream = Cc["@mozilla.org/io/string-input-stream;1"].
	              createInstance(Ci.nsIStringInputStream);
	istream.setData(TEST_DATA, TEST_DATA.length);

	NetUtil.asyncCopy(istream, ostream, function(aResult) {
	  if (!Components.isSuccessCode(aResult)) {
	    // an error occurred!
	  }
	})
}

readFile();

// let e = db.getEnumerator();
// while (e.hasMoreElements()) {
// 	print(e.getNext());
// }