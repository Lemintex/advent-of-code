package main

import (
	"fmt"
	"os"
	"slices"
	"strconv"
	"strings"
)

type vec3 struct {
	x, y, z int
}

type brick struct {
	ID                    rune
	bottom, top           vec3
	length, width, height int
	supports, supportedBy map[*brick]struct{}
}

type mapInfo struct {
	height, brickID int
}

var minX, minY, maxX, maxY, lenX, lenY int
var bricks []*brick
var brickMap []string
var mapHeight [][]mapInfo

func main() {
	minX, minY = 1000000, 1000000
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("err`=")
	}
	brickMap = strings.Split(string(f), "\n")
	for i, l := range brickMap {
		ends := strings.Split(l, "~")
		// TODO: find proper fix
		if len(ends) == 1 { // temporary
			break
		}
		fmt.Println(ends)
		b1, b2 := strings.Split(ends[0], ","), strings.Split(ends[1], ",")

		v1X, _ := strconv.Atoi(b1[0])
		v1Y, _ := strconv.Atoi(b1[1])
		v1Z, _ := strconv.Atoi(b1[2])

		v2X, _ := strconv.Atoi(b2[0])
		v2Y, _ := strconv.Atoi(b2[1])
		v2Z, _ := strconv.Atoi(b2[2])

		minX = min(minX, v1X)
		minY = min(minY, v1Y)
		maxX = max(maxX, v2X)
		maxY = max(maxY, v2Y)

		v1 := vec3{
			x: v1X,
			y: v1Y,
			z: v1Z,
		}

		v2 := vec3{
			x: v2X,
			y: v2Y,
			z: v2Z,
		}
		brick := brick{
			ID:          rune('A' + i),
			bottom:      v1,
			top:         v2,
			length:      v2X - v1X + 1,
			width:       v2Y - v1Y + 1,
			height:      v2Z - v1Z + 1,
			supports:    make(map[*brick]struct{}),
			supportedBy: make(map[*brick]struct{}),
		}
		bricks = append(bricks, &brick)
	}
	lenX = maxX - minX + 1
	lenY = maxY - minY + 1
	slices.SortFunc(bricks, func(a, b *brick) int {
		return a.bottom.z - b.bottom.z
	})
	CreateStackOnGround()
	slices.SortFunc(bricks, func(a, b *brick) int {
		return a.bottom.z - b.bottom.z
	})
	debugWriteStack()
	GetNumHoldingUpOfBricks()
}

// create the stack by making each block 'fall' downwards to be on top of the highest already fallen block intersected with
func CreateStackOnGround() {
	for i, b := range bricks {
		maxZ := 1
		// loop through the 'fallen' bricks to get the Z of the highst intersecting block
		for _, u := range bricks[:i] {
			if DoBricksIntersect(b, u) {
				maxZ = max(maxZ, u.top.z+1)
			}
		}
		b.bottom.z = maxZ
		b.top.z = maxZ + b.height
	}
}

func GetNumHoldingUpOfBricks() {
	for i, upper := range bricks {
		for _, lower := range bricks[:i] {
			// is upper 'on' lower
			if DoBricksIntersect(lower, upper) && upper.bottom.z == lower.top.z+1 {
				lower.supports[upper] = struct{}{}
				upper.supportedBy[lower] = struct{}{}
			}
		}
	}

	ans := 0
	for _, lower := range bricks {
		disintegrate := true
		if len(lower.supports) == 0 {
			goto disintegrate
		}
		for upper, _ := range lower.supports {
			_, exists := upper.supportedBy[lower]
			if exists {
				if len(upper.supportedBy) == 1 {
					disintegrate = false
					goto disintegrate
				}
			}
		}
	disintegrate:
		if disintegrate {
			ans++
		}
	}
	fmt.Println(ans)
}

func DoBricksIntersect(falling, stationary *brick) bool {
	return max(falling.bottom.x, stationary.bottom.x) <= min(falling.top.x, stationary.top.x) && max(falling.bottom.y, stationary.bottom.y) <= min(falling.top.y, stationary.top.y)
}

func debugWriteStack() {
	var s string
	for _, b := range bricks {
		var add string
		add = string(b.bottom.x) + "," + string(b.bottom.y) + "," + string(b.bottom.z) + "-" + string(b.top.x) + "," + string(b.top.y) + "," + string(b.top.z) + "\n"
		// s += string(b)
		s += add
		// fmt.Printf("s: %v\n", s)
	}
	os.WriteFile("../testgo.txt", []byte(s), os.ModeAppend.Perm())
}
