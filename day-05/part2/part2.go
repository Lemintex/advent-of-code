package main

import (
	"fmt"
	"os"
	"slices"
	"strconv"
	"strings"
)

type seedGroup struct {
	start, end int
}

type mapRange struct {
	dest, src, rangelength int
}

var maps [][]mapRange

var seedGroups []seedGroup

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
	pairs := [][2]int{}
	for i := 0; i < len(list); i += 2 {
		start, _ := strconv.Atoi(list[i])
		end, _ := strconv.Atoi(list[i+1])
		end += start
		pairs = append(pairs, [2]int{start, end})
	}
	for _, s := range pairs {
		seeds := seedGroup{
			start: s[0],
			end:   s[1],
		}
		seedGroups = append(seedGroups, seeds)
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
	ans := 999999999999
	for _, m := range maps {
		var n []seedGroup
		for len(seedGroups) > 0 {
			// pop
			front := seedGroups[0]
			seedGroups = seedGroups[1:]
			found := false
			for _, ranges := range m {
				os, oe := max(front.start, ranges.src), min(front.end, ranges.src+ranges.rangelength)
				if os < oe {
					found = true
					n = append(n, seedGroup{os - ranges.src + ranges.dest, oe - ranges.src + ranges.dest})
					if os > front.start {
						seedGroups = append(seedGroups, seedGroup{front.start, os})
					}
					if front.end > oe {

						seedGroups = append(seedGroups, seedGroup{oe, front.end})
					}
					break
				}
			}
			if !found {
				n = append(n, seedGroup{front.start, front.end})
			}
		}
		seedGroups = n
	}
	for _, s := range seedGroups {
		ans = min(ans, s.start)
	}
	fmt.Println("Ans:", ans)
}
