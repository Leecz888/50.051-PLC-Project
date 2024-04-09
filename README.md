# 50.051-PLC-Project

<h1 align="center" style="border-bottom: none">
    <b>ICS Time Manager</b>
    <br>
    Programming Language Concepts (PLC)
    <br>
</h1>

<p align="center">
    A C program that finds common time slots with multiple ICS files
</p>

<table align="center">
<h2>Group 2-3: </h2>
  <tr>
    <td align="center"><a href="https://github.com/ssjh23"><img src="https://avatars.githubusercontent.com/u/64569228?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Sean Soo</b><br/>1005263</sub></a><br />
    <td align="center"><a href="https://github.com/ryanpantk"><img src="https://avatars.githubusercontent.com/u/66586824?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Ryan Pan</b><br/>1005037</sub></a><br />
    <td align="center"><a href="https://github.com/bloomspx"><img src="https://avatars.githubusercontent.com/bloomspx?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Soh Pei Xuan</b><br/>1005552</sub></a><br/>
    <td align="center"><a href="https://github.com/Leecz888"><img src="https://avatars.githubusercontent.com/u/90887690?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Lee Chang Zheng</b><br/>1005289</sub></a><br />
  </tr>
</table>

# Table of Contents
-   [File Directories](#file-directories)
-   [Getting Started](#getting-started)

# File Directories
```
┣ 📂 data                   # place sample ICS files here
┣ 📂 output                 # stores output csv files 
┣ 📄 csvConverter.c         # output generator
┣ 📄 eventTranslator.c      #
┣ 📄 linkedList.c           # 
┣ 📄 main.c                 # executes main() of program
┣ 📄 parser.c               # 
┗ 📄 timeManager.c          #
 ```

# Getting Started

### 1. To compile code with flags (supported on Windows, Linux and Mac)
```
gcc main.c csvConverter.c eventTranslator.c parser.c timeManager.c linkedList.c -o main -ansi -pedantic -Wall -Werror
```

### 2. To run with sample ICS files (files are placed in `/data`)
```
main.c sample1.ics sample2.ics sample3.ics sample4.ics
```
