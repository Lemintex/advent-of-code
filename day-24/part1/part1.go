package main

import (
	"fmt"
	"os"
	"strings"
	"strconv"
)

type vec3 struct {
	x, y, z int
}

type hailstone struct {
	pos, vel vec3
}

var hailstones []hailstone
func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic(err)
	}
	str := strings.Split(strings.TrimSpace(string(f)), "\n")
	for _, s := range str {
		parseHailstone(s)
	}
}

func parseHailstone(s string) {
	parts := strings.Split(strings.TrimSpace(s), " @ ")
	// fmt.Println(parts[0], "|", parts[1])
	position := strings.Split(parts[0], ", ")
	x, errx := strconv.Atoi(position[0])
	if errx != nil {
		panic(errx)
	}
	y, erry := strconv.Atoi(position[1])
	if erry != nil {
		panic(erry)
	}
	z, errz := strconv.Atoi(position[2])
	if errz != nil {
		panic(errz)
	}

	pos := vec3 {
		x: x,
		y: y,
		z: z,
	}
	velocity := strings.Split(parts[2], ", ")
	vx, errvx := strconv.Atoi(velocity[0])
	if errvx != nil {
		panic(errvx)
	}
	vy, errvy := strconv.Atoi(velocity[1])
	if errvy != nil {
		panic(errvy)
	}
	vz, errvz := strconv.Atoi(velocity[2])
	if errvz != nil {
		panic(errvz)
	}
	vel := vec3 {
		x: vx,
		y: vy,
		z: vz,
	}
	stone := hailstone {
		pos: pos,
		vel: vel,
	}
	hailstones = append(hailstones, stone)
	fmt.Println(" ")
}
