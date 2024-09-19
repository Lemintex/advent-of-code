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
	ID                int
	bottom, top       vec3
	length, width     int
	bricksHoldingMeUp map[int]struct{}
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
			ID:                i + 1,
			bottom:            v1,
			top:               v2,
			length:            v2X - v1X + 1,
			width:             v2Y - v1Y + 1,
			bricksHoldingMeUp: make(map[int]struct{}),
		}
		brick.PrintBrick()
		bricks = append(bricks, &brick)
	}
	lenX = maxX - minX + 1
	lenY = maxY - minY + 1
	// fmt.Println(maxX, minX, lenX)
	// fmt.Println(maxY, minY, lenY)
	slices.SortFunc(bricks, func(a, b *brick) int {
		return a.bottom.z - b.bottom.z
	})
	CreateStackOnGround()
	GetNumberOfBricksThatCanBeDisintegrated()
}

func (b *brick) PrintBrick() {
	// fmt.Printf("\nStart = x: %d, y: %d, z: %d, Area: %d x %d | ", b.start.x, b.start.y, b.start.z, b.start.length, b.start.width)
	// fmt.Printf("End = x: %d, y: %d, z: %d, Area: %d x %d", b.end.x, b.end.y, b.end.z, b.end.length, b.end.width)
}

func CreateStackOnGround() {
	for i, b := range bricks {
		fmt.Printf("\nBrick %d:", i+1)
		diff := b.top.z - b.bottom.z
		if i == 0 {
			b.bottom.z = 0
			b.top.z = b.bottom.z + diff
			continue
		}
		for _, under := range bricks[:i-1] {
			if DoBricksIntersect(b, under) {
				//THE HIGHER RESTS ON THE LOWER
				fmt.Println("Intersected")
				diff := b.top.z - b.bottom.z
				b.bottom.z = under.top.z + 1
				b.top.z = b.bottom.z + diff
				goto found
			}
		}
		b.bottom.z = 0
		b.top.z = b.bottom.z + diff

	found:
	}

	for _, b := range bricks {
		fmt.Println(b)
	}
}

func DoBricksIntersect(falling, stationary *brick) bool {
	//TODO:implement
	return falling.bottom.x < stationary.top.x && falling.top.x > stationary.bottom.x && falling.bottom.y < stationary.top.y && falling.top.y > stationary.bottom.y
}

func GetNumberOfBricksThatCanBeDisintegrated() {

	total := 0
	for _, b := range bricks {
		if b.CanDisintegrate() {
			fmt.Println("Disintegrate!", b.ID)
			total++
		}
	}
	fmt.Println("Total:", total)
}
func (b *brick) CanDisintegrate() bool {
	//TODO: implement
	return len(b.bricksHoldingMeUp) > 1
}
