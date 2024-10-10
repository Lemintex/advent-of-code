package main

import (
	"fmt"
	"os"
	"slices"
	"strconv"
	"strings"
)

type seedGroup struct {
	start, size int
}

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
	ParseInput(f)
	SortMaps()
	HandleSeeds()
}

func ParseInput(f []byte) {
	segments := strings.Split(string(f), "\n\n")

	var s []string
	for _, l := range segments {
		_, v, _ := strings.Cut(strings.TrimSpace(l), ":")
		s = append(s, strings.TrimSpace(v))
	}
	seedList := strings.TrimSpace(s[0])
	list := strings.Split(seedList, " ")
	for _, s := range list {
		seed, _ := strconv.Atoi(s)
		seeds = append(seeds, seed)
	}
	mapIndex := -1
	for _, m := range s {
		// skips the seed list
		if mapIndex == -1 {
			mapIndex = 0
			continue
		}
		maps = append(maps, []mapRange{})
		lines := strings.Split(strings.TrimSpace(m), "\n")
		for _, l := range lines {
			numbers := strings.Split(l, " ")
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
		mapIndex++
	}
}

func SortMaps() {
	for _, m := range maps {
		slices.SortFunc(m, func(a, b mapRange) int {
			return a.src - b.src
		})
	}
}

func HandleSeeds() {
	ans := 99999999999
	for _, s := range seeds {
		for i := 0; i < len(maps); i++ {
			currentMap := maps[i]
			for mapIndex := range currentMap {
				if currentMap[mapIndex].src <= s && currentMap[mapIndex].src+currentMap[mapIndex].rangelength > s {
					s = currentMap[mapIndex].dest + (s - currentMap[mapIndex].src)
					break
				}
			}
		}
		ans = min(ans, s)
	}
	fmt.Println("Ans:", ans)
}
