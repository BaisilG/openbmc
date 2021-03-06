/**
 * Copyright 2020-present Facebook. All Rights Reserved.
 *
 * This program file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program in a file named COPYING; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 */

package utils

import (
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"runtime"
)

var SourceRootDir string

func init() {
	// get the source root directory
	_, filename, _, ok := runtime.Caller(0)
	if !ok {
		panic("No caller information")
	}
	// go up three paths, as this file is three directories deep
	SourceRootDir = filepath.Dir(filepath.Dir(filepath.Dir(filename)))
}

func GetExecutablePath() string {
	// get the executable's (flashy's) path
	exPath, err := os.Executable()
	if err != nil {
		panic(err)
	}

	return exPath
}

// sanitize os.Args[0] to get exactly the binary name required
// e.g. if flashy was placed as /tmp/flashy
// and a call was made inside /var for ../tmp/abc/def
// this should return abc/def
func SanitizeBinaryName(arg string) string {
	// full path of the binary requested
	fullPath, err := filepath.Abs(arg)
	if err != nil {
		panic(err)
	}
	exPath := GetExecutablePath()
	exDir := filepath.Dir(exPath)

	// now we truncate fullPath to get the filepath required
	// e.g. fullPath: /tmp/abc/def
	// &    exDir	: /tmp
	// =>   binName : abc/def
	binName := fullPath[len(exDir)+1:]

	return binName
}

// from the absolute path of a file, get the intended symlink path
// e.g. /files/checks/abc.go => checks/abc
func GetSymlinkPathForSourceFile(path string) string {
	// truncate to get the symlink path
	// also truncate ".go"
	symlinkPath := path[len(SourceRootDir)+1 : len(path)-3]
	return symlinkPath
}

// basic file utilities as function variables for mocking purposes
var osStat = os.Stat
var RemoveFile = os.Remove
var TruncateFile = os.Truncate
var WriteFile = ioutil.WriteFile
var ReadFile = ioutil.ReadFile

// PathExists returns true when the path exists
// (can be file/directory)
// defaults to `false` if os.Stat returns any other non-nil error
var PathExists = func(path string) bool {
	_, err := osStat(path)
	if err == nil {
		// exists for sure
		return true
	} else if os.IsNotExist(err) {
		// does not exist for sure
		return false
	} else {
		// may or may not exist
		log.Printf("Existence check of path '%v' returned error '%v', defaulting to false",
			path, err)
		return false
	}
}

// FileExists returns true when the file exists
// also checks that the file is not a directory
// defaults to `false` if os.Stat returns any other non-nil error
var FileExists = func(filename string) bool {
	if PathExists(filename) {
		// err guaranteed to be nil by PathExists
		info, _ := osStat(filename)
		// confirm is not a directory
		return !info.IsDir()
	}
	return false
}

// append to end of file
var AppendFile = func(filename, data string) error {
	// create if non-existent
	f, err := os.OpenFile(filename, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		return err
	}
	_, err = f.Write([]byte(data))
	if err != nil {
		return err
	}
	err = f.Close()
	if err != nil {
		return err
	}
	return nil
}
