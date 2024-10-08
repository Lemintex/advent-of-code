package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
	"slices"
)

type mapRange struct {
	dest, src, rangelength int
}

var maps [][]mapRange

var seeds []int

func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("File not found")
	}
	segments := strings.Split(string(f), "\n\n")
	fmt.Println(len(segments))

	var s []string
	for _, l := range segments {
		_, v, _ := strings.Cut(strings.TrimSpace(l), ":")
		s = append(s, strings.TrimSpace(v))
		fmt.Println("v:", v)
	}
	seedList := strings.TrimSpace(s[0])
	list := strings.Split(seedList, " ")
	for _, s := range list {
		seed, _ := strconv.Atoi(s)
		seeds = append(seeds, seed)
	}
	fmt.Println("Seeds", seedList)
	mapIndex := -1
	for _, m := range s {
		if mapIndex == -1 {
			mapIndex = 0
			continue
		}
		maps = append(maps, []mapRange{})
		lines := strings.Split(strings.TrimSpace(m), "\n")
		fmt.Println(s)
		// fmt.Println("map")
		for _, l := range lines {
			fmt.Println("l", l)
			numbers := strings.Split(l, " ")
			fmt.Println(len(numbers), numbers)
			fmt.Println(numbers[0], " | ", numbers[1], " | ", numbers[2])
			dest, _ := strconv.Atoi(numbers[0])
			src, _ := strconv.Atoi(numbers[1])
			r, _ := strconv.Atoi(numbers[2])
			mapRange := mapRange{
				src:         src,
				dest:        dest,
				rangelength: r,
			}
			fmt.Println("Adding", len(maps), mapIndex)
			maps[mapIndex] = append(maps[mapIndex], mapRange)
		}
			mapIndex++
	}
	SortMaps()
	fmt.Println(maps)
	fmt.Println("Seeds:", seedList)
	fmt.Println("seed map", maps[0])
	fmt.Println("soil map", maps[1])
	fmt.Println("fertilizer map", maps[2])
	fmt.Println("water map", maps[3])
	fmt.Println("light map", maps[4])
	fmt.Println("temperature map", maps[5])
	fmt.Println("humidity map", maps[6])

	HandleSeeds()
}

func SortMaps() {
	for _, m := range maps {
		slices.SortFunc(m, func(a, b mapRange) int {
			return a.src - b.src
		})
	}
}

func HandleSeeds() {
	for _, s := range seeds {
		fmt.Println(s)
	}
}
