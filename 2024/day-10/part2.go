package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

var input []string

func ReadFile() {
	var err error
	input, err = Read("input.txt")
	if err != nil {
		log.Fatal(err)
		return
	}
}

func Read(filename string) ([]string, error) {
	s, err := os.ReadFile(filename)
	if err != nil {
		log.Fatal(err)
		return nil, err
	}
	in := strings.TrimSpace(string(s))

	// depending on the grouping of the input we may want to change the seperator
	return strings.Split(in, "\n"), nil
}

func main() {
	ReadFile()
	answer, time := Solve()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
}

func Parse(in []string) {//edit return type as needed
	// return some kind of data structure once the input has been parsed
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0

	return ans, time.Since(start)
}
