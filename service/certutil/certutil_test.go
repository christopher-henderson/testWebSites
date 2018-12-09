package certutil

import (
	"testing"
)

const DB = "/Users/chris/Documents/Contracting/mozilla/testWebSites/testdb"
const DIST = "/Users/chris/Documents/Contracting/mozilla/testWebSites/dist/Debug"

func init() {
	Init(DB, DIST)
}

// func TestInit(t *testing.T) {
// 	err := os.Mkdir("testdb", 0777)
// 	if err != nil {
// 		t.Fatal(err)
// 	}
// 	defer os.RemoveAll("testdb")
// 	currentPath := os.Getenv("PATH")
// 	currentLDPath := os.Getenv(LIBRARY_PATH)
// 	Init("testdb", "/Users/chris/Documents/Contracting/mozilla/testWebSites/dist/Debug")
// 	defer func() {
// 		os.Setenv("PATH", currentPath)
// 		os.Setenv(LIBRARY_PATH, currentLDPath)
// 	}()
// 	// assert some stuff
// }

// func TestExec(t *testing.T) {
// 	Init("testdb", "/Users/chris/Documents/Contracting/mozilla/testWebSites/dist/Debug")
// 	// t.Log(os.Getenv("PATH"))
// 	// t.Log(os.Getenv(LIBRARY_PATH))
// 	f, err := os.Create("thing")
// 	if err != nil {
// 		t.Fatal(err)
// 	}
// 	logging.AddLog(f)
// 	cmd := exec.Command("certutil", "asd")
// 	out, err := cmd.CombinedOutput()
// 	t.Log(string(out))
// 	// err = execute([]string{"asd"})
// 	// if err != nil {
// 	// 	t.Fatal(err)
// 	// }
// }

//  -V -n expiredec.entrust.net -u V -d testdb/
func TestExecuteExpired(t *testing.T) {
	out, err := Execute([]string{"-V", "-n", "expiredec.entrust.net", "-u", "V", "-d", "testdb"})
	t.Log(out)
	t.Log(err)
}
