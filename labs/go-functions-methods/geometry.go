package main

import (
	"math"
	"os"
	"fmt"
	"math/rand"
	"strconv"
)

type Point struct{ x, y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X() - p.X(), q.Y() - p.Y())
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	d := math.Hypot(q.X() - p.X(), q.Y() - p.Y())
	return math.Round(d*100)/100
	//return d
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Perimeter() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	sum += path[0].Distance(path[len(path)-1])
	return math.Round(sum*100)/100
	//return sum
}

var points Path

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

func generatePoint() Point {
	x := -100 + rand.Float64() * 200
	x = math.Round(x*100)/100
	y := -100 + rand.Float64() * 200
	y = math.Round(y*100)/100

	return Point{x, y}
}

func onSegment(p, q, r Point) bool{
	if (q.X() <= math.Max(p.X(), r.X()) && q.X() >= math.Min(p.X(), r.X()) && q.Y() <= math.Max(p.Y(), r.Y()) && q.Y() >= math.Min(p.Y(), r.Y())) {
		return true
	} else {
		return false
	}
}

func orientation(p, q, r Point) int{
	val := (q.Y() - p.Y()) * (r.X() - q.X()) - (q.X() - p.X()) * (r.Y() - q.Y())
	if val == 0 {
		return 0
	} else if val > 0{
		return 1
	} else {
		return 2
	}
}

func doIntersect(p1, q1, p2, q2 Point) bool{
	o1 := orientation(p1, q1, p2)
	o2 := orientation(p1, q1, q2)
	o3 := orientation(p2, q2, p1)
	o4 := orientation(p2, q2, q1)

	if (o1 != o2 && o3 != o4) {
		return true
	}
	if (o1 == 0 && onSegment(p1, p2, q1)) {
		return true
	}
	if (o2 == 0 && onSegment(p1, q2, q1)) {
		return true
	}
	if (o3 == 0 && onSegment(p2, p1, q2)) {
		return true
	}
	if (o4 == 0 && onSegment(p2, q1, q2)) {
		return true
	}

	return false
}

func main() {

	if len(os.Args) < 2{
		fmt.Println("Error. Usage: geometry.go sides")
		return
	}

	sides,_ := strconv.Atoi(os.Args[1])

	if len(os.Args) > 2 {
		seed,_ := strconv.Atoi(os.Args[2])
		rand.Seed(int64(seed))
	}

	if sides >= 3 {

		points = make([]Point, sides)

		fmt.Printf("- Generating a [%v] sides figure\n",sides)

		//points[0] = Point{-3, 1}
		//points[1] = Point{2, 3}
		//points[2] = Point{0, 0}
		//points[3] = Point{-1.5, -2.5}

		noTriesLeft := false

		fmt.Println("- Figure's vertices")

		for i := 0; i < sides; i++  {
			tries := 0
			noTriesLeft = false
			points[i] = generatePoint()
			if i > 2 {
				for j := 0; j < i - 2; j++ {
					if doIntersect(points[j], points[j+1], points[i-1], points[i]) {
						points[i] = generatePoint()
						tries++
						j = 0
					}
					if (i == sides - 1) {
						for k := 1; k < i - 1; k++ {
							if doIntersect(points[0], points[i], points[k], points[k+1]) {
								points[i] = generatePoint()
								tries++
								j = 0
								break
							}
						}
					}
					if tries >= 100 {
						noTriesLeft = true
						break
					}
				}
				if(noTriesLeft){
					break
				}
			}
			fmt.Printf("\t- (%v, %v)\n",points[i].X(), points[i].Y())
		}

		if noTriesLeft {
			fmt.Println("Too many failed point generation attemps. Program has been terminated.\nYou can enter a seed as a third parameter to get a different result.")
		} else {
			fmt.Println("- Figure's Perimeter")
			fmt.Print("- ")
			for i := 0; i < sides; i++ {
				if i != 0 {
					fmt.Print(" + ")
				}
				fmt.Print(points[i].Distance(points[(i+1)%sides]))
			}
			fmt.Printf(" = %v\n",points.Perimeter())
		}
	} else {
		fmt.Println("Can't generate a figure with less than 3 sides")
	}
}

//!-path
