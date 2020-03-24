package main

import (
	"fmt"
	"os"
	"path/filepath"
)

// scanDir stands for the directory scanning implementation

var dirs,symlins,devs,socs,oths int = 0,0,0,0,0

func scanDir(dir string) error {

	filepath.Walk(dir, classification)
	fmt.Println(dirs)
	fmt.Println(symlins)
	fmt.Println(devs)
	fmt.Println(socs)
	fmt.Println(oths)

	fmt.Println("Directory Scanner Tool")
	fmt.Println("+-----------------------+-----------+")
	fmt.Println("| Path\t\t\t| ",dir)
	fmt.Println("+-----------------------+-----------+")
	fmt.Println("| Directories\t\t| ",dirs)
	fmt.Println("| Symbolic Links\t| ",symlins)
	fmt.Println("| Devices\t\t| ",devs)
	fmt.Println("| Sockets\t\t| ",socs)
	fmt.Println("| Other files\t\t| ",oths)
	fmt.Println("+-----------------------+-----------+")

	return nil
}

func classification(path string, info os.FileInfo, err error) error {
	mode := os.FileMode.String(info.Mode())[0:1]
	switch mode {
		case "d":
			dirs++
		case "L":
			symlins++
		case "D":
			devs++
		case "S":
			socs++
		default:
			oths++
	}
	return nil;
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	scanDir(os.Args[1])
}
