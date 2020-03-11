package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	matrix := make([][]uint8, dy)
	for i:=range matrix {
		matrix[i] = make([]uint8,dx)
	}
	
	for x:=0; x<dy; x++{
		for y:=0; y<dx; y++{
			matrix[x][y] = uint8(x+y)
		}
	}
	
	return matrix
}

func main() {
	pic.Show(Pic)
}
