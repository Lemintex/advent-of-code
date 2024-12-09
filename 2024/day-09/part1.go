package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

type file struct {
	id      int
	size    int
	isSpace bool
}

var input string
var drive []file
var defragmentedDrive []file

func ReadFile() {
	var err error
	input, err = Read("input.txt")
	if err != nil {
		log.Fatal(err)
		return
	}
}

func Read(filename string) (string, error) {
	s, err := os.ReadFile(filename)
	if err != nil {
		log.Fatal(err)
		return "", err
	}
	in := strings.TrimSpace(string(s))

	// depending on the grouping of the input we may want to change the seperator
	return in, nil
}

func main() {
	ReadFile()
	Parse()
	answer, time := Solve()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
	fmt.Print("\n")
	Debug(drive)
	fmt.Print("\n")
	Debug(defragmentedDrive)
}

func Parse() {
	i := 0
	fileID := 0
	for range len(input)/2 + 1 {
		if i >= len(input) {
			break
		}
		size, err := strconv.Atoi(string(input[i]))
		if err != nil {
			log.Fatal(err)
		}
		f := file{
			id:      fileID,
			size:    size,
			isSpace: false,
		}
		fileID++
		drive = append(drive, f)
		i++
		if i >= len(input) {
			break
		}
		size, err = strconv.Atoi(string(input[i]))
		if err != nil {
			log.Fatal(err)
		}
		s := file{
			id:      -1,
			size:    size,
			isSpace: true,
		}
		drive = append(drive, s)
		i++
	}
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0
	l, r := 0, len(drive)-1
	fmt.Println(drive)
	for l < r {
		MoveEndFileToFront(&l, &r)
		fmt.Println(l, r)
	}
	return ans, time.Since(start)
}

func MoveEndFileToFront(l, r *int) {
	left, right := drive[*l], drive[*r]
	for !left.isSpace {
		defragmentedDrive = append(defragmentedDrive, left)
		*l++
		left = drive[*l]
	}
	for right.isSpace {
		*r--
		right = drive[*r]
	}
	fmt.Println(left, right, *l, *r)
	if drive[*l].size == drive[*r].size {
		fmt.Println("==", right)
		defragmentedDrive = append(defragmentedDrive, right)
		*l++
		*r--
	} else if left.size > right.size {
		left.size -= right.size
		fmt.Println(">", right)
		defragmentedDrive = append(defragmentedDrive, right)
		*r--
	} else {
		for right.size > 0 {
		fmt.Println(" ", right)
			right.size -= left.size
			defragmentedDrive = append(defragmentedDrive, right)
			*l++
			left = drive[*l]
		}
	}
}

func Debug(d []file) {
	fmt.Println("=============")
	fmt.Println(d)
	for _, f := range d {
		fmt.Println(f)
		if f.isSpace {
			fmt.Print(strings.Repeat(".", f.size))
		} else {
			fmt.Print(strings.Repeat(strconv.Itoa(f.id), f.size))
		}
	}
}
