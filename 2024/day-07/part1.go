package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

type calibration struct {
	target   int
	operands []int
}

var input []string
var calibrations []calibration

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
	Parse()
	answer, time := Solve()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
}

func Parse() {
	for _, i := range input {
		i = strings.TrimSpace(i)
	}
	for _, i := range input {
		strTarget, operands, _ := strings.Cut(i, ": ")
		target, err := strconv.Atoi(strTarget)
		if err != nil {
			log.Fatal(err)
		}
		strOperands := strings.Split(operands, " ")
		ops := make([]int, 0)
		for _, o := range strOperands {
			op, err := strconv.Atoi(o)
			if err != nil {
				log.Fatal(err)
			}
			ops = append(ops, op)
		}
		cal := calibration{
			target:   target,
			operands: ops,
		}
		calibrations = append(calibrations, cal)
	}
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0
	for _, cal := range calibrations {
		if CanBeSolved(cal, 0, 0) {
			ans += cal.target
		}
	}

	return ans, time.Since(start)
}

func CanBeSolved(cal calibration, arrayIndex, num int) bool {
	if arrayIndex == len(cal.operands) {
		return num == cal.target
	}
	add := num + cal.operands[arrayIndex]
	mul := num * cal.operands[arrayIndex]
	return CanBeSolved(cal, arrayIndex + 1, add) || CanBeSolved(cal, arrayIndex + 1, mul)
}
