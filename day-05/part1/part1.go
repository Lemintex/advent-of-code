package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

type mapRange struct {
	src, dest, rangelength int
}

var maps [][]mapRange

var seeds []int

func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("File not found")
	}
	segments := strings.Split(string(f), "\r\n\r")
	fmt.Println(len(segments))

	var s []string
	for _, l := range segments {
		_, v, _ := strings.Cut(l, ":")
		s = append(s, v)
	}
	// seedList := strings.TrimSpace(s[0])
	// fmt.Println("Seeds", seedList)
	for mapIndex, m := range s {
		maps = append(maps, []mapRange{})
		lines := strings.Split(m, "\n")
		// fmt.Println("map")
		for _, l := range lines[1:] {
			numbers := strings.Split(l, " ")
			// fmt.Println(numbers[0], " | ", numbers[1], " | ", numbers[2])
			dest, _ := strconv.Atoi(numbers[0])
			src, _ := strconv.Atoi(numbers[1])
			r, _ := strconv.Atoi(numbers[2])
			mapRange := mapRange{
				src:         src,
				dest:        dest,
				rangelength: r,
			}
			maps[mapIndex] = append(maps[mapIndex], mapRange)
		}
	}
	fmt.Println(maps)
}
