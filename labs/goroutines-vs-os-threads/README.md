Lab - Goroutines vs OS Threads
==============================
Read section 9.8 from The [Go Programming Language](https://www.amazon.com/dp/0134190440/ref=cm_sw_em_r_mt_dp_U_Uz0RDbHAH27PH) book.
Then, do the 9.4 and 9.5 exercises.

- Update the `README.md` file on instructions about how to run your programs.
- Your 2 programs must automatically generate their proper  result report depending on what parameters they received.
- Generated reports are free format and written on any file extension (i.e. .txt, .pdf, .md, .svg, .png, .jpg, .tex)

General Requirements and Considerations
---------------------------------------
- Submit only `*.go` files.
- Don't forget to handle errors properly.
- Coding best practices implementation will be also considered.


How to submit your work
=======================
```
GITHUB_USER=<your_github_user>  make submit
```
More details at: [Classify API](../../classify.md)

=======================


Instructions about how to run the programs


=======================

Exercise 9.4
The program is called gopl94.go
You can pass the maximum number of stages as a flag called "pipes" when running the program
The report is generated as a file called "report94.txt"
This report states the final pipeline output and registers how much time it took to go through all the stages

Example:

go run gopl94.go -pipes=1000000

My computer allowed up to ~1,400,000 stages before running out of memory


========================

Exercise 9.5
The program is called gopl95.go
The program receives no arguments
The report is generated as a file called "report95.txt"
This report shows how many communications per second were last sustained

Example:

go run gopl95.go

My computer sustained an average of ~3300000 messages per second


=========================
