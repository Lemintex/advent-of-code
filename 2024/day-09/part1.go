package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

var input string
var drive []int
var defragmentedDrive []int

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
}

func Parse() {
	id := 0
	for i, c := range input {
		n, err := strconv.Atoi(string(c))
		if err != nil {
			log.Fatal(err)
		}
		if i%2 == 1 {
			for range n {
				drive = append(drive, -1)
			}
		} else {
			for range n {
				drive = append(drive, id)
			}
			id++
		}
	}
	fmt.Println(drive)
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0
	l, r := 0, len(drive)-1
	for l <= r {
		for drive[l] != -1 {
			l++
		}
		for drive[r] == -1 {
			r--
		}
		if l < r {
			drive[l], drive[r] = drive[r], drive[l]
		}
	}
	i := 0
	for drive[i] != -1 {
		ans += drive[i] * i
		i++
	}
	return ans, time.Since(start)
}
